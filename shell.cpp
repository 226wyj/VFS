//
// Created by lihang on 19-6-27.
//

#include "shell.h"
#include <sstream>
#include<iostream>
#include<string>
#include<vector>
#include"FileSystem.h"
using namespace std;
FileSystem f;
string shell::cut(int i, string mainstring) {
    string substring;
    istringstream istr(mainstring);
    for (int j = 0; j < i; j++)
        if (!istr.eof())
            istr >> substring;

    return substring;
}

void shell::display()
{
    cout<<"/************welcome to the system of file***********/"<<endl;
    cout<<"root$";
    getline(cin,input);
    cmd=cut(1,input);
    if((cmd=="mkdir")||(cmd=="cd")||(cmd=="rm")||(cmd=="touch")||(cmd=="ls")||(cmd=="help"))
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
        else if(cmd=="help")
        {
            help();
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
    int i,j;
    string a,b;
    i=count(file1);
    j=count(file2);
    a=cut_1(i,file1);
    b=cut_1(i,file2);
    if(f.cd(a)&&f.cd(b))
    f.cp(a,b);
    else
        cout<<"error"<<endl;
}
void shell::ls()
{
    f.ls();
}
bool shell::mkdir(string dir_name)
{
    int i,j;
    string a;
    i=count(dir_name);
    a=cut_1(i,dir_name);
    f.cd(dir_name.substr(0,dir_name.size()-a.size()));
    f.mkdir(a);
}
bool shell::cd(string name)
{
    int num;
    string a;
    int i;
    num=count(name);
    for(i=1;i<=num;i++)
    {
        a=cut_1(i,name);
        if(f.cd(a))
            f.cd(a);
        else
        break;
    }
    if(i!=num) {
        int num1;
        num1 = count(name);
        for (int i = 1; i <= num - 1; i++) {
            a=cut_1(i,name);
            f.cd(a);
        }
    }
}

bool shell::rm(string file_name)
{
    int i,j;
    string a;
    i=count(file_name);
    a=cut_1(i,file_name);
    f.cd(file_name.substr(0,file_name.size()-a.size()));
    f.rm(a);
}
bool shell::touch(string file_name)
{
    int i,j;
    string a;
    i=count(file_name);
    a=cut_1(i,file_name);
    f.cd(file_name.substr(0,file_name.size()-a.size()));
    if(f.touch(a))
    {
        cout<<"touch the file " << a<< "successful"<<endl;

    }
    else
      cout<<"cannot touch the file"<<endl;
}
bool shell::open(string file_name,string mood)
{
    int i,j;
    string a;
    i=count(file_name);
    a=cut_1(i,file_name);
    f.cd(file_name.substr(0,file_name.size()-a.size()));
    if(f.open(a,mood))
    {
        cout<<"打开文件"<<a<<endl;

    }
    else
        cout<<"cannot open the file"<<endl;

}
string shell::cut_1(int i,string mainstring)
{
    stringstream ss(mainstring);
    string sub_string;
    for(int j=0;j<i;j++)
    {
        getline(ss,sub_string,'/');
    }
    return sub_string;
}
int shell::count(string mainstring)
{
    int i,j;
    string sub_str1;
    string sub_str2;
    for(int i=1;i<20;i++)
    {
        j=i;
        sub_str1=cut_1(i,mainstring);
        sub_str2=cut_1(i+1,mainstring);
        if(sub_str1==sub_str2)
            break;
        else
            continue;
    }
    return j;
}

vector <string> shell::cun(string a)
{
    vector <string> b;
    int j;
    int i=count(a);
    for(int j=1;j<=i;j++)
    {
        b.push_back(cut_1(i,a));
    }
    return b;
}
void shell::help()
{
    cout<<"ls          显示文件列表"<<endl;
    cout<<"cd          移动当前目录"<<endl;
    cout<<"mkdir       创建目录"<<endl;
    cout<<"rm          删除文件"<<endl;
    cout<<"touch       创建文件"<<endl;
    cout<<"open        打开文件"<<endl;
    cout<<"cp          复制文件"<<endl;

}
