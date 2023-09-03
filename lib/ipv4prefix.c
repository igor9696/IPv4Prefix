#include <stdbool.h>
#include <stdio.h>
#include "ipv4prefix.h"
#include "RbTree.h"

extern sRedBlackTree* rbTree;

static unsigned int ShortMaskToExt(char mask)
{
    unsigned int extMask = 0;
    for(int i = 0; i < mask; i++)
    {
        extMask |= (1 << (31 - i));
    }
    return extMask;
}

static void setIpDataInNode(rbNode* node, unsigned int base, char mask)
{
    unsigned int extMask = ShortMaskToExt(mask);
    unsigned int netIp = base & extMask;

    node->range.low = netIp;
    node->range.high = (~extMask | netIp);
    node->prefix.base = base;
    node->prefix.ext_mask = extMask;
    node->prefix.short_mask = mask;
    node->range.diff = node->range.high - node->range.low;
}

int add(unsigned int base, char mask)
{
    if(base == 0 | mask == 0) {
        return -1;
    }

    rbNode* node = allocRbNode();
    if (node == NULL) {
        return -1;
    }

    setIpDataInNode(node, base, mask);
    if (0 != insertRbNode(rbTree, node)) {
        return -1;
    }
    return 0;
}

int del(unsigned int base, char mask)
{
    rbNode node = {0};
    rbNode* foundNode = NULL;

    setIpDataInNode(&node, base, mask);
    if (NULL != (foundNode = findRbNodeInTree(rbTree, &node))) {
        deleteRbNode(rbTree, foundNode);
    } else {
        return -1;
    }
    return 0;
}

char check(unsigned int ip)
{
    rbNode* node = getTheSmallestPrefixForIp(rbTree, ip);
    if (node != NULL) {
        return node->prefix.short_mask;
    } else {
        return -1;
    }
}