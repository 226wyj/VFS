//
// Created by wulinze on 19-6-21.
//

#ifndef FILESYSTEM_FILEMANAGER_H
#define FILESYSTEM_FILEMANAGER_H

#include "BlockManager.h"
#include "FileTree.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>



#define uint unsigned int
#define SYSTBLSIZE 40       // 系统打开文件表大小
#define USRTBLSIZE 40       // 用户打开文件表大小
#define HASHSIZE 128        // 内存i节点hash表长度
#define NADDR 10            // 每个i节点最多指向10块



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
//磁盘i节点
struct dinode{
    uint di_number;                 //关联文件数，为0则删除该文件
    Mod di_mode;		            //权限
    Type di_type;                   //类型
    uint di_uid;			        //用户id
    uint di_size;			        //文件大小
    Blockinfo di_first            //文件对应首块

    time_t create_time;             //创建时间
    time_t rencent_open;            //最近打开时间

    explicit dinode(uint uid,int data){
        di_number = 0;
        di_mode = Mod ::r__;
        di_type = Type ::dir;
        di_uid = uid;
        di_size = 1;
        di_first.index = data;
        di_first.type = BlockType ::data;

        create_time = time(nullptr);
        rencent_open = time(nullptr);
    }

};

//内存i节点
struct inode{

    uint i_id;			            //内存i节点标识
    uint pos;                       //磁盘i节点块号
    uint offset;                    //磁盘i节点在块内的偏移量

    dinode cur_node;

};

//系统打开表表项
struct sysopen{
    uint s_count;                   //访问计数
    int fileid;	                    //文件描述符
    inode *s_inode;	                //指向对应内存i节点
};

//用户打开表表项
struct usropen{
    uint u_uid;			            //用户ID
    std::string filename;           //文件名
    Mod u_mode;                     //打开方式
    uint sys_pos;                   //对应的系统打开文件表入口
};

//用户
struct usr{
    uint uid{};                     //用户ID
    Mod uright;                     //用户权限
    char usr_name[10]{};            //用户名
    char password[10]{};            //用户密码


    uint dir_pos{};                 //初始目录inode位置



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
struct Blockinfo{
    int index;

    BlockType type;
};


class FileManager {
private:


    BlockManager* bm;                                   //磁盘管理器
    FileTree *catalogue_tree;                           //目录树



    std::unordered_map<int,int> user_sys;                           //系统打开表和用户打开表之间的映射关系
    std::unordered_map<int,sysopen> sys_open_table;                 //系统打开表
    std::unordered_map<int,std::vector<usropen>> user_open_table;   //用户打开表


    usr* cur_usr;                                           //当前使用系统的用户
    std::unordered_map<std::string,dinode>* cur_catalogue;  //当前目录表

    void table_init();                                  //系统表初始化
    void table_back();                                  //系统表写回

public:


    explicit FileManager(){
        bm = new BlockManager();
        cur_usr = nullptr;
        table_init();
    }
    ~FileManager(){
        table_back();
    }



    inode* open_file(const std::string& filename);                                           //打开文件
    void close_file(inode* file);                                                            //关闭文件
    inode* create_file(const std::string& val="",const std::string& filename,dinode* info);  //创建文件
    void del_file(const std::string& filename);                                              //删除文件
    void write_file(const std::string& filname,const std::string& val);                      //写文件
    std::string read_file(const std::string& filename);                                      //读文件

    bool vertify_usr(const std::string& uname,const std::string& pwd);                       //验证用户信息
    int create_usr(const std::string& uname,const std::string& pwd,Mod right);               //创建用户


};


#endif //FILESYSTEM_FILEMANAGER_H
