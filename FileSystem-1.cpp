//
// Created by wulinze on 19-6-19.
//

#include "FileSystem.h"
    void FileSystem::cp()                              //复制指令
    {

    }
    void FileSystem::ls()                               //查看当前目录文件
    {

    }
    void FileSystem::mkdir(string dir_name)             //创建目录
    {

    }
    void FileSystem::cd()                               //移动当前目录
    {

    }
    void FileSystem::rm(string file_name,int i=0)              //删除文件
    {
    unsigned int dinodeid, i;
	struct inode *inode;
	dinodeid = namei(file_name);												//获取对应的i节点

	if (dinodeid != NULL)
		inode = iget(dinodeid);//获取索引节点内容
	else
	{
		cout<<"deleted file does not exist!"<<endl;
		return;
	}
	
	if ((inode->di_mode & DIDIR) != 0 || (inode->di_mode & DIFILE) != 0)	//判断类型，目录或文件
	{
		char flag;
		if ((inode->di_mode & DIDIR) != 0)									
			cout<<">Are you sure to remove the  directory?(y/n):"<< file_name<<endl;
		else cout<<">Are you sure to remove the  file?(y/n):"<< file_name<<endl;
		
		do
		{
			cin>>flag;
			_flushall();													//清除缓冲区
			if (flag == 'y' || flag == 'Y')break;
			else if (flag == 'n' || flag == 'N')return;
			else continue;
		} while (1);
		//检查是否非空
		if ((inode->di_mode & DIDIR) != 0)									//如果是目录
		{
			cd(ROOT, filename);											//跳转到要删除的目录
			if (dir.size > 2)												//如果存在子目录
			{
				cout<<("DIR %s is NOT EMPTY. You can't remove an unEmpty DIR."<<endl;
				cd(ROOT, "..");                   
				return;
			}
			cd(ROOT, "..");                    
		}
	}

	for (i = 0; i<dir.size; i++)
	if (dir.direct[i].d_ino == dinodeid) break;								//找到要删除的文件或者目录

	for (i++; i<dir.size; i++)												//删除目录或文件名
	{
		strcpy(dir.direct[i - 1].d_name, dir.direct[i].d_name);				//前移
		dir.direct[i - 1].d_ino = dir.direct[i].d_ino;
	}
	dir.direct[i - 1].d_ino = 0;											//目录表置空
	dir.size = i - 1;								
	inode->di_number--;														
	iput(inode);
    }
    void FileSystem::mv()                              //移动文件
    {

    }
    void FileSystem::touch(string file_name)            //创建文件
    {
	 int di_pos, di_ino;
	struct inode * inode;
	 int i, j;
	di_ino = namei(file_name);													//在当前目录中查找
	if (di_ino != NULL)	
	{
		cout<<"> has already existed as file or subdirectory"<<file_name<<endl;
		return -1;
	}
	
	else
	{
		di_ino = namei(".");
		inode = iget(di_ino);													//获取当前目录i节点
		if (!access(user_id, inode, WRITE) || !access(user_id, inode, EXICUTE)) 
		{
			cout<<">failed to creat file because of no authority!"<<endl;			
			iput(inode);
			return -1;
		}
		iput(inode);															
		
		inode = ialloc();														//分配新的i节点
		inode->di_mode = mode;																						
		inode->di_size = 0;														
		inode->di_number = 1;													
		
		int i;
		for (i=0; i < NADDR ; i++)
		{
			inode->di_addr[i] = -1;
		}

		di_pos = iname(filename);												//预分配在目录中的位置
		if (di_pos == -1)return;												
		
		dir.size++;																
		dir.direct[di_pos].d_ino = inode->i_ino;								//目录项i节点号赋值								
		iput(inode);
	}
}

    }
    void FileSystem::open(string file_name)             //打开文件
    {
    int dinodeid;
	struct inode * inode;
	int i,j;
	dinodeid =namei(filename);												//给定文件名在当前目录中寻找对应的i节点
	if (dinodeid == NULL)													//if (dinodeid != NULL)    /* nosuchfile */ 
	{
		cout<<">file does not existed!"<<endl;
		return NULL;
	}
	
	for (i = 0; i < SYSOPENFILE; i++)										//系统最多打开40个文件表表项，寻找空文件表表项
	{
		if (sys_ofile[i].f_count==0) break;									//找到空表项退出循环
		if (sys_ofile[i].f_inode->i_ino == inode->i_ino)					//编号匹配，已经打开
		{
			cout<<"File is open already!"<<endl;
			return NULL;
		}
	}

	if (i==SYSOPENFILE)														
	{
		cout<<">Too much file open!"<<endl;
		iput(inode);														
		return NULL;
	}
	//找到空表项
	sys_ofile[i].f_inode=inode;												//记录i节点
	sys_ofile[i].f_flag=openmode;											//记录操作
	sys_ofile[i].f_count=1;													//引用计数+1
	
	if (openmode & FAPPEND)													//在文件尾端添加元素
		sys_ofile[i].f_off=inode->di_size;									//读或写指针移至文件尾端
	else
		sys_ofile[i].f_off=0;												//否则读或写指针移至文件头
	for (j=0; j<NOFILE; j++)												//查找用户文件打开表，每个用户最多打开20个文件
		if (user[user_id].u_ofile[j] == SYSOPENFILE + 1) break;			    //找到空的表项退出循环   if (sys_ofile[i].f_count==0) break;
	if (j==NOFILE)															//没找到空的表项，表满
	{
		cout<<">Too much file opened by the user! "<<endl;
		sys_ofile[i].f_count=0;												//对应的系统打开表引用计数清零
		iput(inode);														
		return NULL;
	}
	//找到空的表项
	user[user_id].u_ofile[j]=i;												//记录在系统打开表中的位置   user[user_id].u_ofile[j]=1;
	
	if (!(openmode & FAPPEND))												//如果不是在文件尾端添加元素，则释放文件旧的磁盘块
	{
		for (i = 0;															
			 i<inode->di_size / BLOCKSIZ + (inode->di_size % BLOCKSIZ != 0);//i<inode->di_size/BLOCKSIZ+1;
			 i++)  
			bfree (inode->di_addr[i]);										
		inode->di_size=0;													
	}
	return j;																//返回在用户打开表中的位置
    }
    bool FileSystem::mount(string path)                 //系统挂载目录
    {

    }
int FileSystem::namei(char *name)//搜索文件的内存i节点指针
{
	int i, notfound = 1;
	for (i = 0; ((i<dir.size) && (notfound)); i++)//当前目录
	{//目录表
		if ((strcmp(dir.direct[i].d_name, name)==0) && (dir.direct[i].d_ino != 0)) //对比当前目录中的名字与要查找的文件名是有相同且对应的i节点号不为0
			return dir.direct[i].d_ino;   /*find,返回对应的内存i节点号 原来是 return i*/
	}
	/* notfind */
	return 0;
};

int FileSystem::iname(char * name)//找一个空的目录表，创建新的目录和文件时使用
{
	int i, notfound = 1;    //默认情况下，不存在空的目录项
	for (i = 0; ((i < DIRNUM) && (notfound)); i++)
	{//内存中一次能放128个目录项
		if (dir.direct[i].d_ino == 0)
		{//存在空的目录项
			notfound = 0;   //说明找到空的目录项啦
			break;
		}
	}
	if (notfound)
	{//找不到空的目录项数组
		printf(">The current directory is full!\n");
		return -1;
	}
	else
	{//找到空的目录项数组
		strcpy(dir.direct[i].d_name, name);/*将要创建的目录(路径名)放到找到的空闲目录项数组中*/
		return i;//返回在当前目录中的位置
	}
}