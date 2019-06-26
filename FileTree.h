//
// Created by wulinze on 19-6-21.
//

#ifndef FILESYSTEM_FILETREE_H
#define FILESYSTEM_FILETREE_H

#include "FileManager.h"



struct FNode{

    std::string filename;



};

class FileTree {
private:
    FNode* root;
public:
    void VisualTree(const FNode* root);
    void Init(const dinode& root);
};


#endif //FILESYSTEM_FILETREE_H
