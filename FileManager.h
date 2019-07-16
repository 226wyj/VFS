//
// Created by wulinze on 19-6-27.
//

#ifndef MYFILESYSTEM_FILEMANAGER_H
#define MYFILESYSTEM_FILEMANAGER_H

#include "BlockManager.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <time.h>



#define uint unsigned int
#define SYSTBLSIZE 40       // 系统打开文件表大小
#define USRTBLSIZE 40       // 用户打开文件表大小
#define HASHSIZE 128        // 内存i节点hash表长度
#define NADDR 10            // 每个i节点最多指向10块


struct dinode;
struct inode;
struct Blockinfo;


enum class Mod:char{
    ___=0x00,__x=0x01,_w_=0x02,_wx=0x03,r__=0x04,r_x=0x05,rw_=0x06,rwx=0x07
};
enum class Type:char{
    dir=0x01,normal=0x02,pipe=0x03,link=0x04
};
enum class BlockType:char{
    index=0x00,data=0x01
};

struct Blockinfo{
    int index;

    BlockType type;

    Blockinfo(int data,BlockType type){
        this->index = data;
        this->type = type;
    }
    Blockinfo() = default;
};

//磁盘i节点
struct dinode{
    std::string filename;             //文件名
    Mod di_mode;		              //权限
    Type di_type;                     //类型
    uint di_uid{};			          //用户id
    uint di_size{};			          //文件大小


    Blockinfo di_first;               //文件对应首块


    time_t create_time{};             //创建时间
    time_t recent_open{};            //最近打开时间


    std::unordered_map<std::string,int> getCurCatalogue(BlockManager* bm,int curpos){
        if(this->di_type==Type::dir){
            std::unordered_map<std::string,int> res = getChildren(bm);
            res["../"] = curpos;
            return res;
        } else{
            throw std::runtime_error("invalid use of dir block");
        }
    }
    std::unordered_map<std::string,int> getChildren(BlockManager* bm){
        if(this->di_type == Type::dir){
            std::unordered_map<std::string,int> res;
            if(di_first.type == BlockType::data){
                int cur = 0;
                int *size = (int *)bm->ReadBlock(di_first.index,cur, sizeof(int));
                cur += sizeof(int);
                for(int i=0;i<*size;i++){
                    std::string filename = (char* )bm->ReadBlock(di_first.index,cur, 20);
                    cur += 20;
                    int *pos = (int *)bm->ReadBlock(di_first.index,cur, sizeof(int));
                    cur += sizeof(int);
                    res.insert(std::make_pair(filename,*pos));
                    free(&filename[0]);
                    free(pos);
                }
                free(size);
                return res;
            } else{
                std::vector<int> block_data;
                std::vector<int> block_index;
                block_index.emplace_back(di_first.index);
                while(!block_index.empty()){
                    int cur = 0;
                    int *size = (int *)bm->ReadBlock(block_index.front(),cur, sizeof(int));
                    cur += sizeof(int);
                    for(int i=0;i<*size-1;i++){
                        Blockinfo* cur_level;
                        cur_level = (Blockinfo*)bm->ReadBlock(block_index.front(),cur, sizeof(Blockinfo));
                        cur += sizeof(Blockinfo);
                        if(cur_level->type==BlockType::data){
                            block_data.emplace_back(cur_level->index);
                        } else{
                           block_index.emplace_back(cur_level->index);
                        }
                        free(cur_level);
                    }
                    block_index.erase(block_index.begin());
                }
                while (!block_data.empty()){

                    for(std::pair<std::string,int> item:getChildren(bm,block_data.front())){
                        res.insert(item);
                    }
                    block_data.erase(block_data.begin());
                }
                return res;
            }
        } else{
            throw std::runtime_error("invalid use of dir block");
        }

    }
    std::unordered_map<std::string,int> getChildren(BlockManager* bm,int pos){
        std::unordered_map<std::string,int> res;
        int cur = 0;
        int *size = (int *)bm->ReadBlock(di_first.index,cur, sizeof(int));
        cur += sizeof(int);
        for(int i=0;i<*size;i++){
            std::string filename = (char* )bm->ReadBlock(di_first.index,cur, 20);
            cur += 20;
            int *pos = (int *)bm->ReadBlock(di_first.index,cur, sizeof(int));
            cur += sizeof(int);
            res.insert(std::make_pair(filename,*pos));
            free(&filename[0]);
            free(pos);
        }
        return res;
    }
    std::unordered_map<int,int> getData(BlockManager* bm) {
        std::unordered_map<int, int> res;
        if (this->di_type == Type::dir) {
            if (di_first.type == BlockType::data) {
                int cur = 0;
                int *size = (int *) bm->ReadBlock(di_first.index, cur, sizeof(int));
                res.insert(std::make_pair(di_first.index,-1));
                free(size);
                return res;
            } else {
                std::vector<int> block_index;
                int cur_index;
                block_index.emplace_back(di_first.index);
                while(!block_index.empty()){
                    int cur = 0;
                    int *size = (int *)bm->ReadBlock(block_index.front(),cur, sizeof(int));
                    cur += sizeof(int);
                    for(int i=0;i<*size-1;i++){
                        Blockinfo* cur_level;
                        cur_level = (Blockinfo*)bm->ReadBlock(block_index.front(),cur, sizeof(Blockinfo));
                        cur += sizeof(Blockinfo);
                        if(cur_level->type==BlockType::data){
                            res.insert(std::make_pair(cur_level->index,block_index.front()));
                        } else{
                            block_index.emplace_back(cur_level->index);
                        }
                        free(cur_level);
                    }
                    block_index.erase(block_index.begin());
                }
                return res;
            }
        } else{
            throw std::runtime_error("invalid use of dir block");
        }
    }
    std::unordered_map<int,int> getIndex(BlockManager* bm){
        std::unordered_map<int, int> res;
        if (this->di_type == Type::dir) {
            if (di_first.type == BlockType::data) {
                return {};
            } else {
                std::vector<int> block_index;
                int cur_index;
                block_index.emplace_back(di_first.index);
                while(!block_index.empty()){
                    int cur = 0;
                    int *size = (int *)bm->ReadBlock(block_index.front(),cur, sizeof(int));
                    cur += sizeof(int);
                    for(int i=0;i<*size-1;i++){
                        Blockinfo* cur_level;
                        cur_level = (Blockinfo*)bm->ReadBlock(block_index.front(),cur, sizeof(Blockinfo));
                        cur += sizeof(Blockinfo);
                        if(cur_level->type==BlockType::index){
                            res.insert(std::make_pair(cur_level->index,block_index.front()));
                            block_index.emplace_back(cur_level->index);
                        }
                        free(cur_level);
                    }
                    block_index.erase(block_index.begin());
                }
                return res;
            }
        } else{
            throw std::runtime_error("invalid use of dir block");
        }
    }
    std::vector<int> getDataBlock(BlockManager* bm){

        if(di_first.type==BlockType::data){
            return {di_first.index};
        } else{
            std::vector<int> block_data;
            std::vector<int> block_index;
            block_index.emplace_back(di_first.index);
            while(!block_index.empty()){
                int cur = 0;
                int *size = (int *)bm->ReadBlock(block_index.front(),cur, sizeof(int));
                cur += sizeof(int);
                for(int i=0;i<*size-1;i++){
                    Blockinfo* cur_level;
                    cur_level = (Blockinfo*)bm->ReadBlock(block_index.front(),cur, sizeof(Blockinfo));
                    cur += sizeof(Blockinfo);
                    if(cur_level->type==BlockType::data){
                        block_data.emplace_back(cur_level->index);
                    } else{
                        block_index.emplace_back(cur_level->index);
                    }
                    free(cur_level);
                }
                block_index.erase(block_index.begin());
            }
            return block_data;
        }

    }
    std::vector<int> getAllBlock(BlockManager* bm){
        if(di_first.type==BlockType::data){
            return {di_first.index};
        } else{
            std::vector<int> block_data;
            std::vector<int> block_index;
            block_index.emplace_back(di_first.index);
            while(!block_index.empty()){
                int cur = 0;
                int *size = (int *)bm->ReadBlock(block_index.front(),cur, sizeof(int));
                cur += sizeof(int);
                for(int i=0;i<*size-1;i++){
                    Blockinfo* cur_level;
                    cur_level = (Blockinfo*)bm->ReadBlock(block_index.front(),cur, sizeof(Blockinfo));
                    cur += sizeof(Blockinfo);
                    if(cur_level->type==BlockType::data){
                        block_data.emplace_back(cur_level->index);
                    } else{
                        block_index.emplace_back(cur_level->index);
                        block_data.emplace_back(cur_level->index);
                    }
                    free(cur_level);
                }
                block_index.erase(block_index.begin());
            }
            return block_data;
        }
    }
    bool is_Full_index(BlockManager* bm,int i){
        int *size = (int*)bm->ReadBlock(i,0, sizeof(int));
        {
            free(size);
            return *size < 25;
        }
    }
    void add_block(BlockManager* bm,int pos,Blockinfo block){
        int *size = (int*)bm->ReadBlock(pos,0, sizeof(int));

        (*size)++;

        bm->WriteBlock(pos,0, sizeof(int),size);
        bm->WriteBlock(pos, sizeof(Blockinfo)*(*size)+ sizeof(int), sizeof(Blockinfo),&block);

        free(size);
    }
    bool resize(BlockManager* bm,std::unordered_map<std::string,int>* new_data){

        if(this->di_type == Type::dir){

            std::unordered_map<int,int> index_to_parent = this->getIndex(bm);
            std::unordered_map<int,int> data_to_parent = this->getData(bm);
            std::vector<int> datablock = getDataBlock(bm);
            int i = 0;
            int j = 0;

            while(i < new_data->size()){
                int *size = new int;
                if(j >= datablock.size()){
                    int data = bm->AllocateDataBlock();
                    datablock.emplace_back(data);
                    int parent = data_to_parent[datablock[j-1]];
                    while(is_Full_index(bm,parent)){
                        parent = index_to_parent[parent];
                    }
                    add_block(bm,parent,Blockinfo{data,BlockType ::data});
                    continue;
                }
                if(i>25){
                    *size = 25;
                    int cur = 0;
                    bm->WriteBlock(datablock[j],cur, sizeof(int),size);
                    cur += 4;
                    for(int m=0;m<*size;m++){
                        void * buf = malloc(20);
                        memcpy(buf,&new_data->begin(i)->first[0],new_data->begin(i)->first.length());
                        bm->WriteBlock(datablock[j],cur,20,buf);
                        cur += 20;
                        memcpy(buf,&new_data->begin(i)->second, sizeof(int));
                        bm->WriteBlock(datablock[j],cur, sizeof(int),buf);
                        free(buf);
                    }
                    i += *size;
                } else{
                    *size = i;
                    int cur = 0;
                    bm->WriteBlock(datablock[j],cur, sizeof(int),size);
                    cur += 4;
                    for(int m=0;m<*size;m++){
                        void * buf = malloc(20);
                        memcpy(buf,&new_data->begin(i)->first[0],new_data->begin(i)->first.length());
                        bm->WriteBlock(datablock[j],cur,20,buf);
                        cur += 20;
                        memcpy(buf,&new_data->begin(i)->second, sizeof(int));
                        bm->WriteBlock(datablock[j],cur, sizeof(int),buf);
                        free(buf);
                    }
                    i += *size;
                }
                j++;
            }
            if(j<datablock.size()){
                if(data_to_parent[datablock[j]] == data_to_parent[datablock[j-1]]){
                    bm->FreeDataBlock(datablock[j]);
                } else{
                    bm->FreeDataBlock(datablock[j]);
                    bm->FreeIndexBlock(data_to_parent[datablock[j]]);
                }
            }

            delete new_data;
            return true;
        } else{
            delete new_data;
            throw std::runtime_error("invalid use of dinode of dir");
        }

    }
    bool resize(BlockManager* bm,const std::string& val){

        if(this->di_type == Type::normal){
            std::unordered_map<int,int> index_to_parent = this->getIndex(bm);
            std::unordered_map<int,int> data_to_parent = this->getData(bm);
            std::vector<int> datablock = getDataBlock(bm);
            int i = 0;
            int j = 0;

            while (j<val.length()){
                if(i>=datablock.size()){
                    int data = bm->AllocateDataBlock();
                    datablock.emplace_back(data);
                    int parent = data_to_parent[datablock[i-1]];
                    while(is_Full_index(bm,parent)){
                        parent = index_to_parent[parent];
                    }
                    add_block(bm,parent,Blockinfo{data,BlockType ::data});
                    continue;
                }
                void *buf = malloc(512);
                j += (val.length() > 512) ? 512 : val.length();
                memcpy(buf, &val, j);
                bm->WriteBlock(datablock[i],buf);
                i++;
            }
            if(i<datablock.size()){
                if(data_to_parent[datablock[i]] == data_to_parent[datablock[i-1]]){
                    bm->FreeDataBlock(datablock[i]);
                } else{
                    bm->FreeDataBlock(datablock[i]);
                    bm->FreeIndexBlock(data_to_parent[datablock[i]]);
                }
            }

            return true;
        } else{
            throw std::runtime_error("invalid use of dinode of normal");
        }

    }

    explicit dinode(uint uid,const std::string& filename,
            int data,Mod mode,Type type=Type::dir,uint size=0):di_first(Blockinfo{data,BlockType ::data}){
        this->di_mode = mode;
        this->di_type = type;
        this->di_uid = uid;
        this->di_size = size;
        create_time = time(nullptr);
        recent_open = time(nullptr);
    }
    dinode() = default;


};

//内存i节点
struct inode{

    uint i_id;			            //内存i节点标识
    uint pos;                       //磁盘i节点块号


    dinode cur_node;


};
//用户打开表表项
struct usropen{
    uint file_id;
    Mod u_mode;                     //打开方式
};

//用户
struct usr{
    uint uid{};                     //用户ID
    Mod uright;                     //用户权限
    char usr_name[10]{};            //用户名
    char password[10]{};            //用户密码
    uint dir_pos{};                 //初始目录inode位置
    friend std::ostream&operator<<(std::ostream& out,usr& user){

        out << "uid:" << user.uid << std::endl;
        out << "user name:" << user.usr_name << std::endl;
        out << "password:" << user.password << std::endl;
        switch (user.uright){

            case Mod::___:std::cout << "___"; break;
            case Mod::__x:std::cout << "__x";break;
            case Mod::_w_:std::cout << "_w_";break;
            case Mod::_wx:std::cout << "_wx";break;
            case Mod::r__:std::cout << "r__";break;
            case Mod::r_x:std::cout << "r_x";break;
            case Mod::rw_:std::cout << "rw_";break;
            case Mod::rwx:std::cout << "rwx";break;
            default:std::cout << "wrong" << std::endl;
        }
        out << "directory pos:" << user.dir_pos << std::endl;
        return out;
    }


    usr(Mod uright,const std::string& uname,const std::string& psd){
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
class FileManager {
private:
    uint file_id;
    BlockManager* bm;
    //用户－系统打开表
    std::unordered_map<uint,uint> map_table;
    //用户打开表
    std::unordered_map<uint,std::unordered_map<std::string,usropen>> user_table;
    // fid, <引用计数,inode*>
    std::unordered_map<uint,std::pair<uint,inode*>> sys_table;
    //i节点和pos的映射
    std::unordered_map<inode*,uint> tool_table;
    // inode*野指针表
    std::vector<inode*> freeInode;
    // dinode野指针表
    std::vector<dinode*> freeDinoe;
    //当前目录(绝对路径)
    std::string cur_path;
    dinode* cur_dir_dinode;
    int cur_pos;
    //当前目录表<filename, di_pos>
    std::unordered_map<std::string,int>* cur_dir;
    //当前用户
    usr* cur_usr;
    void inode_copy(inode *iNode, dinode *diNode);
    void inode_copy(inode iNode, dinode diNode);
    int isUserOpen(const std::string& filename);
    void drop();
    bool is_exist_usr(const std::string& usrname);
public:
    explicit FileManager(){
        bm = new BlockManager();
        file_id = 0;
        cur_dir = nullptr;
        cur_usr = nullptr;
        cur_path = "";
        cur_dir_dinode = nullptr;
    }
    std::string ReadFile(dinode* file);
    void WriteFile(dinode* file,std::string val);
    void WriteFile(dinode* file,std::unordered_map<std::string,int>* dir);
    inode* CreateFile(const std::string& filename,Type filetype);
    void DeleteFile(inode* file);
    inode* OpenFile(dinode* file);
    void CloseFile(inode* file);
    void EnterDir(int dir_pos);
    void ChangeName(inode* iNode, std::string newname);
    // 验证用户信息
    bool verify_usr(const std::string& uname,const std::string& pwd);
    // 创建新用户
    int create_usr(const std::string& uname,const std::string& pwd,Mod right);
    void show_usr();
};


#endif //MYFILESYSTEM_FILEMANAGER_H
