#include <stdio.h>
#include "ipv4prefix.h"
#include "RbTree.h"
#include "assert.h"


sRedBlackTree* rbTree = NULL;

int main()
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    add(541097984, 8);
    add(541097984, 12);
    add(541097984, 16); 
     
    // nodes with the same range->low value
    add(541097984, 20);
    add(541097984, 24);
    add(541097984, 28);
    add(541097984, 30);


    rbNode* node = getSmallestPrefixForIp(rbTree, 541100000);
    


    deleteRedBlackTree(rbTree);
    return 0;
}