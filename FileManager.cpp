#include <utility>

#include "FileManager.h"
#include <iostream>
using namespace std;

// 通过dinode构造inode
void FileManager::inode_copy(inode *iNode, dinode *diNode) {
    iNode->cur_node.filename = diNode->filename;
    iNode->cur_node.di_mode = diNode->di_mode;
    iNode->cur_node.create_time = diNode->create_time;
    iNode->cur_node.di_first = diNode->di_first;
    iNode->cur_node.di_size = diNode->di_size;
    iNode->cur_node.di_uid = diNode->di_uid;
    iNode->cur_node.di_type = diNode->di_type;
    iNode->cur_node.recent_open = time(0);
}

void FileManager::inode_copy(inode iNode, dinode diNode) {
    iNode.cur_node.filename = diNode.filename;
    iNode.cur_node.di_mode = diNode.di_mode;
    iNode.cur_node.create_time = diNode.create_time;
    iNode.cur_node.di_first = diNode.di_first;
    iNode.cur_node.di_size = diNode.di_size;
    iNode.cur_node.di_uid = diNode.di_uid;
    iNode.cur_node.di_type = diNode.di_type;
    iNode.cur_node.recent_open = time(0);
}

// 判断文件是否被当前用户打开
int FileManager::isUserOpen(const std::string& filename){
    if(user_table[cur_usr->uid].count(filename) != 0){
        return 1;
    }
    return 0;
}

//　释放野指针
void FileManager::drop(){
    for(auto v1 : freeInode){
        free(v1);
    }
    for(auto v2 : freeDinoe){
        free(v2);
    }
}

// open file
inode* FileManager::OpenFile(dinode* file){
    drop();
    if(cur_dir->find(file->filename) == cur_dir->end()){
        return nullptr;
    }
    if(isUserOpen(file->filename)){
        return nullptr;
    }
    inode* iNode = nullptr;
    inode_copy(iNode, file);
    freeInode.push_back(iNode);
    
    //更新用户打开表
    usropen u = {file_id, file->di_mode};
    if(user_table.find(cur_usr->uid) != user_table.end()){
        unordered_map<string, usropen> temp_map;
        user_table[cur_usr->uid] = temp_map;
    }
    user_table[cur_usr->uid][file->filename] = u;

    // 更新系统打开表
    if(sys_table.find(file_id) != sys_table.end()){
        sys_table[file_id].first++;
    }
    else{
        pair<uint,inode*> temp_pair = {1, iNode};
        sys_table[cur_usr->uid] = temp_pair;
    }

    // 更新map_table
    map_table[cur_usr->uid] = file_id;
    return iNode;
}


// 关闭文件
void FileManager::CloseFile(inode* file){
    drop();
    if(!isUserOpen(file->cur_node.filename)){
        return;
    }
    // 删除用户打开表项
    unordered_map<std::string,usropen> u = user_table[cur_usr->uid];
    for(auto it = u.begin(); it != u.end(); it++){
        if(it->first == file->cur_node.filename){
            u.erase(it);
            user_table[cur_usr->uid] = u;
            break;
        }
    }
    // 系统打开表项引用计数-1
    if(--sys_table[file_id].first == 0){
        pair<uint,inode*> s = sys_table[file_id];
        sys_table.erase(file_id);
    }

    // 删除map_table链接
    for(auto i = map_table.begin(); i != map_table.end(); i++){
        if(i->first == cur_usr->uid && i->second == file_id){
            map_table.erase(i);
            break;
        }
    }

}

// create file
inode* FileManager::CreateFile(const std::string& filename,Type filetype){
    drop();
    if((*cur_dir).find(filename) != (*cur_dir).end())
        return nullptr;
    if(filetype == Type::dir && filename == "/")
        return nullptr;
    //  构造inode
    int data_pos = bm->AllocateDataBlock();
    int dinode_pos = bm->AllocateIndexBlock();
    dinode dInode(cur_usr->uid, filename, data_pos, cur_usr->uright, filetype);
    dinode* p = &dInode;
    freeDinoe.push_back(p);
    inode iNode = {(uint)file_id++, (uint)dinode_pos, dInode};
    inode* pInode = &iNode;
    freeInode.push_back(pInode);
    // 修改当前目录
    (*cur_dir)[filename] = dinode_pos;

    // 写回
    bm->WriteIndexBlock(dinode_pos, (dinode*)p);

    return pInode;
}

// delete file
void FileManager::DeleteFile(inode* file){
    if((*cur_dir).find(file->cur_node.filename) == (*cur_dir).end()){
        cout << "文件不存在　" << endl;
        return;
    }
    if(isUserOpen(file->cur_node.filename)){
        CloseFile(file);
    }
    int di_pos = (*cur_dir)[file->cur_node.filename];
    (*cur_dir).erase(file->cur_node.filename);

    bm->FreeIndexBlock(di_pos);
    bm->FreeDataBlocks(file->cur_node.getAllBlock(bm));

}

// read file
std::string FileManager::ReadFile(dinode* file){
    inode* iNode = OpenFile(file);
    vector<int> blocks = file->getDataBlock(bm);
    string res;
    for(int block : blocks){
        char* tmp = (char*)malloc(BLOCK_SIZE);
        tmp = (char*)bm->ReadBlock(block);
        res += tmp;
        free(tmp);
    }
    return res;
}

// write file
void FileManager::WriteFile(dinode* file,std::string val){
    if(file->di_type == Type::normal){
        file->resize(bm, val);
    }
    else{
        throw runtime_error("文件类型错误");
    }
}

void FileManager::WriteFile(dinode* file,std::unordered_map<std::string,int>* dir){
    if(file->di_type == Type::dir){
        file->resize(bm, dir);
    }
    else{
        throw runtime_error("文件类型错误");
    }
}

void FileManager::EnterDir(int dir_pos) {
    auto* d = (dinode*)bm->ReadIndexBlock(dir_pos);
    for(auto & i : (*cur_dir)){
        if(i.second == dir_pos){
            if(i.first == ".."){
                int index = cur_path.find_last_of('/');
                cur_path =cur_path.substr(0, index);
            }
            else{
                cur_path += ("/" + i.first);
            }
        }
    }
    *cur_dir = d->getCurCatalogue(bm, cur_pos);
    cur_pos = dir_pos;
}

void FileManager::ChangeName(inode* iNode, std::string newname) {
    if((*cur_dir).find(iNode->cur_node.filename) == (*cur_dir).end()){
        throw runtime_error("当前目录不存在该文件");
    }
    iNode->cur_node.filename = std::move(newname);
    bm->WriteIndexBlock(iNode->pos, iNode);
}
bool FileManager::is_exist_usr(const string& usrname) {
    if(usrname.length() > 10)return false;
    else{

        for(int i=0;i<8;i++){
            if(bm->getSuperBlock().user_info[i] != -1){
                usr* temp_usr = (usr*)bm->ReadUser(i);

                if(temp_usr!= nullptr){
                    if(strcmp(temp_usr[0].usr_name,&usrname[0])==0){
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
bool FileManager::verify_usr(const std::string& uname, const std::string& pwd) {

    if(uname.length() > 10 || pwd.length() > 10)return false;
    else{

        for(int i=0;i<8;i++){
            if(bm->getSuperBlock().user_info[i] != -1){
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

    if(uname.length() > 10 || pwd.length()>10 || is_exist_usr(uname))
        return -1;

    usr* new_usr = new usr(right,uname,pwd);
    new_usr->uid = bm->AllocateUser();

    if(new_usr->uid!=-1){
        new_usr->dir_pos = bm->AllocateIndexBlock();
        int file_pos = bm->AllocateDataBlock();

        auto* new_dinode = new dinode(new_usr->uid, file_pos);

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
void FileManager::show_usr() {
    for(int i=0;i<8;i++){
        if(bm->getSuperBlock().user_info[i] != -1){
            usr* temp_usr = (usr*)bm->ReadUser(i);

            cout << *temp_usr << endl;

            free(temp_usr);

        }
    }
}









