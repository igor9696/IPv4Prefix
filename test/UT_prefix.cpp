
#include <gtest/gtest.h>

extern "C" {
#include "ipv4prefix.h"
#include "RbTree.h"
}

sRedBlackTree* rbTree = NULL;

TEST(ipv4prefix, CreateRedBlackTreeAndGetValidPointer_StructMembersAreZeros)
{
    rbTree = createRedBlackTree();
    ASSERT_TRUE(rbTree != NULL);
    ASSERT_TRUE(rbTree->_root == NULL);
    ASSERT_TRUE(rbTree->_cnt == 0);
    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, AddPrefix_BaseIsZero_ReturnErr)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    ret = add(0, 20);
    
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(0, rbTree->_cnt);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, AddPrefix_MaskIsZero_ReturnErr)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    ret = add(541097984, 0);
    
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(0, rbTree->_cnt);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, AddFirstPrefix_IncrementCounterAndReturnZero_PrefixIsRoot)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    EXPECT_TRUE(rbTree != NULL);

    ret = add(541097984, 20);

    EXPECT_EQ(0, ret);
    EXPECT_EQ(1, rbTree->_cnt);
    EXPECT_TRUE(NULL != rbTree->_root);
    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, AddingPrefixThatAlreadyExistsDoesntInsertItAndReturnsErrCode)
{
    int ret = 0;
    rbTree = createRedBlackTree();

    ret = add(541097984, 20);
    ret = add(541097984, 20);
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(1, rbTree->_cnt);

    // nodes with the same low key val
    add(541097984, 16);
    add(541097984, 15);
    ret = add(541097984, 15);

    EXPECT_EQ(-1, ret);
    EXPECT_EQ(3, rbTree->_cnt);
    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeletePrefix_MaskIsZero_ReturnErr)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    add(541097984, 20);
    ret = del(541097984, 0);
    
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(1, rbTree->_cnt);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeletePrefix_BaseIsZero_ReturnErr)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    add(541097984, 20);
    ret = del(0, 20);
    
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(1, rbTree->_cnt);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeletePrefixThatExists_DecrementCounterAndReturn0)
{
    int ret = 0;
    rbTree = createRedBlackTree();

    add(541097984, 20);
    ret = del(541097984, 20);
    
    EXPECT_EQ(0, ret);
    EXPECT_EQ(0, rbTree->_cnt);
    EXPECT_TRUE(rbTree->_root == NULL);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeletePrefixThatDoesntExists_ReturnErrCode)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    add(541097984, 20);
    ret = del(541077984, 20);
    
    EXPECT_EQ(-1, ret);
    EXPECT_EQ(1, rbTree->_cnt);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, GetTheSmallestPrefixForGivenIpAddr_WithDuplicatedNodes)
{
    int ret = 0;
    rbTree = createRedBlackTree();
    
    // nodes with the same range->low value
    add(541097984, 4);
    add(541097984, 8);
    add(541097984, 12);
    add(541097984, 16); 
     
    // nodes with the same range->low value
    add(541097984, 20);
    add(541097984, 24);
    add(540097984, 16);
    add(541097984, 28);
    add(541097984, 30);

    add(540097984, 20);
    add(540097984, 24);

    ret = check(541100000);
    EXPECT_EQ(16, ret);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeleteNodeWithDuplicates_NodeReplacedByFirstElementFromTheList)
{
    int ret = 0;
    rbTree = createRedBlackTree();

    add(541097984, 4);
    // duplicates
    add(541097984, 5);
    add(541097984, 6);
    add(541097984, 7);

    EXPECT_TRUE(rbTree->_root->dNodesCnt == 3);
    EXPECT_TRUE(rbTree->_cnt == 4);

    rbNode* firstdNodePtr = rbTree->_root->dNodeNext;
    rbNode* secondNodePtr = rbTree->_root->dNodeNext->dNodeNext;

    // delete node that holds all of the duplicates
    del(541097984, 4);

    EXPECT_TRUE(firstdNodePtr == rbTree->_root);
    EXPECT_TRUE(rbTree->_root->dNodeNext == secondNodePtr);
    EXPECT_TRUE(rbTree->_root->dNodeNext->dNodePrev == rbTree->_root);

    // EXPECT_TRUE(rbTree->_root->dNodes == duplNodeStructurePtr);
    EXPECT_EQ(rbTree->_root->dNodesCnt, 2);
    EXPECT_EQ(rbTree->_cnt, 3);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, DeleteDuplicatedNodeFromTheList_ListPointersUpdated_NodeCounterDecremented)
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
    EXPECT_EQ(rbTree->_cnt, 4);

    // delete duplicate node
    del(541097984, 5);

    EXPECT_TRUE(rbTree->_root->dNodeNext == dNodeAfter);
    EXPECT_TRUE(dNodeAfter->dNodePrev == rbTree->_root);
    EXPECT_EQ(rbTree->_cnt, 3);
    EXPECT_EQ(rbTree->_root->dNodesCnt, 2);

    deleteRedBlackTree(rbTree);
}

TEST(ipv4prefix, IpDoesNotMatchAnyPrefix_ReturnErr)
{
    char ret = 0;
    rbTree = createRedBlackTree();
    
    add(541097984, 4);
    add(541097984, 8);
    add(541097984, 12);
    add(541097984, 16); 
    add(541097984, 20);
    add(541097984, 24);
    add(540097984, 16);
    add(541097984, 28);
    add(541097984, 30);
    add(540097984, 20);
    add(540097984, 24);

    ret = check(540000000);
    EXPECT_EQ(-1, ret);

    ret = check(740000000);
    EXPECT_EQ(-1, ret);

    deleteRedBlackTree(rbTree);
}