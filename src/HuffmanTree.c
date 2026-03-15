#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct HuffNode {
    unsigned char symbol;
    unsigned long frequency;
    struct HuffNode* left;
    struct HuffNode* right;
} HuffNode;

typedef struct HuffmanTree {
    HuffNode* root;
} HuffmanTree;

HuffmanTree* treeCreate(void)
{
    HuffmanTree* newTree = malloc(sizeof(HuffmanTree));
    if (newTree == NULL) {
        return NULL;
    }
    newTree->root = NULL;
    return newTree;
}

HuffNode* nodeCreate(unsigned char symbol, unsigned long freq)
{
    HuffNode* n = malloc(sizeof(HuffNode));
    if (n == NULL) {
        return NULL;
    }
    n->symbol = symbol;
    n->frequency = freq;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void treeSetRoot(HuffmanTree* tree, HuffNode* root)
{
    if (tree == NULL)
        return;
    tree->root = root;
}

unsigned long getFrequency(HuffNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return node->frequency;
}
unsigned char getSymbol(HuffNode* node)
{
    if (node == NULL) {
        return 0;
    }
    return node->symbol;
}

void addLeftAndRight(HuffNode* root, HuffNode* node1, HuffNode* node2)
{
    if (root == NULL) {
        return;
    }
    root->left = node1;
    root->right = node2;
}

void freeTreeRecursion(HuffNode* node)
{
    if (node == NULL) {
        return;
    }
    freeTreeRecursion(node->left);
    freeTreeRecursion(node->right);
    free(node);
}

void treeFree(HuffmanTree* tree)
{
    if (tree == NULL) {
        return;
    }
    freeTreeRecursion(tree->root);
    free(tree);
}

bool isEmpty(HuffmanTree* tree)
{
    return tree->root;
}

HuffNode* getRoot(HuffmanTree* tree)
{
    if (tree->root == NULL) {
        return NULL;
    }
    return tree->root;
}
HuffNode* getLeft(const HuffNode* node)
{
    if (node == NULL) {
        return NULL;
    }
    return node->left;
}

HuffNode* getRight(const HuffNode* node)
{
    if (node == NULL) {
        return NULL;
    }
    return node->right;
}
