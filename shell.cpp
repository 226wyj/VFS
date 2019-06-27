//
// Created by lihang on 19-6-27.
//

#include "shell.h"
#include <sstream>
#include<iostream>
#include<cstring>
using namespace std;

string shell::cut(int i, string mainstring) {
    string substring;
    istringstream istr(mainstring);
    for (int j = 0; j < i; j++) {
        if (!istr.eof())
            istr >> substring;
    }
    return substring;
}

void shell::display()
{
    cout<<"/************welcome to the system of file***********/"<<endl;
    cout<<"root$";
    getline(cin,input);
    cmd=cut(1,input);
    if((cmd=="mkdir")||(cmd=="cd")||(cmd=="rm")||(cmd=="touch")||(cmd=="ls"))
    {
        file1=cut(2,input);
        if(cmd=="mkdir")
        {
            mkdir(file1);
        }
        else if(cmd=="cd")
        {
            cd(file1);
        }
        else if(cmd=="rm")
        {
            rm(file1);
        }
        else if(cmd=="touch")
        {
            touch(file1);
        }
        else if(cmd=="ls")
        {
            ls();
        }
        else
        {
            cout<<"error"<<endl;
        }
    }
    else if((cmd=="open")||(cmd=="cp"))
    {
        file1=cut(2,input);
        file2=cut(3,input);
        if(file1==file2)
        {
            cout<<"error"<<endl;
        }
        else {
            if (cmd == "open") {
                open(file1, file2);
            } else if (cmd == "cp") {
                cp(file1, file2);
            } else cout << "error" << endl;
        }
    }
    else cout<<"error"<<endl;

}

bool shell::cp(string file1,string file2)
{
    cout<<"复制文件"<<file1<<"到"<<file2<<"成功"<<endl;
}
void shell::ls()
{
    cout<<"list"<<endl;
}
bool shell::mkdir(string dir_name)
{
    cout<<"创建目录"<<dir_name<<"成功"<<endl;
}
bool shell::cd(string name)
{
    cout<<"移动"<<name<<"成功"<<endl;
}
bool shell::rm(string file_name)
{
    cout<<"删除文件"<<file_name<<" 成功"<<endl;
}
bool shell::touch(string file_name)
{
    cout<<"创建文件"<<file_name<<"成功"<<endl;
}
bool shell::open(string file_name,string mood)
{
    cout<<"以"<<mood<<"打开文件"<<file_name<<"成功"<<endl;
}

