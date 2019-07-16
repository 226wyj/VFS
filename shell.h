//
// Created by lihang on 19-6-27.
//

#ifndef TESTING_SHELL_H
#define TESTING_SHELL_H

#include <string>
#include<vector>
using namespace std;
class shell {
private:
    string input;
    string cmd;
    string file1;
    string file2;
public:
    string cut(int i,string mainstring);
    vector <string> cun(string a);
    void cd__(string name);
    string cut_1(int i,string mainstring);
    int count(string mainstring);
    void display();
    void help();
    bool cp(string file1,string file2);                               //复制指令
    void ls();                               //查看当前目录文件
    bool mkdir(string dir_name);             //创建目录
    bool cd(string name);                               //移动当前目录
    bool rm(string file_name);               //删除文件
    bool touch(string file_name);            //创建文件
    bool open(string file_name,string mood);             //打开文

};


#endif //TESTING_SHELL_H
