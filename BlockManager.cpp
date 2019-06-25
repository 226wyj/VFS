//
// Created by wulinze on 19-6-19.
//

#include "BlockManager.h"
#define UserBegin this->superBlock.user_begin
#define IndexBegin this->superBlock.index_begin
#define DataBegin this->superBlock.data_begin
#define ValidSize 50
#define ValidDataDepth this->superBlock.ValidData[0]        //数据栈深度
#define ValidIndexDepth this->superBlock.ValidIndex[0]      //索引栈深度
#define BLOCK_SIZE 512                                      //块大小
#define IndexSize  128                                      //索引区大小
#define DataSize MAX_BLOCK-IndexSize
#define INODE_SIZE 64



using namespace std;

//分配数据操作块
int BlockManager::AllocateDataBlock() {
    if(ValidDataDepth>1){
        superBlock.free_data--;
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
            for(int i:res)FreeDataBlock(i);
            return {};
        }
    }
    return res;
}

//分配索引操作块
int BlockManager::AllocateIndexBlock() {
    if(ValidIndexDepth>1){

        superBlock.free_index--;
        return superBlock.ValidIndex[ValidIndexDepth--];
    } else if(ValidIndexDepth==1){
        if(superBlock.ValidIndex[1]==-1)
            return -1;
        int res = superBlock.ValidIndex[1];

        IndexStackCopy(res);

        superBlock.free_index--;
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
            for(int i:res)FreeIndexBlock(i);
            return {};
        }
    }
    return res;
}

//释放数据块
//可能会有问题如果已经是空闲块可能会重复释放
void BlockManager::FreeDataBlocks(const std::vector<int> &blockseq) {
    for(int i: blockseq){
        FreeDataBlock(i);
    }
}
void BlockManager::FreeDataBlock(int pos) {
    if (pos<DataBegin || pos>MAX_BLOCK){
        throw runtime_error("释放错误数据块");
    }
    if(ValidDataDepth < ValidSize){
        superBlock.ValidData[++ValidDataDepth] = pos;
    } else{
        CopyDataStack(pos);
        ValidDataDepth=1;
        superBlock.ValidData[ValidDataDepth] = pos;
    }
    superBlock.free_data++;
}
//释放索引块
void BlockManager::FreeIndexBlock(const std::vector<int>& blockseq) {
    for(int i: blockseq){
        FreeIndexBlock(i);
    }
}
void BlockManager::FreeIndexBlock(int pos) {
    if(pos<0 || pos>BLOCK_SIZE/INODE_SIZE*IndexSize)throw runtime_error("释放索引块错误");
    if(ValidIndexDepth < ValidIndexSize){
        superBlock.ValidIndex[++ValidIndexDepth] = pos;
    } else{
        CopyIndexStack(pos);
        ValidIndexDepth=1;
        superBlock.ValidIndex[ValidIndexDepth] = pos;
    }
    superBlock.free_index++;
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
//复制inode
void BlockManager::InodeCopy(int source, int des) {
    void * buf = ReadIndexBlock(source);
    if(!buf) {
        WriteIndexBlock(des, buf);
        free(buf);
    } else{
        throw runtime_error("找不到源inode");
    }
}
//交换inode
void BlockManager::InodeExchanger(int source, int des) {
    void * s = ReadIndexBlock(source);
    if(s){
        WriteIndexBlock(source,ReadBlock(des));
        WriteIndexBlock(des,s);
        free(s);
    } else{
        throw runtime_error("找不到源inode");
    }
}
//复制数据栈内容至指定块
void BlockManager::CopyDataStack(int pos) {
    WriteBlock(pos,0,(ValidIndexSize+1)* sizeof(int),superBlock.ValidData);
}
//复制索引栈内容至制定索引块
void BlockManager::CopyIndexStack(int pos) {
    WriteIndexBlock(pos,superBlock.ValidIndex);
}
//指定块内容放入索引栈
void BlockManager::IndexStackCopy(int pos) {
    void * buffer = ReadIndexBlock(pos);
    memcpy(superBlock.ValidIndex,buffer, sizeof(int)*(ValidIndexSize+1));
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
//将buffer数据写入指定的索引块
void BlockManager::WriteIndexBlock(int pos, void *buffer) {
    int row = pos/(BLOCK_SIZE/INODE_SIZE);
    int line = pos%(BLOCK_SIZE/INODE_SIZE);

    WriteBlock(row+IndexBegin,line*INODE_SIZE,INODE_SIZE,buffer);

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
//读取指定编号(pos)索引块
void *BlockManager::ReadIndexBlock(int pos) {
    int row = pos/(BLOCK_SIZE/INODE_SIZE);
    int line = pos%(BLOCK_SIZE/INODE_SIZE);

    return ReadBlock(row+IndexBegin,line*INODE_SIZE,INODE_SIZE);
}
//读取pos位置块begin位置size大小的数据 需要free




//工具函数
void BlockManager::FirstData(int end,int size) {
    if(size<ValidSize){
        superBlock.ValidData[0] = size;
        for(int i=1;i<=ValidDataDepth;i++){
            superBlock.ValidData[i] = end - i;
        }
        return;
    }
    if(size<2*ValidSize-1){
        superBlock.ValidData[0] = size-ValidSize+1;

        int tmp[ValidSize+1];
        tmp[0] = ValidSize;
        tmp[1] = 0;
        for(int i=2;i<=tmp[0];i++){
            tmp[i] = end-i+1;
        }

        for(int i=1;i<=ValidDataDepth;i++){
            superBlock.ValidData[i] = end - ValidSize +2 -i;
        }
        WriteBlock(end - ValidSize,0, sizeof(int)*(ValidSize+1),tmp);
        return;
    }

    int tmp[ValidSize+1];
    tmp[0] = ValidSize;
    tmp[1] = 0;
    for(int i=2;i<=tmp[0];i++){
        tmp[i] = end-i+1;
    }

    WriteBlock(end - ValidSize,0, sizeof(int)*(ValidSize+1),tmp);
}
void BlockManager::FirstIndex(int size) {
    if(size < ValidIndexSize){
        superBlock.ValidIndex[0] = size;
        for(int i=1;i<ValidIndexDepth;i++){
            superBlock.ValidIndex[i] = size-i;
        }
        return;
    }
    if(size < 2*ValidIndexSize-1){
        superBlock.ValidIndex[0] = size-ValidIndexSize+1;

        int tmp[ValidIndexSize+1];
        tmp[0] = ValidIndexSize;
        tmp[1] = -1;
        for(int i=2;i<=tmp[0];i++){
            tmp[i] = size-i+1;
        }

        for(int i=1;i<=ValidIndexDepth;i++){
            superBlock.ValidData[i] = size - ValidIndexSize +2 -i;
        }
        WriteIndexBlock(size-ValidIndexSize,tmp);
        return;
    }

    int tmp[ValidIndexSize+1];
    tmp[0] = ValidIndexSize;
    tmp[1] = -1;
    for(int i=2;i<=tmp[0];i++){
        tmp[i] = size-i+1;
    }


    WriteIndexBlock(size-ValidIndexSize, tmp);

}
//进行成组链接初始化的工具函数
void BlockManager::InitDataGroup(int &cur, int left) {
    if(left<=0)
        return;

    int tmp[ValidSize+1];
    tmp[0] = ValidSize;
    for(int i=1;i<ValidSize+1;i++){
        tmp[i] = cur + ValidSize - i;
    }
    WriteBlock(cur-1,0,sizeof(int)*(ValidSize+1),tmp);


    if(left<ValidSize){

        superBlock.ValidData[0] = left;

        for(int i=1;i<=ValidDataDepth;i++){
            superBlock.ValidData[i] = cur-i;
        }

        cur -= left;
    } else{
        cur -= ValidSize;
    }

}
void BlockManager::InitIndexGroup(int &cur) {
    if(cur<=0)
        return;

    int tmp[ValidIndexSize+1];
    tmp[0] = ValidIndexSize;
    for(int i=1;i<ValidIndexSize+1;i++){
        tmp[i] = cur + ValidIndexSize - i;
    }

//    for(int i=0;i<=tmp[0];i++){
//        if(i%10==0)cout << endl;
//        cout << "tmp[" << i << "]=" << tmp[i] << ' ';
//    }
//    cout << endl;

    WriteIndexBlock(cur-1,tmp);

    if(cur<ValidIndexSize){

        superBlock.ValidIndex[0] = cur;

        for(int i=1;i<=ValidIndexDepth;i++){
            superBlock.ValidIndex[i] = cur-i;
        }

        cur = 0;
    } else{
        cur -= ValidIndexSize;
    }
}
//格式化文件
//应该包括将所有块进行成组链接组成成空闲块
void BlockManager::Format(bool type) {


    //清空超级块信息
    superBlock.Super_Block_Format(type);
    //格式化索引区域对于索引区域重新组织

//    cout << "Index Area:" << endl;
    FirstIndex(superBlock.free_index);
    for(int i=superBlock.free_index-2*ValidIndexSize+1;i>0;){
        InitIndexGroup(i);
    }

    //格式化数据区域对于数据区域重新组织

//    cout << "Data Area:" << endl;
    FirstData(MAX_BLOCK,DataSize);
    for(int i=MAX_BLOCK-2*ValidSize+1;i>DataBegin;){
        int left = i-DataBegin;
        InitDataGroup(i,left);
    }

}
//初始化磁盘大小 size表示块数
void BlockManager::InitDisk(int size) {
    void * block = malloc(BLOCK_SIZE);


    for(int i=0;i<size;i++){
        fwrite(block, 1,BLOCK_SIZE,disk);
    }

    free(block);
}










