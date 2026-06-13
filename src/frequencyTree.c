#include "frequencyTree.h"
#include "huffmanTree.h"
#include "minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

HuffmanTree* frequencyTreeCreate(MinHeap* heap)
{
    if (heap == NULL) {
        return NULL;
    }
    if (heapSize(heap) == 0) {
        HuffmanTree* newTree = treeCreate();
        if (!newTree) {
            return NULL;
        }
        treeSetRoot(newTree, NULL);
        return newTree;
    }
    HuffmanTree* newTree = treeCreate();
    while (heapSize(heap) != 1) {
        HuffNode* node1 = heapPop(heap);
        HuffNode* node2 = heapPop(heap);
        if (!node1 || !node2) {
            return NULL;
        }
        unsigned long freq = getFrequency(node1) + getFrequency(node2);
        HuffNode* newRoot = nodeCreate(' ', freq);
        if (!newRoot) {
            return NULL;
        }
        addLeftAndRight(newRoot, node1, node2);
        if (!heapPush(heap, newRoot)) {
            return NULL;
        }
    }
    HuffNode* newRoot = heapPop(heap);
    treeSetRoot(newTree, newRoot);
    return newTree;
}
