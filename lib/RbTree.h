#pragma once
#include <stdlib.h>
#include "ipv4prefix.h"

typedef enum rbNodeColor {
    BLACK,
    RED
} rbNodeColor;

typedef struct rbNode rbNode;

struct rbNode
{
    /* ipv4 data */
    sIpv4Interval   range;
    sIpv4Prefix     prefix;

    /* list of duplicated nodes */
    size_t dNodesCnt; 
    rbNode* dNodeNext;
    rbNode* dNodePrev;

    // Node RB-tree internals
    unsigned int    maxVal;
    rbNode*         parent;
    rbNode*         left;
    rbNode*         right;
    rbNodeColor     color;
};

typedef struct sRedBlackTree
{
    size_t _cnt;
    rbNode* _root;
} sRedBlackTree;

/* FUNCTIONS */
sRedBlackTree* createRedBlackTree();
void deleteRedBlackTree(sRedBlackTree* rbTree);
int insertRbNode(sRedBlackTree* rbTree, rbNode* node);
void deleteRbNode(sRedBlackTree* rbTree, rbNode* node);
rbNode* allocRbNode();
rbNode* findRbNodeInTree(sRedBlackTree* rbTree, rbNode* node);
rbNode* getTheSmallestPrefixForIp(sRedBlackTree* rbTree, unsigned int ip);