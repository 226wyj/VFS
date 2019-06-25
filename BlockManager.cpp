//
// Created by wulinze on 19-6-19.
//

#include "BlockManager.h"
#define UserArea this->superBlock.user_begin
#define IndexArea this->superBlock.index_begin
#define DataArea this->superBlock.data_begin
#define ValidSize 50
#define ValidDataDepth this->superBlock.ValidData[0]        //数据栈深度
#define ValidIndexDepth this->superBlock.ValidIndex[0]      //索引栈深度
#define BLOCK_SIZE 512                                      //块大小
#define DISK_MAX 1024*BLOCK_SIZE                            //磁盘大小
#define IndexSize  128                                      //索引区大小
#define DataSize MAX_BLOCK-IndexSize



using namespace std;

//分配数据操作块
int BlockManager::AllocateDataBlock() {
    if(ValidDataDepth>1){
        superBlock.free_data--;
        cout << "DataDepth:" << ValidDataDepth << endl;
        return superBlock.ValidData[ValidDataDepth--];
    } else if(ValidDataDepth==1){
        if(superBlock.ValidData[1]==0)
            return -1;
        int res = superBlock.ValidData[1];
        DataStackCopy(res);
        superBlock.free_data--;
        return res;
    } else{
        return -1;
    }
}
std::vector<int> BlockManager::AllocateDataBlocks(int count) {
    vector<int> res;
    while (count--){
        int temp;
        if((temp=AllocateDataBlock())!=-1) res.emplace_back(temp);
        else{//如果分配块数无法完全分配则一个也不分配
            for(int i:res)FreeBlock(i);
            return {};
        }
    }
    return res;
}

//分配索引操作块
int BlockManager::AllocateIndexBlock() {
    if(ValidIndexDepth>1){
        return superBlock.ValidIndex[ValidIndexDepth--];
    } else if(ValidIndexDepth==1){
        if(superBlock.ValidIndex[1]==0)
            return -1;
        int res = superBlock.ValidIndex[1];
        IndexStackCopy(res);
        return res;
    } else{
        return -1;
    }
}
std::vector<int> BlockManager::AllocateIndexBlcoks(int count) {
    vector<int> res;
    while (count--){
        int temp;
        if((temp=AllocateIndexBlock())!=-1) res.emplace_back(temp);
        else{//如果分配块数无法完全分配则一个也不分配
            for(int i:res)FreeBlock(i);
            return {};
        }
    }
    return res;
}

//释放操作块
//可能会有问题如果已经是空闲块可能会重复释放
void BlockManager::FreeBlocks(const std::vector<int> &blockseq) {
    for(int i: blockseq){
        FreeBlock(i);
    }
}
void BlockManager::FreeBlock(int pos) {
    if(pos>= IndexArea && pos < DataArea){
        if(ValidIndexDepth < ValidSize){
            superBlock.ValidIndex[++ValidIndexDepth] = pos;
        } else{
            CopyIndexStack(pos);
            ValidIndexDepth=1;
            superBlock.ValidIndex[ValidIndexDepth] = pos;
        }
    } else if(pos >= DataArea && pos< DISK_MAX){
        if(ValidDataDepth < ValidSize){
            superBlock.ValidData[++ValidDataDepth] = pos;
            superBlock.free_data++;
        } else{
            CopyDataStack(pos);
            ValidDataDepth=1;
            superBlock.ValidData[ValidDataDepth] = pos;
            superBlock.free_data++;
        }
    } else return;
}

//将源操作块复制至目的操作块
void BlockManager::BlockCopy(int source, int des) {
    void * buf = ReadBlock(source);
    if(!buf) {
        WriteBlock(des, buf);
        free(buf);
    } else{
        throw runtime_error("找不到源操作块");
    }
}
//交换块信息
void BlockManager::BlockExchange(int source, int des) {
    void * s = ReadBlock(source);
    if(s){
        WriteBlock(source,ReadBlock(des));
        WriteBlock(des,s);
        free(s);
    } else{
        throw runtime_error("找不到源操作块");
    }
}
//复制数据栈内容至指定块
void BlockManager::CopyDataStack(int pos) {
    WriteBlock(pos,0,51* sizeof(int),superBlock.ValidData);
}
//复制索引栈内容至制定块
void BlockManager::CopyIndexStack(int pos) {
    WriteBlock(pos,0,51* sizeof(int),superBlock.ValidIndex);
}
//指定块内容放入索引栈
void BlockManager::IndexStackCopy(int pos) {
    void * buffer = ReadBlock(pos,0, sizeof(int)*(ValidSize+1));
    memcpy(superBlock.ValidIndex,(int *)buffer,sizeof(int)*(ValidSize+1));
    usleep(200);
    free(buffer);
}
//制定块内容放入数据栈
void BlockManager::DataStackCopy(int pos) {
    void * buffer = ReadBlock(pos,0, sizeof(int)*(ValidSize+1));
    memcpy(superBlock.ValidData,(int *)buffer,sizeof(int)*(ValidSize+1));
    usleep(200);
    free(buffer);
}
//将buffer数据写入制定pos块
void BlockManager::WriteBlock(int pos, void *buffer) {
    if(pos<0){
        throw runtime_error("文件块错误 指定块:" + to_string(pos));
    }
    if(fseek(disk,pos*BLOCK_SIZE,SEEK_SET)==0){
        fwrite(buffer,1,BLOCK_SIZE,disk);
    } else{
        perror("fseek error");
        throw runtime_error("文件指针定位失败");
    }
}
//将buffer数据写入指定的pos的begin和end之间
void BlockManager::WriteBlock(int pos, int begin, int size, void *buffer) {
    if(pos<0){
        throw runtime_error("文件块错误 指定块:" + to_string(pos));
    }
    if(fseek(disk,pos*BLOCK_SIZE+begin,SEEK_SET)==0){
        fwrite(buffer,1,size,disk);
        fwrite(buffer,1,BLOCK_SIZE-size,disk);
    } else{
        perror("fseek error");
        throw runtime_error("文件指针定位失败");
    }
}
//读取pos位置块的数据  需要free
void *BlockManager::ReadBlock(int pos) {
    void * buffer = malloc(BLOCK_SIZE);
//    cout << "pos=" << pos << endl;
//    cout << "bitpos=" << pos*BLOCK_SIZE << endl;
    if(fseek(disk,pos*BLOCK_SIZE,SEEK_SET)==0){
        fread(buffer, 1,BLOCK_SIZE,disk);
        return buffer;
    } else {
//        cout << "cur=" << ftell(disk) << endl;
        perror("fseek error:");
        free(buffer);
        return nullptr;
    }
}
//读取pos位置块begin位置size大小的数据 需要free
void *BlockManager::ReadBlock(int pos, int begin, int size) {
//    cout << "pos=" << pos << endl;
//    cout << "bitpos=" << pos*BLOCK_SIZE << endl;
    void * buffer = malloc(BLOCK_SIZE);
    if(fseek(disk,pos*BLOCK_SIZE+begin,SEEK_SET)==0){
        fread(buffer, sizeof(char),size,disk);
        return buffer;
    } else {
        free(buffer);
        return nullptr;
    }
}
//工具函数
void BlockManager::FirBloGro(int max, int type,int size) {
    if(size<ValidSize){
        if (type==1){
            superBlock.ValidIndex[0] = size;
            superBlock.ValidIndex[1] = 0;
            for(int i=0;i<=size;i++){
                superBlock.ValidIndex[i+2] = max-i-1;
            }
        } else{
            superBlock.ValidData[0] = size;
            superBlock.ValidData[1] = 0;
            for(int i=0;i<=size;i++){
                superBlock.ValidData[i+2] = max-i-1;
            }
        }
        return;
    }
    if(size<2*ValidSize-1){
        int tmp[ValidSize+1];
        tmp[0] = ValidSize;
        tmp[1] = 0;
        if (type==1){
            superBlock.ValidIndex[0] = size-ValidSize+1;
            for(int i=1;i<=ValidIndexDepth;i++){
                superBlock.ValidIndex[i] = max-ValidSize-i;
            }
        } else{
            superBlock.ValidData[0] = size-ValidSize+1;
            superBlock.ValidData[1] = 0;
            for(int i=1;i<ValidDataDepth;i++){
                superBlock.ValidData[i] = max-ValidSize-i;
            }
        }
        for(int i=2;i<=tmp[0];i++){
            tmp[i] = max-i+1;
        }
        WriteBlock(max-ValidSize+1,tmp);
        return;
    }
    int tmp[ValidSize+1];
    tmp[0] = ValidSize;
    tmp[1] = 0;
    for(int i=2;i<=tmp[0];i++){
        tmp[i] = max-i;
    }
    WriteBlock(max-ValidSize,tmp);
}
//进行成组链接初始化的工具函数
void BlockManager::InitGroup(int &cur, int left,bool type) {
    if(left<=0)
        return;
    int * stack = (int*)malloc(512);
    stack[0] = ValidSize;
    for(int i=1;i<=stack[0];i++){
        stack[i] = cur+ValidSize-i+1;
    }
    WriteBlock(cur,stack);
//    for (int i = 0; i < ValidSize+1; ++i) {
//        if(((i+1)%10)==0)cout << endl;
//        cout << "stack[" << i << "]=" << stack[i] << ' ';
//    }
//    cout << endl;
    if(left<ValidSize){
        if(type){
//            cout << "depth:" << ValidIndexDepth << endl;
            superBlock.ValidIndex[0] = left;
            for(int i=1;i<=ValidIndexDepth;i++){
                superBlock.ValidIndex[i] = cur+1-i;
            }
        } else{
//            cout << "depth:" << ValidDataDepth << endl;
            superBlock.ValidData[0] = left;
            for(int i=1;i<=ValidDataDepth;i++){
                superBlock.ValidData[i] = cur+1-i;
            }
        }
//        cout << superBlock << endl;
        cur -= left;
    } else{
        cur -= 50;
    }
}


//格式化文件
//应该包括将所有块进行成组链接组成成空闲块
void BlockManager::Format(bool type) {

    void * next_block_info = malloc(512);
    //格式化索引区域对于索引区域重新组织

//    cout << "Index Area:" << endl;
    FirBloGro(superBlock.data_begin,1,IndexSize);
    for(int i=superBlock.data_begin-2*ValidSize;i>superBlock.index_begin;){
        int left = i-superBlock.index_begin+1;
        InitGroup(i,left, true);
    }

    //格式化数据区域对于数据区域重新组织

//    cout << "Data Area:" << endl;
    FirBloGro(MAX_BLOCK,0,DataSize);
    for(int i=MAX_BLOCK-2*ValidSize;i>superBlock.data_begin;){
        int left = i-superBlock.data_begin;
        InitGroup(i,left, false);
    }

    //清空超级块信息
    superBlock.Super_Block_Format(type);
    free(next_block_info);

}
//初始化磁盘大小 size表示块数
void BlockManager::InitDisk(int size) {
    void * block = malloc(BLOCK_SIZE);
    for(int i=0;i<size;i++){
        fwrite(block, 1,BLOCK_SIZE,disk);
    }
}




