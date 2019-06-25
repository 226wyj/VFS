////
//// Created by wulinze on 19-6-21.
////
//
//#include <cstring>
//#include "FileManager.h"
//#include "BlockManager.h"
//
//
//#define SuperBlcok this->bm->getSuperBlock()
//
//using namespace std;
//#define DINODENUM BLOCK_SIZE / DINODE_SIZE  // 每块可以容纳的磁盘i节点数量
//
//// 初始化用户信息表
//void FileManager::readUsr() {
//    BlockManager blockManager;
//    Super_Block superBlock = blockManager.getSuperBlock();
//    usr* temp_usr;
//    int i;
//    for(i = 0; i < superBlock.user_count; i++)
//    {
//        temp_usr = (usr*)blockManager.ReadBlock(superBlock.user_begin, i * sizeof(usr), sizeof(usr));
//        v_usr.push_back(temp_usr);
//    }
//}
//
//// 判断用户名是否已经存在
//int FileManager::usrExists(char* name) {
//    for(usr* v : v_usr)
//    {
//        if(strcmp(v->usr_name, name) == 0)
//            return 1;
//    }
//    return 0;
//}
//
//// 构造函数
//FileManager::FileManager() {
//    readUsr();                  // 读取用户信息
//}
//
//// 注册
//void FileManager::signUp() {
//    cout << "欢迎注册！以下为注意事项：" << endl;
//    cout << "---1. 确保用户名和密码均不超过19个字符---" << endl;
//    cout << "---2. 用户名不能与已经存在的用户重名。如有重名，则系统会要求您重新输入---" << endl;
//    char temp_name[20];
//    char temp_password[20];
//    usr* temp_usr;
//    do{
//        cout << "请输入用户名，若要求您重复输入，则说明该用户名已被使用: " << endl;
//        cin >> temp_name;
//    }while(usrExists(temp_name));
//    cout << "用户名输入成功！请输入密码: " << endl;
//    cin >> temp_password;
//    temp_usr -> usr_name = temp_name;
//    temp_usr -> password = temp_password;
//}
//
//// 打开指定文件
//int FileManager::open(char *filename, Mod mode) {
//    // 检查文件是否已被打开
//    for(int i = 0; i < USRTBLSIZE; i++)
//    {
//        if(strcmp(usrtbl[i].filename, filename) == 0)
//            std::cout << "该文件已被打开，禁止重复打开 ！" << std::endl;
//    }
//    // 用户打开表中新增一项
//    usrtbl[usr_index] =
//}
//
//
//// 磁盘i节点分配
//inode* FileManager::ialloc() {
//
//}
//
//// 磁盘i节点释放
//void FileManager::ifree(uint di_no) {
//
//}
//
//// 内存i节点分配
//inode* FileManager::iget(uint di_no) {
//    BlockManager blockManager;
//    Super_Block& superBlock = blockManager.getSuperBlock(); // 获取超级块信息
//    int existed=0, inodeid, pos, begin;
//    inode* temp = nullptr, *newinode;
//    inodeid = di_no % HASHSIZE;         // 除法散列法
//    uint addr;
//    // 遍历哈希表，查看所需内存i节点是否已被分配
//    if(hinode[inodeid].i_forw != nullptr)
//    {
//        temp = hinode[inodeid].i_forw;
//        while(temp)
//        {
//            if(temp->i_id == di_no)
//            {
//                existed = 1;
//                temp->di_number++;    // 关联+1
//                return temp;
//            }
//            else
//                temp = temp->i_forw;
//        }
//    }
//    // 哈希表中没有要找的内存i节点，则进行分配
//    newinode = (inode*)malloc(sizeof(inode));                   // 分配一个新的内存i节点
//    blockManager.CreateiNode();                                 // i节点总数+1
//    pos = blockManager.AllocateIndexBlock();                    // 获取磁盘上分配的块号
//}
//
//// 内存i节点释放
//void FileManager::iput(inode *di_no) {
//
//}
//
//bool FileManager::vertify_usr(const std::string& uname, const std::string& pwd) {
//
//    if(uname.length() > 10 || pwd.length() > 10)return false;
//    else{
//
//        for(int i=0;i<SuperBlcok.user_count;i++){
//            usr* tmp_usr = (usr*)bm->ReadBlock(SuperBlcok.user_begin, sizeof(usr)*i, sizeof(usr));
//            if(strcmp(tmp_usr->usr_name,&uname[0])==0 &&
//               strcmp(&pwd[0],tmp_usr->password)==0){
//                this->cur_usr = tmp_usr;
//                return true;
//            }
//        }
//
//        return false;
//
//    }
//
//}
