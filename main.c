#include <stdio.h>
#include "ipv4prefix.h"
#include "RbTree.h"

sRedBlackTree* rbTree = NULL;

int main()
{
    int ret = 0;
    rbTree = createRedBlackTree();
    

    add(541097984, 4);
    // duplicates
    add(541097984, 5);
    add(541097984, 6);
    add(541097984, 7);

    // node after the one to be deleted
    rbNode* dNodeAfter = rbTree->_root->dNodeNext->dNodeNext;

    // delete duplicate node
    del(541097984, 5);

    deleteRedBlackTree(rbTree);

    return 0;
}