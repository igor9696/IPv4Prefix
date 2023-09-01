#pragma once
#include <stdlib.h>
#include "ipv4prefix.h"

typedef enum rbNodeColor {
    BLACK,
    RED
} rbNodeColor;

typedef struct rbDuplicateNode rbDuplicateNode;
typedef struct rbNode rbNode;

struct rbNode
{
    /* ipv4 data */
    sIpv4Interval   range;
    sIpv4Prefix     prefix;

    /* list of duplicated nodes */
    rbDuplicateNode* dNodes;
    size_t dNodesCnt; 

    // Node internals
    unsigned int    maxVal;
    rbNode*         parent;
    rbNode*         left;
    rbNode*         right;
    rbNodeColor     color;
};

struct rbDuplicateNode
{
    rbNode* node;
    rbDuplicateNode* next;
};

typedef struct sRedBlackTree
{
    size_t _cnt;
    rbNode* _root;
} sRedBlackTree;

/* FUNCTIONS */
sRedBlackTree* createRedBlackTree();
void deleteRedBlackTree(sRedBlackTree* rbTree);
rbNode* allocRbNode();
void freeRbNode(rbNode* node);
int insertRbNode(sRedBlackTree* rbTree, rbNode* node);
void deleteRbNode(sRedBlackTree* rbTree, rbNode* node);
rbNode* searchRbNode(sRedBlackTree* rbTree, rbNode* node);
rbNode* getSmallestPrefixForIp(sRedBlackTree* rbTree, int ip);