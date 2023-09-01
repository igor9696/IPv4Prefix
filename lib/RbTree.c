#include <stdlib.h>
#include <stdio.h>
#include "RbTree.h"
#include "ipv4prefix.h"

// extern sRedBlackTree* rbTree;

static rbDuplicateNode* allocDuplicateNode()
{
    return (rbDuplicateNode*)calloc(1, sizeof(rbDuplicateNode));
}

static void deallocDuplicateNodes(rbNode* node)
{
    while (node->dNodes != NULL) {
        rbDuplicateNode* prevdNode = node->dNodes;
        free(node->dNodes->node);
        node->dNodes = node->dNodes->next;
        free(prevdNode);
        printf("Duplicate node dealocated!\n");
    }
}

static void freeRbNode(rbNode* node)
{   
    free(node);
}

static void freeAllNodesInTree(rbNode* node)
{
    if (node == NULL) return;
    freeAllNodesInTree(node->left);
    freeAllNodesInTree(node->right);
    if (node->dNodes != NULL) {
        deallocDuplicateNodes(node);
    }
    freeRbNode(node);
}

sRedBlackTree* createRedBlackTree()
{
    return (sRedBlackTree*)calloc(1, sizeof(sRedBlackTree));
}

void deleteRedBlackTree(sRedBlackTree* rbTree)
{
    if (rbTree == NULL) return;
    freeAllNodesInTree(rbTree->_root);
    free(rbTree);
}

rbNode* allocRbNode()
{
    return (rbNode*)calloc(1, sizeof(rbNode));
}

static void setMaxValInRbNode(rbNode* node)
{
    unsigned int max = 0;
    if(node->left == NULL && node->right == NULL)
    {
        return ;
    }
    else if (node->left != NULL && node->right != NULL)
    {
        if (node->range.high >= node->left->range.high && 
            node->range.high >= node->right->range.high)
        {
            max = node->range.high;
        }
        else if (node->left->range.high >= node->range.high &&
                    node->left->range.high >= node->range.high)
        {
            max = node->left->range.high;
        }
        else
        {
            max = node->right->range.high;
        }
    }
    else if(node->left == NULL) {
        if (node->range.high > node->right->range.high) {
            max = node->range.high;
        } else {
            max = node->right->range.high;
        }
    }
    else {
        if (node->range.high > node->left->range.high) {
            max = node->range.high;
        } else {
            max = node->left->range.high;
        }
    }

    node->maxVal = max;
}

static int leftRotateRbNode(sRedBlackTree* rbTree, rbNode* x)
{
    rbNode* y = x->right;
    if(y == NULL) {
        return -1;
    }
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL)
    {
        rbTree->_root = y;
    }
    else if (x == x->parent->left)
    {   
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;

    setMaxValInRbNode(x);
    setMaxValInRbNode(y);
    return 0;
}

static int rightRotateRbNode(sRedBlackTree* rbTree, rbNode* x)
{
    rbNode* y = x->left;
    if (y == NULL) {
        return -1;
    }
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == NULL) {
        rbTree->_root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;

    setMaxValInRbNode(x);
    setMaxValInRbNode(y);
    return 0;
}

/* func is fixing up violations of red-black properties 
*  that occured after RED node insertion */
static void insertRbNodeFixup(sRedBlackTree* rbTree, rbNode* node)
{    
    while ( node->parent != NULL &&
            node->parent->parent != NULL &&
            node->parent->color == RED)
    {
        rbNode* y = NULL;
        if (node->parent == node->parent->parent->left) {

            y = node->parent->parent->right;
            if (y != NULL && y->color == RED) {
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
                continue;
            }
            else if (node == node->parent->right) {
                node = node->parent;
                leftRotateRbNode(rbTree, node);
            }
            node->parent->color = BLACK;
            node->parent->parent->color = RED;
            rightRotateRbNode(rbTree, node->parent->parent);
        }
        else {

            y = node->parent->parent->left;

            if (y != NULL && y->color == RED) {
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
                continue;
            }
            else if (node == node->parent->left) {
                node = node->parent;
                rightRotateRbNode(rbTree, node);
            }
            node->parent->color = BLACK;
            node->parent->parent->color = RED;
            leftRotateRbNode(rbTree, node->parent->parent);
        }
    }

    rbTree->_root->color = BLACK;
}

int insertRbNode(sRedBlackTree* rbTree, rbNode* node)
{
    if(rbTree == NULL | node == NULL) {
        return -1;
    }

    rbNode* y = NULL;
    rbNode* x = rbTree->_root;

    // rbTree was empty, so node will be a root
    if (x == NULL) {
        rbTree->_root = node;
        node->dNodes = NULL;
        node->dNodesCnt = 0;
        node->left = NULL;
        node->right = NULL;
        node->color = RED;
        insertRbNodeFixup(rbTree, node);
    } 
    else {

        while ((x != NULL) && (x->range.low != node->range.low))
        {
            y = x;
            if (node->range.low < x->range.low) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        if( x == NULL) {
            if (node->range.low < y->range.low) {
                y->left = node;
            }   
            else {
                y->right = node;
            }

            node->dNodes = NULL;
            node->dNodesCnt = 0;
            node->left = NULL;
            node->right = NULL;
            node->color = RED;
            node->parent = y;
            insertRbNodeFixup(rbTree, node);

        } else {

            if (x->range.high == node->range.high)
            {
                // node already exists
                printf("[DBG] Same node already exists, ret -1\n");
                return -1;
            }

            // traverse duplicate nodes list
            rbDuplicateNode* it = x->dNodes;
            rbDuplicateNode* prev_it = NULL;

            while (it != NULL)
            {   
                prev_it = it;
                if (it->node->range.high == node->range.high)
                {
                    // node already exists
                    printf("[DBG] Same node already exists, ret -1\n");
                    return -1;
                }    
                it = it->next;
            }

            // allocate new duplicate node and update pointers
            it = allocDuplicateNode();
            if (it != NULL) {
                printf("debug: duplication node allocation!\n");
                it->node = node;
                it->next = NULL;
                x->dNodesCnt++;
                
                if (x->dNodes == NULL) {
                    x->dNodes = it;
                } else {
                    prev_it->next = it;
                }
            }
            else {
                return -1;
            }
        } 
    }

    rbTree->_cnt++;
    return 0;
}

static void transplantRbNodes(sRedBlackTree* rbTree, rbNode* u, rbNode* v)
{
    if(u == NULL || v == NULL) 
    {
        return;
    }
    
    if (u->parent == NULL) {
        rbTree->_root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

static rbNode* minimumRbNodePtr(rbNode* node)
{
    rbNode* x = node;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

static rbNode* maximumRbNodePtr(rbNode* node)
{
    rbNode* x = node;
    while (x->right != NULL) {
        x = x->right;
    }
    return x;
}

static void deleteRbNodeFixup(sRedBlackTree* rbTree, rbNode* node)
{
    rbNode* w = NULL;
    while (node != rbTree->_root && node->color == BLACK)
    {
        
        if (node == node->parent->left) {
            w = node->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                leftRotateRbNode(rbTree, node->parent);
                w = node->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else if (w->right->color == BLACK) {
                w->left->color == BLACK;
                w->color = RED;
                rightRotateRbNode(rbTree, w);
            }
            w = node->parent->right;
            w->color = node->parent->color;
            node->parent->color = BLACK;
            w->right->color = BLACK;
            leftRotateRbNode(rbTree, node->parent);
            node = rbTree->_root;
        }
        else {
            w = node->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                leftRotateRbNode(rbTree, node->parent);
                w = node->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else if (w->left->color == BLACK) {
                w->right->color == BLACK;
                w->color = RED;
                rightRotateRbNode(rbTree, w);
                w = node->parent->left;
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->left->color = BLACK;
                leftRotateRbNode(rbTree, node->parent);
                node = rbTree->_root;
            }
        }
    }
    node->color = BLACK;
}

// TODO deletion on duplicate node
void deleteRbNode(sRedBlackTree* rbTree, rbNode* node)
{
    rbNode* y = node;
    rbNode* x = NULL;
    rbNodeColor y_orginall_color = y->color;
    
    if (rbTree->_cnt == 1) {
        printf("Deleting root node!\n");
        rbTree->_root = NULL;
        goto Cleanup;
    }

    // if node has allocated duplicates, then replace node with first element from the list
    if (node->dNodesCnt != 0) {
        rbDuplicateNode* s = node->dNodes; // save address for first dNode structure
        rbNode* tmp = node->dNodes->node;
        tmp->left = node->left;
        tmp->right = node->right;

        tmp->dNodesCnt = node->dNodesCnt - 1;
        tmp->dNodes = node->dNodes->next;

        if (node->left != NULL) {
            node->left->parent = tmp;
        }
        if (node->right != NULL) {
            node->right->parent = tmp;
        }

        if (node->parent != NULL) {
            tmp->parent = node->parent;
            if (node->parent->left == node) {
                node->parent->left = tmp;
            }
            else {
                node->parent->right = tmp;
            }
        }
        else {
            rbTree->_root = tmp;
        }

        // deallocate rbDuplicateNode structure
        free(s);
        goto Cleanup;
    }

    if (node->left == NULL) {
        x = node->right;
        transplantRbNodes(rbTree, node, node->right);
    }
    else if (node->right == NULL) {
        x = node->left;
        transplantRbNodes(rbTree, node, node->left);
    }
    else {
        y = minimumRbNodePtr(node->right);
        y_orginall_color = y->color;
        x = y->right;
        if (y->parent == node) {
            x->parent = y;
        }
        else {
            transplantRbNodes(rbTree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        transplantRbNodes(rbTree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (y_orginall_color == BLACK)
    {
        deleteRbNodeFixup(rbTree, x);
    }

Cleanup:
    freeRbNode(node);
    rbTree->_cnt--;
}

rbNode* searchRbNode(sRedBlackTree* rbTree, rbNode* node)
{   
    rbNode* x = rbTree->_root;
    while (x != NULL && node->range.low != x->range.low)
    {
        if (node->range.low < x->range.low) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (x == NULL) {
        // node is not present
        return NULL;
    }
    else {
        // check if found node is exactly the same
        if (node->range.high == x->range.high) {
            return x;
        }
        else {
            // check if node has duplicates
            if (x->dNodes != NULL) {
                rbDuplicateNode* it = x->dNodes;
                while (it != NULL) {
                    if (node->range.high == x->range.high)
                    {
                        return it->node;
                    }
                    it = it->next;
                }
            }
        }        
    }

    return NULL;
}

rbNode* getSmallestPrefixForIp(sRedBlackTree* rbTree, unsigned int ip)
{
    size_t prefix_range_diff_min = __SIZE_MAX__;
    rbNode* nodeMinPrefix = NULL;
    rbNode* x = rbTree->_root;

    while(x != NULL ) {

        if (ip < x->range.low || ip > x->range.high) {

            if (x->left != NULL && x->left->maxVal >= ip) 
            {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        else { // matching interval found
            printf("[DBG] Found node params: range: %u - %u, mask: %u, range diff: %u, ip: %u\n",
                x->range.low, 
                x->range.high,
                x->prefix.short_mask,
                x->range.diff,
                ip);
            if (x != NULL && (x->range.diff < prefix_range_diff_min))
            {
                prefix_range_diff_min = x->range.diff;
                nodeMinPrefix = x;
            }

            // check if found node has duplicates
            // and if yes then we need to also iterate trough list and check for smallest range diff
            if (x->dNodesCnt != 0 && x->dNodes != NULL) {
                rbDuplicateNode* y = x->dNodes;
                while(y != NULL) {
                    
                    if (y->node->range.diff < prefix_range_diff_min && 
                        y->node->range.high >= ip) {

                        prefix_range_diff_min = y->node->range.diff;
                        nodeMinPrefix = y->node;
                    }
                    y = y->next;
                }
            }

            if (x->left != NULL && x->left->maxVal >= ip) 
            {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
    }
    
    if (nodeMinPrefix != NULL) {
        printf("[DBG] Picked node params: range: %u - %u, mask: %u, range diff: %u, ip: %u\n",
                nodeMinPrefix->range.low, 
                nodeMinPrefix->range.high,
                nodeMinPrefix->prefix.short_mask,
                nodeMinPrefix->range.diff,
                ip);
    }
    return nodeMinPrefix;
}   