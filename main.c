#include <stdio.h>
#include "ipv4prefix.h"
#include "RbTree.h"
#include "assert.h"


sRedBlackTree* rbTree = NULL;

int main()
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    // nodes with the same range->low value
    add(541097984, 4);
    add(541097984, 5);
    add(541097984, 6);
    add(541097984, 7);

    del(541097984, 4);

    

    rbNode* node = getSmallestPrefixForIp(rbTree, 541300000);
    

    deleteRedBlackTree(rbTree);

    return 0;
}