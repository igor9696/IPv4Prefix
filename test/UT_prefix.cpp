
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
    add(541097984, 8);
    add(541097984, 12);
    add(541097984, 16); 
     
    // nodes with the same range->low value
    add(541097984, 20);
    add(541097984, 24);
    add(541097984, 28);
    add(541097984, 30);

    rbNode* node = getSmallestPrefixForIp(rbTree, 541100000);
    EXPECT_TRUE(node != NULL);
    EXPECT_EQ(30, node->prefix.short_mask);

    deleteRedBlackTree(rbTree);
}
