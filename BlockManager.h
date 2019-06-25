//
// Created by wulinze on 19-6-19.
//

#ifndef FILESYSTEM_BLOCKMANAGER_H
#define FILESYSTEM_BLOCKMANAGER_H

#define BLOCK_SIZE 512              //块大小
#define MAX_BLOCK 1024              //磁盘大小
#define IndexSize 128               //索引区大小
#define USER_AREAR_SIZE 1           //用户区域大小
#define ValidSize 50                //数据组内块数
#define ValidIndexSize 15           //索引组内
#define ValidIndexSize 15           //索引组内
#define SUPER_POS 0                 //超级块位置
#define INODE_SIZE 64


#include <vector>
#include <string>
#include <unordered_map>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <sys/stat.h>



struct Super_Block{

    int ValidData[ValidSize+1];                                 //空闲数据栈 4*51bytes
    int ValidIndex[ValidIndexSize+1];                           //空闲索引栈 64bytes


    int free_data;                                              //空闲数据块数量 4bytes
    int free_index;                                             //空闲iNode数量 4bytes

    int user_begin;                                             //系统用户区域    4bytes
    int index_begin;                                            //索引区域起始位置 4bytes
    int data_begin;                                             //数据区域起始位置 4bytes


    int index_size;                                             //索引区大小 4bytes
    int user_size;                                              //用户区大小 4bytes


    int user_count;                                             //用户数量  4bytes
    int iNode_count;                                            //iNode数量 4bytes

    //格式化超级块
    void Super_Block_Format(bool user= false,int new_user_size=-1,int new_index_size=-1){//重新分配区间大小



        free_index = index_size*BLOCK_SIZE/INODE_SIZE;
        free_data = MAX_BLOCK-data_begin;
        iNode_count = 0;

        if(user)user_count = 0;


        if(new_user_size>0){
            user_size = new_user_size;
        }
        if(new_index_size >0){
            index_size = new_index_size;
            index_begin = user_begin + user_size;
            data_begin = index_begin + index_size;
        }

    }
    //初始化超级块
    void Super_Block_Init(FILE* disk,bool first=false){
        if(!first && fseek(disk,SUPER_POS,SEEK_SET)==0){
            //栈初始化
            fread(ValidData, sizeof(int),ValidSize+1,disk);
            fread(ValidIndex, sizeof(int),ValidIndexSize+1,disk);
            //起始位置初始化
            fread(&user_begin, sizeof(int),1,disk);
            fread(&index_begin, sizeof(int),1,disk);
            fread(&data_begin, sizeof(int),1,disk);
            //区域大小初始化
            fread(&index_size, sizeof(int),1,disk);
            fread(&user_size, sizeof(int),1,disk);
            //数量初始化
            fread(&user_count, sizeof(int),1,disk);
            fread(&iNode_count, sizeof(int),1,disk);
            fread(&free_index, sizeof(int),1,disk);
            fread(&free_data, sizeof(int),1,disk);
        } else {
            //初始化栈
            memset(ValidData,0,ValidSize+1* sizeof(int));
            memset(ValidIndex,0,ValidIndexSize+1* sizeof(int));
            //初始化起始位置
            user_begin = SUPER_POS+1;
            index_begin = user_begin + USER_AREAR_SIZE;
            data_begin = index_begin + IndexSize;
            //初始化区域大小
            index_size = IndexSize;
            user_size = 1;
            //初始化当前数量
            user_count = iNode_count = 0;
            free_index = index_size*BLOCK_SIZE/INODE_SIZE;
            free_data = MAX_BLOCK-data_begin;
        }
    }
    //写回超级块
    void Super_Block_Back(FILE* disk){
        if(fseek(disk,SUPER_POS,SEEK_SET)==0){
            //栈写回
            fwrite(ValidData, sizeof(int),ValidSize+1,disk);
            fwrite(ValidIndex, sizeof(int),ValidIndexSize+1,disk);
            //起始位置写回
            fwrite(&user_begin, sizeof(int),1,disk);
            fwrite(&index_begin, sizeof(int),1,disk);
            fwrite(&data_begin, sizeof(int),1,disk);
            //区域大小写回
            fwrite(&index_size, sizeof(int),1,disk);
            fwrite(&user_size, sizeof(int),1,disk);
            //数量写回
            fwrite(&user_count, sizeof(int),1,disk);
            fwrite(&iNode_count, sizeof(int),1,disk);
            fwrite(&free_index, sizeof(int),1,disk);
            fwrite(&free_data, sizeof(int),1,disk);
            std::cout << "超级块写回成功" << std::endl;
        } else {
            throw std::runtime_error("超级块写回错误");
        }
    }

    friend std::ostream&operator<<(std::ostream& out,Super_Block& b){
        out << "index depth:" << b.ValidIndex[0] << std::endl << "index data:" << std::endl;
        for(int i=1;i<ValidIndexSize+1;i++){
            if(i%10==0)std::cout << std::endl;
            out << "index[" << i << "]=" << b.ValidIndex[i] << ' ';
        }
        std::cout << std::endl;
        out << std::endl << "file depth:" << b.ValidData[0] << std::endl << "file data:" << std::endl;
        for(int i=1;i<ValidSize+1;i++){
            if(i%10==0)std::cout << std::endl;
            out << "file[" << i << "]=" << b.ValidData[i] << ' ';
        }
        std::cout << std::endl;
        out << std::endl <<"free data:" << b.free_data << std::endl << "free index:" << b.free_index << std::endl;
        out << "user begin:" << b.user_begin << std::endl << "index begin:" << b.index_begin << std::endl;
        out << "data begin:" << b.data_begin << std::endl;
        out << "index size:" << b.index_size << std::endl << "user_size:" << b.user_size << std::endl;
        out << "user count:" << b.user_count << std::endl << "iNode count:" << b.iNode_count << std::endl;

        return out;
    }

};


class BlockManager {
private:
    Super_Block superBlock{};
    char* filename{};
    FILE* disk;

    void FirstData(int max,int size);                           //对于第一个结构的特殊分配
    void InitDataGroup(int& cur,int left);                      //初始化组块
    void CopyDataStack(int pos);                                //数据栈复制至磁盘块
    void CopyIndexStack(int pos);                               //索引栈复制至磁盘块
    void IndexStackCopy(int pos);                               //磁盘块复制至索引栈
    void DataStackCopy(int pos);                                //磁盘块复制至数据栈
    void InitDisk(int size=MAX_BLOCK);                          //初始化磁盘大小
    void FirstIndex(int size);                                  //初始化第一个索引块
    void InitIndexGroup(int& cur);                              //初始化索引组块
public:

    //构造函数和析构函数
    explicit BlockManager(const std::string& filename = "../disk"){
        this->filename = (char*)malloc(filename.length());
        strcpy(this->filename , &filename[0]);
        if(access(this->filename,F_OK)!=-1){
            if((disk = fopen(this->filename,"rb+"))!= nullptr)
                superBlock.Super_Block_Init(disk);
            else {
                perror("file error");
                throw std::runtime_error("文件打开失败");
            }
        } else{
            if((disk = fopen(this->filename,"wb+"))!=nullptr){
                int fd = fileno(disk);

                fchmod(fd, 0666);

                superBlock.Super_Block_Init(disk,true);

                InitDisk();

                Format(true);
            } else {
                perror("file error");
                throw std::runtime_error("文件打开失败");
            }
        }
    }
    ~BlockManager(){
        superBlock.Super_Block_Back(disk);

//        superBlock.Super_Block_Init(disk);

//        std::cout << superBlock;
        fclose(disk);
    }

    void Format(bool type= false);                              //格式化文件 type规定是否删除用户信息

    std::vector<int> AllocateDataBlocks(int count);             //分配多个数据块
    std::vector<int> AllocateIndexBlcoks(int count);            //分配多个索引块
    int AllocateDataBlock();                                    //分配一个数据块
    int AllocateIndexBlock();                                   //分配一个索引块
    void FreeDataBlock(int pos);                                //释放一个数据块
    void FreeIndexBlock(int pos);                               //释放一个索引块
    void FreeDataBlocks(const std::vector<int>& blockseq);      //释放多个数据块
    void FreeIndexBlock(const std::vector<int>& blockseq);      //释放多个索引块
    void BlockCopy(int source,int des);                         //块复制操作
    void BlockExchange(int source,int des);                     //块交换操作
    void InodeCopy(int source,int des);                         //inode复制
    void InodeExchanger(int source,int des);                    //inode交换



    void WriteBlock(int pos,void* buffer);                      //写入整块磁盘
    void WriteBlock(int pos,int begin,int size,void* buffer);   //写磁盘块 pos:磁盘块位置 begin:起始位置 end:结束位置 buffer要写的数据
    void* ReadBlock(int pos,int begin,int size);                //读磁盘块 pos:磁盘块位置 begin:起始位置 size大小 返回读取数据
    void* ReadBlock(int pos);                                   //读取整块磁盘

    void WriteIndexBlock(int pos,void* buffer);                 //写入索引“块”
    void* ReadIndexBlock(int pos);                              //读取索引“块”




    Super_Block& getSuperBlock(){
        return superBlock;
    }

    void CreateiNode(){
        this->superBlock.iNode_count++;
    }
    void CreateUser(){
        this->superBlock.user_count++;
    }

    int getiNodeStart(){
        return superBlock.index_begin;
    }


};


#endif //FILESYSTEM_BLOCKMANAGER_H
