#include "../src/HuffmanTree.h"
#include "../src/frequencyTree.h"
#include "../src/minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool testFrequencyTreeNullHeap(void)
{
    HuffmanTree* tree = frequencyTreeCreate(NULL);
    if (tree != NULL) {
        fprintf(stderr, "ERROR: frequencyTreeCreate(NULL) must return NULL\n");
        if (tree) {
            treeFree(tree);
        }
        return false;
    }
    return true;
}

bool testFrequencyTreeEmptyHeap(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }

    HuffmanTree* tree = frequencyTreeCreate(heap);
    heapFree(heap);

    if (!tree) {
        fprintf(stderr, "ERROR: frequencyTreeCreate returned NULL for empty heap\n");
        return false;
    }

    if (getRoot(tree) != NULL) {
        fprintf(stderr, "ERROR: tree root must be NULL for empty heap\n");
        treeFree(tree);
        return false;
    }

    treeFree(tree);
    return true;
}

bool testFrequencyTreeSingleNode(void)
{
    MinHeap* heap = heapCreate(1);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }

    unsigned char symbol = 'x';
    unsigned long freq = 10;

    HuffNode* node = nodeCreate(symbol, freq);
    if (!node || !heapPush(heap, node)) {
        fprintf(stderr, "ERROR: failed to prepare heap for single-node test\n");
        heapFree(heap);
        return false;
    }

    HuffmanTree* tree = frequencyTreeCreate(heap);
    heapFree(heap);

    if (!tree) {
        fprintf(stderr, "ERROR: frequencyTreeCreate returned NULL for single-node heap\n");
        return false;
    }

    HuffNode* root = getRoot(tree);
    if (!root) {
        fprintf(stderr, "ERROR: root is NULL in single-node tree\n");
        treeFree(tree);
        return false;
    }

    if (getFrequency(root) != freq || getSymbol(root) != symbol) {
        fprintf(stderr, "ERROR: root freq/symbol mismatch in single-node tree\n");
        treeFree(tree);
        return false;
    }

    if (getLeft(root) != NULL || getRight(root) != NULL) {
        fprintf(stderr, "ERROR: single-node tree root must have no children\n");
        treeFree(tree);
        return false;
    }

    treeFree(tree);
    return true;
}

bool testFrequencyTreeSumFrequency(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }

    unsigned char symbols[] = { 'a', 'b', 'c' };
    unsigned long freqs[] = { 5, 2, 3 };
    size_t count = sizeof(freqs) / sizeof(freqs[0]);

    unsigned long totalFreq = 0;
    for (size_t i = 0; i < count; ++i) {
        HuffNode* node = nodeCreate(symbols[i], freqs[i]);
        if (!node || !heapPush(heap, node)) {
            fprintf(stderr, "ERROR: preparing heap in TestFrequencyTreeSumFrequency\n");
            heapFree(heap);
            return false;
        }
        totalFreq += freqs[i];
    }

    HuffmanTree* tree = frequencyTreeCreate(heap);
    heapFree(heap);

    if (!tree) {
        fprintf(stderr, "ERROR: frequencyTreeCreate returned NULL for non-empty heap\n");
        return false;
    }

    if (tree == NULL || getRoot(tree) == NULL) {
        fprintf(stderr, "ERROR: root is NULL in non-empty tree\n");
        treeFree(tree);
        return false;
    }
    HuffNode* root = getRoot(tree);
    if (getFrequency(root) != totalFreq) {
        fprintf(stderr, "ERROR: expected root freq %lu, got %lu\n",
            totalFreq, getFrequency(root));
        treeFree(tree);
        return false;
    }

    treeFree(tree);
    return true;
}

bool testIncreaseFrequency(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }

    HuffNode* node1 = nodeCreate('A', 5);
    HuffNode* node2 = nodeCreate('B', 3);

    if (!node1 || !node2) {
        fprintf(stderr, "nodeCreate failed\n");
        if (node1)
            free(node1);
        if (node2)
            free(node2);
        heapFree(heap);
        return false;
    }

    heapPush(heap, node1);
    heapPush(heap, node2);

    increaseFrequency(heap, node1);

    HuffNode* min1 = heapPop(heap);
    if (!min1 || getSymbol(min1) != 'B' || getFrequency(min1) != 3) {
        fprintf(stderr, "ERROR: First pop should be 'B' (freq 3)\n");
        heapFree(heap);
        return false;
    }

    HuffNode* min2 = heapPop(heap);
    if (!min2 || getSymbol(min2) != 'A' || getFrequency(min2) != 6) {
        fprintf(stderr, "ERROR: Second pop should be 'A' (freq 6)\n");
        if (min1)
            free(min1);
        heapFree(heap);
        return false;
    }

    free(min1);
    free(min2);

    heapFree(heap);

    return true;
}

int testsForFrequencyTree(void)
{
    bool okNull = testFrequencyTreeNullHeap();
    bool okEmpty = testFrequencyTreeEmptyHeap();
    bool okSingle = testFrequencyTreeSingleNode();
    bool okSum = testFrequencyTreeSumFrequency();
    bool okFreq = testIncreaseFrequency();

    printf("TestFrequencyTreeNullHeap:   %s\n", okNull ? "PASS" : "FAIL");
    printf("TestFrequencyTreeEmptyHeap:  %s\n", okEmpty ? "PASS" : "FAIL");
    printf("TestFrequencyTreeSingleNode: %s\n", okSingle ? "PASS" : "FAIL");
    printf("TestFrequencyTreeSumFrequency: %s\n", okSum ? "PASS" : "FAIL");
    printf("TestFrequencyIncreaseFrequency: %s\n", okFreq ? "PASS" : "FAIL");

    if (okNull && okEmpty && okSingle && okSum) {
        printf("All frequencyTreeCreate tests passed!\n");
        return 0;
    } else {
        printf("Some frequencyTreeCreate tests FAILED\n");
        return 1;
    }
}

int main(void)
{
    testsForFrequencyTree();
    return 0;
}
