#pragma once
#include "HuffmanTree.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct MinHeap MinHeap;
MinHeap* heapCreate(size_t capacity);
void heapFree(MinHeap* h);
bool heapPush(MinHeap* h, HuffNode* node);
HuffNode* heapPop(MinHeap* h);
int heapEmpty(const MinHeap* h);
size_t heapSize(const MinHeap* h);
void addLeftAndRight(HuffNode* root, HuffNode* node1, HuffNode* node2);
void treeSetRoot(HuffmanTree* tree, HuffNode* root);
