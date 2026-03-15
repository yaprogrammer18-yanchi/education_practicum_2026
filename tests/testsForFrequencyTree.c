#include "../src/HuffmanTree.h"
#include "../src/frequencyTree.h"
#include "../src/minHeap.h"
#include <stdbool.h>
#include <stdio.h>

bool TestFrequencyTreeNullHeap(void)
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

bool TestFrequencyTreeEmptyHeap(void)
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

bool TestFrequencyTreeSingleNode(void)
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

bool TestFrequencyTreeSumFrequency(void)
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

    if (!isEmpty(tree)) {
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

int testsForFrequencyTree(void)
{
    bool okNull = TestFrequencyTreeNullHeap();
    bool okEmpty = TestFrequencyTreeEmptyHeap();
    bool okSingle = TestFrequencyTreeSingleNode();
    bool okSum = TestFrequencyTreeSumFrequency();

    printf("TestFrequencyTreeNullHeap:   %s\n", okNull ? "PASS" : "FAIL");
    printf("TestFrequencyTreeEmptyHeap:  %s\n", okEmpty ? "PASS" : "FAIL");
    printf("TestFrequencyTreeSingleNode: %s\n", okSingle ? "PASS" : "FAIL");
    printf("TestFrequencyTreeSumFrequency: %s\n", okSum ? "PASS" : "FAIL");

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
