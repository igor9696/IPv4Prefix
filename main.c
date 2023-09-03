#include <stdio.h>
#include "ipv4prefix.h"
#include "RbTree.h"
#include <assert.h>

sRedBlackTree* rbTree = NULL;

int main()
{
    /* Example */
    int ret = 0;
    unsigned int ip = 541493984;
    rbTree = createRedBlackTree(); // Red black tree needs to be first allocated
    assert(rbTree != NULL);

    add(541097984, 4);
    add(541297984, 5);
    add(541397984, 6);
    add(541497984, 7);

    del(541397984, 5);

    ret = check(541493984);
    if (ret == -1) {
        printf("Prefix for ip: %u not found!\n", ip);
    }
    else {
        printf("Matched prefix found with mask %d\n", ret);
    }

    deleteRedBlackTree(rbTree); // to release resources in Tree
    return 0;
}