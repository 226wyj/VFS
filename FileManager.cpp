//
// Created by wulinze on 19-6-21.
//

#include "FileManager.h"
#include "BlockManager.h"
#include <cstring>



#define SuperBlcok this->bm->getSuperBlock()

using namespace std;
#define DINODENUM BLOCK_SIZE / DINODE_SIZE  // 每块可以容纳的磁盘i节点数量




bool FileManager::vertify_usr(const std::string& uname, const std::string& pwd) {

    if(uname.length() > 10 || pwd.length() > 10)return false;
    else{

        for(int i=0;i<8;i++){
            if(SuperBlcok.user_info[i] != -1){
                usr* temp_usr = (usr*)bm->ReadUser(i);

                if(temp_usr!= nullptr){
                    if(strcmp(temp_usr[0].usr_name,&uname[0]) == 0 &&
                       strcmp(temp_usr[0].password,&pwd[0]) == 0){

                        cur_usr = new usr(temp_usr);

                        return true;
                    }
                    free(temp_usr);
                } else{
                    return false;
                }

            }
        }

        return false;

    }

}

int FileManager::create_usr(const std::string &uname, const std::string &pwd, Mod right) {

    if(uname.length() > 10 || pwd.length()>10)
        return -1;

    usr* new_usr = new usr(right,uname,pwd);
    new_usr->uid = bm->AllocateUser();

    if(new_usr->uid!=-1){
        new_usr->dir_pos = bm->AllocateIndexBlock();
        int file_pos = bm->AllocateDataBlock();

        auto* new_dinode = new dinode(new_usr->uid,file_pos);

        void* buf = malloc(INODE_SIZE);
        void* user_buf = malloc(USER_SIZE);

        memcpy(buf,new_dinode, sizeof(dinode));
        memcpy(user_buf,new_usr, sizeof(usr));

        usleep(100);

        bm->WriteIndexBlock(new_usr->dir_pos,buf);
        bm->WriteUser(new_usr->uid,user_buf);

        free(buf);
        free(user_buf);

        delete new_dinode;

        return new_usr->uid;
    }

    delete(new_usr);

    return -1;
}

inode *FileManager::open_file(const std::string& filename) {
    return nullptr;
}

void FileManager::close_file(inode *file) {

}

inode *FileManager::create_file(const string& val, const std::string& filename, dinode *info) {
    return nullptr;
}

void FileManager::del_file(const std::string& filename) {

}

void FileManager::write_file(const std::string& filname, const std::string& val) {

}

std::string FileManager::read_file(const std::string& filename) {

}





void FileManager::table_init() {

}

void FileManager::table_back() {

}