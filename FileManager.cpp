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
    inode* iNode;

    // 更新当前目录
    updateDir(filename);

    // 未在当前目录找到该文件
    if(!inCurDir(filename)){
        cout << "想打开的文件不在当前目录" << endl;
        exit(0);
    }
    // 检查对应权限
    if(!checkMode(filename, mode)){
        cout << "您的权限不够" << endl;
        exit(0);
    }
    // 判断是否已经打开
    if(isOpen(filename)){
        cout << "您已经打开此文件"　<< endl;
        exit(0);
    }

    iNode = find_dir(filename);

    // 更新用户打开表
    usropen uOpen = {cur_usr->uid, filename, mode};
    user_open_table.push_back(uOpen);
    uint fid = getFid(filename);
    auto it_fopen = sys_open_table.find(fid);

    // 系统打开表中该文件已经存在则关联+1
    if(it_fopen != sys_open_table.end()){
        (it_fopen->second).s_count++;
    }

    // 不存在则创建新的系统打开表项
    else{
        sysopen sOpen = {1, fid, iNode};
        sys_open_table.insert(make_pair(fid, sOpen));
    }
    // 更新用户-系统打开表
    user_sys.insert(make_pair(cur_usr->uid, fid));
    
    return iNode;
}

// 关闭文件
void FileManager::close_file(std::string filename) {
    // 查找用户打开表，判断文件是否打开
    int flag = 0;
    auto i = user_open_table.begin();
    while(i != user_open_table.end()){
        if(i->u_uid == cur_usr->uid && i->filename == filename){
            flag = 1;
            break;
        }
        i++;
    }
    if(!flag){
        cout << "错误！当前文件并没有被打开" << endl;
        exit(0);
    }
    // 删除用户打开表中的对应表项
    user_open_table.erase(i);
    //　删除usr-sys表中对应的链接
    user_sys.erase(cur_usr->uid);
}

// 创建文件，默认创建目录文件.
inode *FileManager::create_file(const std::string& filename, dinode *info = nullptr) {

    // 该文件已经存在
    if(cur_catalog->find(filename) != cur_catalog->end()){
        cout << "该文件已经存在" << endl;
        exit(0);
        //return nullptr;
    }

    // 分配数据块
    int data_pos = bm->AllocateDataBlock();

    // 分配磁盘i节点块
    dinode dInode(cur_usr->uid, data_pos);
    int di_pos = bm->AllocateIndexBlock();              //分配磁盘i节点块,获得i节点编号
    dInode.di_number++;                                 // 关联数+1
    dInode.di_mode = info->di_mode;                     // 权限由参数给出
    dInode.di_type = info->di_type;                     // 类型由参数给出(数据 / 目录)
    dInode.create_time = time(nullptr);

    // 将更新后的i节点信息写回磁盘
    auto* temp_dinode = (dinode*)malloc(INODE_SIZE);
    memcpy(temp_dinode, &dInode, sizeof(dinode));
    bm->WriteIndexBlock(di_pos, (dinode*)temp_dinode);
    free(temp_dinode);

    // 构造对应的内存i节点
    inode* iNode = createInode(di_pos, dInode);

    // 更新文件名-文件id映射表
    file_id.insert(make_pair(filename, iNode.i_id));

    // 显式链接表新增表项
    updateLink(iNode->i_id, di_pos);

    // 用户权限表更新
    updateLimits(iNode->i_id, dInode.di_mode);

    // 更新目录
    updateCatalog(filename, &iNode);
    
    // 返回新建文件的内存i节点
    return iNode;
}

// 删除文件
void FileManager::del_file(const std::string& filename) {
    int fid = getFid(filename);
    if(file_link.find(fid) == file_link.end()){
        cout << "file_link表中不存在该文件" << endl;
        exit(0);
    }
    else{
        inode* iNode = find_dir(filename);
        // 删除普通数据文件
        if(iNode->cur_node.di_type == Type::normal){
            deleteData(filename);
        }
        // 删除目录文件
        else if(iNode->cur_node.di_type == Type::dir){
            
        }



        // 依次删除文件包含的所有数据块
        deleteLink(fid);
        inode* iNode = find_dir(filename);  // 在目录表中获取对应的内存i节点
        // 删除目录中的项
        // 如果该文件是目录文件，则递归删除
        if(iNode->cur_node.di_type == Type::dir){
            auto c_dir = catalog.find(filename);
            if(c_dir == catalog.end()){
                cout << "该目录不存在" << endl;
                exit(0);
            }
            del_file(c_dir->first);
        }
        // 数据文件，则删除单个文件即可
        else{
            
            return;
        }
    }
}

// 写文件
void FileManager::write_file(const std::string& filname, const std::string& val) {

}

// 读取文件
std::string FileManager::read_file(const std::string& filename) {
    int fid = getFid(filename);
    auto it = file_link.find(fid);
    string result = "";
    char* temp_str = "";
    if(it == file_link.end()){
        cout << "file_link表中不存在该文件" << endl;
        exit(0);
    }
    else{
        for(int i = 0; i < it->second.size(); i++){
            temp_str = bm->ReadBlock(it->second[i], i * BLOCK_SIZE, BLOCK_SIZE);
            result += temp_str;
        }
    }
    return result;
}





void FileManager::table_init() {

}

void FileManager::table_back() {

}


// 判断当前用户是否打开文件
int FileManager::isOpen(const std::string& filename){
  for(int i = 0; i < user_open_table.size(); i++){
      if(user_open_table[i].u_uid == cur_usr->uid && user_open_table[i].filename == filename){
          return 1;
      }
  }
  return 0;
}

// 判断文件是否在当前目录下
int FileManager::inCurDir(const std::string& filename){
    auto it = cur_catalog->find(filename);
    if(it == cur_catalog->end())
        return 0;
    return 1;
}

//　工具函数，将目录总表中的vector目录结构拷贝到当前目录的map结构中
void FileManager::vector_to_map(std::vector<cur_dir> v, std::unordered_map<std::string,dinode>* m){
    for(auto i : v){
        m->insert(make_pair(i.filename, *(i.iNode)));
    }
}

// 更新当前目录表与当前目录名字
void FileManager::updateDir(const std::string& filename){
    for(auto it = catalog.begin(); it != catalog.end(); it++){
        for(int i = 0; i < it->second.size(); i++){
            if(it->second[i].filename == filename){
                vector_to_map(it->second, cur_catalog);
                dir = it->first;
                return;
            }
        }
    }
}                          
// 重载
void FileManager::updateDir(inode* iNode){
    for(auto it = catalog.begin(); it != catalog.end(); it++){
        for(int i = 0; i < it->second.size(); i++){
            if(it->second[i].iNode == iNode){
                vector_to_map(it->second, cur_catalog);
                return;
            }
        }
    }
}

// 根据文件的名字在目录表中找到其对应的i节点
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
                return find_dir(last_name);
            }
        }
        it++;
    }
    return nullptr;
}

// 检测权限
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

// 判断用户是否存在
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

// 通过filename获取文件包含的所有数据块
std::vector<int>* FileManager::get_data_blocks(std::string filename) {
    uint fid = getFid(filename);
    auto it_file = file_link.find(fid);
    if(it_file == file_link.end()){
        cout << "该文件不在file_link表中" << endl;
        exit(0);
    }
    return &(it_file->second);
}

// 根据磁盘i节点生成对应的内存i节点
inode* FileManager::createInode(int di_pos, dinode di){
    inode* iNode;
    iNode->i_id = ID++;
    iNode->pos = di_pos;
    iNode->cur_node = di;
    return iNode;
}           

// 显式链接表新增表项
void FileManager::updateLink(int fid, int pos){
    vector<int> v;
    v.push_back(pos);
    auto iter = file_link.find(fid);
    if(iter == file_link.end()){
        file_link.insert(make_pair(fid, v));
    }
    else{
        iter->second.push_back(pos);
    }
}        

// 用户权限表更新
void FileManager::updateLimits(uint fid, Mod mod){
    usr_limit temp_limit = {fid, mod};
    vector<usr_limit> v_limit;
    v_limit.push_back(temp_limit);
    auto iter_limit = limits.find(cur_usr->uid);  // 查找用户是否在权限表中出现过
    if(iter_limit == limits.end()){
        limits.insert(make_pair(cur_usr->uid, v_limit));
    }
    else{
        limits[cur_usr->uid].push_back(temp_limit);
    }

}                                    

// 目录表更新(需要判断文件类型)
void FileManager::updateCatalog(std::string filename, inode* iNode){
    cur_dir temp_dir = {filename, iNode};
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
    v_dir.push_back(temp_cur_dir);
    v_dir.push_back(temp_las_dir);

    // 普通数据文件则在当前目录下追加
    if(iNode->cur_node.di_type == Type::normal){
        catalog[dir].push_back(temp_dir);
        updateDir(filename);
    }
    // 目录文件则新增目录总表项
    else if(iNode->cur_node.di_type == Type::dir){
        if(dir == filename){
            cout << "该目录已经存在" << endl;
            exit(0);
        }
        else{
            catalog.insert(make_pair(filename, v_dir));
        }
    }
}               

dinode *FileManager::create_file(const std::string &filename, bool index, dinode *info,bool isdir) {

    if(index){
        int inode_block,data_block;
        inode_block = bm->AllocateIndexBlock();
        data_block = bm->AllocateDataBlock();

        auto *new_dinode = new dinode(cur_usr->uid,data_block);
        void * buf = malloc(INODE_SIZE);
        memcpy(buf,new_dinode, sizeof(dinode));

        if(!isdir){
            new_dinode->di_type = Type ::normal;
        } else{
            bm->WriteIndexBlock(inode_block,buf);
            bm->WriteBlock(data_block,0, sizeof(int),&new_dinode->di_size);
        }

        free(buf);

        return new_dinode;

    } else{
        return nullptr;
    }


}

void FileManager::show_usr() {
    for(int i=0;i<8;i++){
        if(SuperBlcok.user_info[i] != -1){
            usr* temp_usr = (usr*)bm->ReadUser(i);

            cout << *temp_usr << endl;

            free(temp_usr);

        }
    }
}

// 根据fid删除对应的数据块，同时在链接表中删除其表项
void FileManager::deleteLink(int fid){
    auto it = file_link.find(fid);
    if(it == file_link.end()){
        cout << "链接表中找不到对应的fid" << endl;
        exit(0);
    }
    else{
        bm->FreeDataBlocks(it->second);
        file_link.erase(fid);
    }
}

// 删除file-id中的某一项
void FileManager::deleteId(const std::string& filename){
    for(auto i = file_id.begin(); i != file_id.end(); i++){
        if(i->first == filename){
            file_id.erase(i);
            return;
        }
    }
    cout << "file-id表中没有该文件项" << endl;
    exit(0);
}

// 在当前目录中删除对应数据项
void FileManager::deleteData(const string& filename){
    for(auto i = catalog[dir].begin(); i != catalog[dir].end(); i++){
        if(i->filename == filename){
            // 删除目录中数据
            catalog[dir].erase(it);
            // 更新当前目录
            updateDir(dir);
            // 删除链接表项
            deleteLink(getFid(filename));
            // 删除fid表项
            deleteId(filename);
            return;
        }
    }
    cout << "当前目录中不存在该文件" << endl;
    exit(0);
}

// 删除整个目录以及其中包括的文件
void FileManager::deleteDir(const std::string& filename){
    auto it = catalog.find(filename);
    if(it == catalog.end()){
        cout << "该目录不存在" << endl;
        exit(0);
    }
    // 遍历目录中的内容并依次删除
    for(auto v : it->second){
        deleteLink(getFid(v.filename));
        deleteId(v.filename);
    }
    // 删除目录项
    catalog.erase(it);
    deleteLink(getFid(filename));
    deleteId(filename);
    // 更新当前目录
    updateDir(dir);
}