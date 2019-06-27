//
// Created by wulinze on 19-6-21.
//

#include "FileManager.h"
#include "BlockManager.h"
#include <cstring>



#define SuperBlcok this->bm->getSuperBlock()

using namespace std;
//#define DINODENUM BLOCK_SIZE / DINODE_SIZE  // 每块可以容纳的磁盘i节点数量



// 验证用户
bool FileManager::verify_usr(const std::string& uname, const std::string& pwd) {

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

// 创建用户
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

// 打开文件
inode *FileManager::open_file(const std::string& filename, Mod mode) {
    Mod fileMode;
    inode* finode;
    int flag = 0;
    // 在当前目录中搜索该文件
    for(int i = 0; i < catalog[filename].size(); i++){
        if((catalog[filename])[i].filename == filename){
            flag = 1;
            finode = (catalog[filename])[i].iNode;
            break;
        }
    }
    // 未在当前目录找到
    if(!flag){
        cout << "想打开的文件不在当前目录" << endl;
        exit(0);
    }
    // 检查对应权限
    flag = checkMode(filename, mode);
    if(!flag){
        cout << "您的权限不够" << endl;
        exit(0);
    }
    // 判断是否已经打开
    for(int i = 0; i < user_open_table.size(); i++){
        if(user_open_table[i].filename == filename){
            cout << "您已经打开该文件" <<endl;
            exit(0);
        }
    }
    usropen uOpen = {cur_usr->uid, filename, mode};

    // 更新用户打开表
    user_open_table.push_back(uOpen);
    uint fid = getFid(filename);
    auto it_fopen = sys_open_table.find(fid);

    // 系统打开表中该文件已经存在则关联+1
    if(it_fopen != sys_open_table.end()){
        (it_fopen->second).s_count++;
    }

    // 不存在则创建新的系统打开表项
    else{
        sysopen sOpen = {1, fid, find_dir(filename)};
        sys_open_table.insert(make_pair(fid, sOpen));
    }
    user_sys.insert(make_pair(cur_usr->uid, fid));

}

void FileManager::close_file(inode *file) {

}

// 创建文件
inode *FileManager::create_file(const std::string& filename, dinode *info) {



    if(this->cur_catalog->find(filename)!=cur_catalog->end()){
        return nullptr;
    }


    // 分配数据块
    int data_pos = bm->AllocateDataBlock();

    // 分配磁盘i节点块
    dinode dInode(cur_usr->uid, data_pos);
    int di_pos = bm->AllocateIndexBlock();              //分配磁盘i节点块,获得i节点编号
    dInode.di_number++;                                 // 关联数+1
    dInode.di_mode = info->di_mode;                     // 权限由参数给出
    dInode.create_time = time(nullptr);

    // 将更新后的信息写回磁盘
    auto* temp_dinode = (dinode*)malloc(INODE_SIZE);
    memcpy(temp_dinode, &dInode, sizeof(dinode));
    bm->WriteIndexBlock(di_pos, (dinode*)temp_dinode);
    free(temp_dinode);

    // 构造对应的内存i节点
    inode iNode;
    iNode.i_id = ID++;
    iNode.pos = di_pos;
//    iNode.offset = ;                                       // 填不出来，暂时丢弃
    iNode.cur_node = dInode;

    // 更新文件名-文件id映射表
    file_id.insert(make_pair(filename, iNode.i_id));

    // 显式链接表新增表项
    vector<int> v_link;
    v_link.push_back(data_pos);
    file_link.insert(make_pair(data_pos, v_link));

    // 用户权限表更新
    usr_limit temp_limit = {iNode.i_id, dInode.di_mode};
    vector<usr_limit> v_limit;
    v_limit.push_back(temp_limit);
    auto iter_limit = limits.find(cur_usr->uid);  // 查找用户是否在权限表中出现过
    if(iter_limit == limits.end()){
        limits.insert(make_pair(cur_usr->uid, v_limit));
    }
    else{
        limits[cur_usr->uid].push_back(temp_limit);
    }

    // 目录表新增表项
    cur_dir temp_dir = {filename, &iNode};
    inode* current_dir = find_dir(dir);
    if(current_dir == nullptr){
        cout << "无法获取当前目录!" << endl;
        exit(0);
    }
    inode* last_dir = find_last_dir(dir);
    if(last_dir == nullptr){
        cout << "无法获取上层目录!" << endl;
        exit(0);
    }
    cur_dir temp_cur_dir = {".", current_dir};
    cur_dir temp_las_dir = {"..", last_dir};
    vector<cur_dir> v_dir;
    v_dir.push_back(temp_dir);
    v_dir.push_back(temp_cur_dir);
    v_dir.push_back(temp_las_dir);
    auto iter_dir = catalog.find(dir);  // 在目录表中查找是否包含当前目录
    if(iter_dir == catalog.end()){
        catalog.insert(make_pair(dir, v_dir));
    }
    else{
        catalog[dir].push_back(temp_dir);
    }
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

inode* FileManager::find_dir(const std::string& dirname) {
    auto it = catalog.begin();
    while(it != catalog.end())
    {
        for(auto & i : it->second){
            if(i.filename == dirname){
                return i.iNode;
            }
        }
        it++;
    }
    return nullptr;
}

// 有问题，如果是根目录，则应该特殊处理。待加入
inode* FileManager::find_last_dir(const std::string& dirname) {
    string last_name;
    auto it = catalog.begin();
    while(it != catalog.end()){
        for(int i = 0; i < it->second.size(); i++){
            if(it->second[i].filename == dirname){
                last_name = it->first;
                goto found;
            }
        }
        it++;
    }
    return nullptr;
    found:
    find_dir(last_name);
}

int FileManager::checkMode(const std::string& filename, Mod mode) {
    auto iter_uid = limits.find(cur_usr->uid);
    auto iter_fid = file_id.find(filename);
    if(iter_fid == file_id.end()){
        cout << "所想操作的文件并不存在" << endl;
        exit(0);
    }
    uint fid = file_id[filename];   //  将文件名映射为文件id
    Mod um = Mod::___;
    if(iter_uid == limits.end()){
        um = Mod::r__;
    }
    else{
        for(auto & i : iter_uid->second){
            if(i.file_id == fid){
                um = i.usr_mode;
                break;
            }
        }
    }
    // 比较用户权限um和打开文件模式mode，允许访问则返回1，否则返回0
    int result;
    switch (um){
        case Mod::___:{
            if(mode == Mod::___)
                result = 1;
            result = 0;
            break;
        }
        case Mod::r__:{
            if(mode == Mod::r__)
                result = 1;
            result = 0;
            break;
        }
        case Mod::_w_:{
            if(mode == Mod::_w_)
                result = 1;
            result = 0;
            break;
        }
        case Mod::__x:{
            if(mode == Mod::__x)
                result = 1;
            result = 0;
            break;
        }
        case Mod::rw_:{
            if(mode == Mod::r__ || mode == Mod::_w_ || mode == Mod::rw_)
                result = 1;
            result = 0;
            break;
        }
        case Mod::_wx:{
            if(mode == Mod::_w_ || mode == Mod::__x || mode == Mod::_wx)
                result = 1;
            result = 0;
            break;
        }
        case Mod::r_x:{
            if(mode == Mod::r__ || mode == Mod::__x || mode == Mod::r_x)
                result = 1;
            result = 0;
            break;
        }
        case Mod::rwx:{
            result = 1;
            break;
        }
    }
    return result;
}

// 通过filename获取fid
uint FileManager::getFid(const std::string& filename) {
    auto iter = file_id.find(filename);
    if(iter == file_id.end()){
        cout << "该文件不在file_id表中" << endl;
        exit(0);
    }
    return file_id[filename];
}

void FileManager::format() {
    bm->Format(true);
}

bool FileManager::is_exist_usr(const string& usrname) {
    if(usrname.length() > 10)return false;
    else{

        for(int i=0;i<8;i++){
            if(SuperBlcok.user_info[i] != -1){
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
