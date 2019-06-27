//
// Created by wulinze on 19-6-21.
//

#ifndef FILESYSTEM_FILEMANAGER_H
#define FILESYSTEM_FILEMANAGER_H

#include "BlockManager.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <time.h>



#define uint unsigned int
#define SYSTBLSIZE 40       // 系统打开文件表大小
#define USRTBLSIZE 40       // 用户打开文件表大小
#define HASHSIZE 128        // 内存i节点hash表长度
#define NADDR 10            // 每个i节点最多指向10块

static int ID = 0;        // i节点初始id

struct dinode;
struct inode;
struct Blockinfo;


enum class Mod:char{
    ___=0x00,__x=0x01,_w_=0x02,_wx=0x03,r__=0x04,r_x=0x05,rw_=0x06,rwx=0x07
};
enum class Type:char{
    dir=0x01,normal=0x02,pipe=0x03,link=0x04
};
enum class BlockType:char{
    index=0x00,data=0x01
};

struct Blockinfo{
    int index;

    BlockType type;

    Blockinfo(int data,BlockType type){
        this->index = data;
        this->type = type;
    }
    Blockinfo(){
        index = -1;
        type = BlockType ::data;
    }
};

//磁盘i节点
struct dinode{
    uint di_number{};                 //关联文件数，为0则删除该文件
    Mod di_mode;		              //权限
    Type di_type;                     //类型
    uint di_uid{};			          //用户id
    uint di_size{};			          //文件大小
    Blockinfo di_first;               //文件对应首块
    time_t create_time{};             //创建时间
    time_t rencent_open{};            //最近打开时间



    explicit dinode(uint uid, int data) : di_first(Blockinfo{data,BlockType ::data}) {
        di_number = 0;
        di_mode = Mod ::r__;
        di_type = Type ::dir;
        di_uid = uid;
        di_size = 1;


        create_time = time(nullptr);
        rencent_open = time(nullptr);
    }
    explicit dinode()= default;

    std::unordered_map<std::string,int> getChildren(BlockManager* bm){
        std::unordered_map<std::string,int> res;

        if(this->di_type != Type::dir){
            return {};
        } else{
            int cur = 0;
            int* size = (int *)bm->ReadBlock(this->di_first.index,cur, sizeof(int));
            cur += sizeof(int);
            for(int i=0;i<*size;i++){
                std::string name;
                int inode_pos;
                name = (char *)bm->ReadBlock(di_first.index,cur, 20);
                cur += 20;
                inode_pos = *(int *)bm->ReadBlock(di_first.index,cur, sizeof(int));
                cur += sizeof(int);
                res.insert(std::make_pair(name,inode_pos));
            }

            return res;
        }

    }
    std::unordered_map<std::string,int> getCurrentCatalogue(BlockManager* bm,uint pos){
        if(this->di_type == Type::dir){
            std::unordered_map<std::string,int> res = getChildren(bm);

            res.insert(std::make_pair("../",pos));

            return res;
        } else{
            return {};
        }
    }

    void operator = (const dinode& d){
        di_number = d.di_number;
        di_mode = d.di_mode;
        di_type = d.di_type;
        di_uid = d.di_uid;
        di_size = d.di_size;
        di_first = d.di_first;
        create_time = d.create_time;
        rencent_open = d.rencent_open;
    }

};

//内存i节点
struct inode{

    uint i_id;			            //内存i节点标识
    uint pos;                       //磁盘i节点块号
//    uint offset;                  //磁盘i节点在块内的偏移量


    dinode cur_node;


};

//系统打开表表项
struct sysopen{
    uint s_count;                   //访问计数
    uint fileid;	                //文件描述符
    inode *s_inode;	                //指向对应内存i节点
};

//用户打开表表项
struct usropen{
    uint u_uid;			            //用户ID
    std::string filename;           //文件名
    Mod u_mode;                     //打开方式
};

//用户
struct usr{
    uint uid{};                     //用户ID
    Mod uright;                     //用户权限
    char usr_name[10]{};            //用户名
    char password[10]{};            //用户密码


    uint dir_pos{};                 //初始目录inode位置




    friend std::ostream&operator<<(std::ostream& out,usr& user){

        out << "uid:" << user.uid << std::endl;
        out << "user name:" << user.usr_name << std::endl;
        out << "password:" << user.password << std::endl;
        switch (user.uright){

            case Mod::___:std::cout << "___"; break;
            case Mod::__x:std::cout << "__x";break;
            case Mod::_w_:std::cout << "_w_";break;
            case Mod::_wx:std::cout << "_wx";break;
            case Mod::r__:std::cout << "r__";break;
            case Mod::r_x:std::cout << "r_x";break;
            case Mod::rw_:std::cout << "rw_";break;
            case Mod::rwx:std::cout << "rwx";break;
            default:std::cout << "wrong" << std::endl;
        }
        out << "directory pos:" << user.dir_pos << std::endl;
        return out;
    }


    usr(Mod uright,const std::string& uname,const std::string& psd){
        this->uright = uright;
        strcpy(usr_name,&uname[0]);
        strcpy(password,&psd[0]);
    }
    explicit usr(usr* usr1){
        strcpy(this->usr_name,usr1->usr_name) ;
        strcpy(this->password,usr1->password) ;
        this->uid = usr1->uid;
        this->dir_pos = usr1->dir_pos;
        this->uright = usr1->uright;
    }
};
// 目录表结构
struct cur_dir{
    std::string filename;
    inode* iNode;
};

// 用户权限表结构
struct usr_limit{
    uint file_id;
    Mod usr_mode;
};
 
class FileManager {
private:


    BlockManager* bm;                                       //磁盘管理器

    std::string dir;                                                //用户所在当前目录名字
    std::unordered_map<std::string, std::vector<cur_dir> > catalog; //总目录表
    std::unordered_map<int, std::vector<int> > file_link;           //显式链接表
    std::unordered_map<uint, std::vector<usr_limit> > limits;       //用户权限表
    std::unordered_map<std::string, uint> file_id;                  //文件名-文件id映射表

    std::unordered_map<int,int> user_sys;                   //系统打开表和用户打开表之间的映射关系
    std::unordered_map<int,sysopen> sys_open_table;         //系统打开表
    std::vector<usropen> user_open_table;                   //用户打开表


    usr* cur_usr;                                           //当前使用系统的用户
    std::unordered_map<std::string,dinode>* cur_catalog;    //当前目录表

    void table_init();                                      //系统表初始化
    void table_back();                                      //系统表写回

    ///基本完成，需要验证
    int checkMode(const std::string& filename, Mod mode);   //检测用户权限
    bool is_exist_usr(const std::string& usrname);          //验证用户名是否重复
    inode* createInode(int di_pos, dinode di);               // 创建内存i节点
    void updateLink(int fid, int pos);                      // 更新显式链接表
    void updateLimits(uint fid, Mod mod);                   // 更新用户权限表        
    void updateCatalog(std::string filename, inode* iNode); // 更新目录表
    void deleteLink(int fid);                               // 删除显式链接表中某一项的文件

    void vector_to_map(std::vector<cur_dir> v, std::unordered_map<std::string,dinode>* m); 
    void updateDir(const std::string& filename);                   // 更新当前目录表 filename-文件名
    void updateDir(inode* iNode);                           // iNode - 文件对应的i节点指针
    int inCurDir(const std::string& filename);                     // 判断文件是否在当前目录下
    int isOpen(const std::string& filename);                       // 判断当前用户是否已经打开某文件
    void deleteData(const std::string& filename);           // 在当前目录中删除对应数据项
    void deleteDir(const std::string& filename);            // 删除整个目录以及其中的文件
    void deleteId(const std::string& filename);             // 删除file-id表中的某一项
public:


    explicit FileManager(){
        bm = new BlockManager();
        cur_usr = nullptr;
        table_init();
    }
    ~FileManager(){
        delete(bm);
        table_back();
    }



    void del_file(const std::string& filename);                                              //删除文件
    void write_file(const std::string& filname,const std::string& val);                      //写文件

    ///未验证，但基本完成
    inode* create_file(const std::string& filename,dinode* info= nullptr);                   //创建文件
    dinode* create_file(const std::string& filename,bool index,dinode* info= nullptr,bool dir=false);
    inode* open_file(const std::string& filename, Mod mode);                                 //打开文件
    void close_file(std::string filename);                                                   //关闭文件
    std::string read_file(const std::string& filename);                                      //读文件

    
    ///接口
    uint getFid(const std::string& filename);                                                //获得文件名对应的id
    inode* find_last_dir(const std::string& dirname);                                        //根据文件名字在目录表中找到其上一层目录的i节点
    inode* find_dir(const std::string& dirname);                                             //根据文件名字在目录表中找到其对应i节点
    std::vector<int>* get_data_blocks(std::string filename);                                 //根据文件名获取其对应的数据块

    ///已完成
    bool verify_usr(const std::string& uname,const std::string& pwd);                       //验证用户信息
    int create_usr(const std::string& uname,const std::string& pwd,Mod right);              //创建用户
    void show_usr();


    void format();


};


#endif //FILESYSTEM_FILEMANAGER_H
