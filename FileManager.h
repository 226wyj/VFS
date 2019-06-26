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



#define uint unsigned int
#define SYSTBLSIZE 40       // 系统打开文件表大小
#define USRTBLSIZE 40       // 用户打开文件表大小
#define HASHSIZE 128        // 内存i节点hash表长度
#define NADDR 10            // 每个i节点最多指向10块



struct dinode;
struct inode;

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
    uint di_first;                  //文件首块

    time_t create_time;             //创建时间
    time_t rencent_open;            //最近打开时间

    explicit dinode(uint uid,int data){
        di_number = 0;
        di_mode = Mod ::r__;
        di_type = Type ::dir;
        di_uid = uid;
        di_size = 1;
        di_first = data;

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
    char filename[20];              //文件名
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



    usr(Mod uright,const std::string& uname,const std::string psd){
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

    std::unordered_map<int,int> user_sys;
    std::vector<sysopen> sys_open_table;
    std::vector<usropen> user_open_table;



    BlockManager* bm;

    usr* cur_usr;

    int sys_index;                                      // 系统打开文件表当前空闲位置
    int usr_index;                                      // 用户打开文件表当前空闲位置
    void readUsr();                                     // 初始化用户信息表
    int usrExists(char* name);                          // 判断用户名是否已经存在
public:
    FileManager();


    bool vertify_usr(const std::string& uname,const std::string& pwd);
    int create_usr(const std::string& uname,const std::string& pwd,Mod right);



    void signUp();                                      // 注册
    int open(char* filename, Mod mode);                 // 打开指定文件
    inode* ialloc();                                    // 磁盘i节点的分配
    void ifree(uint di_no);                             // 磁盘i节点的释放
    inode* iget(uint di_no);                            // 内存i节点的分配
    void iput(inode* di_no);                            // 内存i节点的释放
};


#endif //FILESYSTEM_FILEMANAGER_H
