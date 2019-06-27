//
// Created by wulinze on 19-6-19.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H


#include <string>
#include "FileManager.h"

using namespace std;



class FileSystem {
private:
    FileManager* fm;
    string current_path;
public:

    explicit FileSystem(){
        this->fm = new FileManager();
    }
    ~FileSystem(){
        delete(this->fm);
    }

//    bool login(string uname,string psd);     //用户登录
//    bool logout();                           //用户登出
    void cp();                               //复制指令
    void ls();                               //查看当前目录文件
    void mkdir(string dir_name);             //创建目录
    void cd();                               //移动当前目录
    void rm(string file_name,int i=0);               //删除文件
    void mv();                               //移动文件
    void touch(string file_name);            //创建文件
    void open(string file_name);             //打开文件
    bool mount(string path);                 //系统挂载目录


    void showUsers();
};


#endif //FILESYSTEM_FILESYSTEM_H
