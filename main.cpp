#include <iostream>
#include "FileManager.h"

using namespace std;


int main() {
    auto *fm = new FileManager();

    if(fm->create_usr("wu","980815",Mod::rwx) == -1){
        cout << "创建失败" << endl;
    } else{
        cout << "创建成功" << endl;
    }
    if(fm->verify_usr("wu","980815")){
        cout << "登录成功" << endl;
    } else{
        cout << "用户信息错误" << endl;
    }

//    fm->create_file("test.cpp");

//    fm->format();

    delete fm;

    return 0;
}