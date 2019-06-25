#include <iostream>
#include "BlockManager.h"

using namespace std;


int main() {

    BlockManager* bm = new BlockManager();

    int tmp;
    int* buf;
    int data[16];

    cout << bm->getSuperBlock() << endl;



    for(int i=0;i<10;i++) {
//        if((i+1)%10==0)bm->FreeBlock(tmp);
        tmp = bm->AllocateDataBlock();
         cout << "Data Allocate:" << tmp << endl;
    }
    bm->Format();

    cout << bm->getSuperBlock() << endl;

    if(tmp >= 0){

        bm->WriteBlock(tmp,0, sizeof(int)*50,buf);
    }
    delete bm;

    return 0;
}