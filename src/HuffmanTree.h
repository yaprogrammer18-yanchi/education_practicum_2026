#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct HuffNode HuffNode;
typedef struct HuffmanTree HuffmanTree;
typedef struct MinHeap MinHeap;
HuffmanTree* treeCreate(void);
void treeFree(HuffmanTree* tree);
bool treeMerge(HuffmanTree* tree1, HuffmanTree* tree2);
HuffNode* nodeCreate(unsigned char symbol, unsigned long freq);
unsigned char getSymbol(HuffNode* node);
unsigned long getFrequency(HuffNode* node);
bool IsEmpty(HuffmanTree* tree);
HuffNode* getRoot(HuffmanTree* tree);
HuffNode* getLeft(const HuffNode* node);
HuffNode* getRight(const HuffNode* node);