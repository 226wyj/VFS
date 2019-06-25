#include <iostream>
#include "BlockManager.h"

using namespace std;


int main() {

    BlockManager* bm = new BlockManager();

    int tmp;
    int buf[50];

    cout << bm->getSuperBlock() << endl;
    for(int i=0;i<50;i++){
        buf[i] = i+1;
    }

    for(int i=0;i<129;i++) {
        if((i+1)%10==0)bm->FreeBlock(tmp);
        tmp = bm->AllocateIndexBlock();
         cout << "Index Allocate:" << tmp << endl;
    }
//    bm->Format();

    cout << bm->getSuperBlock() << endl;

    if(tmp >= 0){

        bm->WriteBlock(tmp,0, sizeof(int)*50,buf);
    }
    delete bm;

    return 0;
}