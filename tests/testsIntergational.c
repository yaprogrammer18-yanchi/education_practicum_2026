#include "../src/HuffmanTree.h"
#include "../src/fileReader.h"
#include "../src/frequencyTree.h"
#include "../src/minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Взаимодействие модулей frequencyTree, MinHeap и HuffmanTree

// функция проверки инварианта частот дерева : Частота внутреннего узла = сумма частот потомков
static bool checkTreeFrequencyInvariant(HuffNode* node)
{
    if (!node)
        return true;
    if (!getLeft(node) && !getRight(node))
        return true;

    return (getFrequency(node) == getFrequency(getLeft(node)) + getFrequency(getRight(node))) && checkTreeFrequencyInvariant(getLeft(node)) && checkTreeFrequencyInvariant(getRight(node));
}

// тест, который проверяет, что frequencyTree полностью потребляет переданную ему кучу
bool testFreqTreeHeapConsumption(void)
{
    MinHeap* heap = heapCreate(8);
    if (!heap)
        return false;

    heapPush(heap, nodeCreate('a', 5));
    heapPush(heap, nodeCreate('b', 9));
    heapPush(heap, nodeCreate('c', 12));
    heapPush(heap, nodeCreate('d', 13));

    HuffmanTree* tree = frequencyTreeCreate(heap);
    if (!tree) {
        heapFree(heap);
        return false;
    }

    if (!heapEmpty(heap)) {
        fprintf(stderr, "ERROR: Heap was not fully consumed by frequencyTreeCreate\n");
        treeFree(tree);
        heapFree(heap);
        return false;
    }

    treeFree(tree);
    heapFree(heap);
    return true;
}

// тест, который проверяет корректность использования упорядоченности кучи
bool testFreqTreeHeapOrdering(void)
{
    MinHeap* heap = heapCreate(8);
    if (!heap)
        return false;

    heapPush(heap, nodeCreate('z', 50));
    heapPush(heap, nodeCreate('a', 10));
    heapPush(heap, nodeCreate('m', 30));
    heapPush(heap, nodeCreate('b', 20));

    HuffmanTree* tree = frequencyTreeCreate(heap);
    if (!tree) {
        heapFree(heap);
        return false;
    }

    unsigned long expectedSum = 10 + 20 + 30 + 50;
    HuffNode* root = getRoot(tree);
    if (getFrequency(root) != expectedSum) {
        fprintf(stderr, "ERROR: Root freq %lu != expected sum %lu (heap ordering violated?)\n", getFrequency(root), expectedSum);
        treeFree(tree);
        heapFree(heap);
        return false;
    }

    if (!checkTreeFrequencyInvariant(root)) {
        fprintf(stderr, "ERROR: Tree frequency invariants violated (heap contract broken)\n");
        treeFree(tree);
        heapFree(heap);
        return false;
    }

    treeFree(tree);
    heapFree(heap);
    return true;
}

// тест проверяющий корректность построения дерева (в частности )
bool testFreqTreeTreeStructure(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap)
        return false;

    heapPush(heap, nodeCreate('X', 3));
    heapPush(heap, nodeCreate('Y', 7));

    HuffmanTree* tree = frequencyTreeCreate(heap);
    if (!tree) {
        heapFree(heap);
        return false;
    }

    HuffNode* root = getRoot(tree);
    if (getFrequency(root) != 10) {
        fprintf(stderr, "ERROR: Root frequency mismatch\n");
        treeFree(tree);
        heapFree(heap);
        return false;
    }
    if (!getLeft(root) || !getRight(root)) {
        fprintf(stderr, "ERROR: Root children not linked via addLeftAndRight\n");
        treeFree(tree);
        heapFree(heap);
        return false;
    }

    HuffNode* left = getLeft(root);
    HuffNode* right = getRight(root);
    if (getSymbol(left) != 'X' || getFrequency(left) != 3 || getSymbol(right) != 'Y' || getFrequency(right) != 7) {
        fprintf(stderr, "ERROR: Leaf nodes attached incorrectly to tree root\n");
        treeFree(tree);
        heapFree(heap);
        return false;
    }

    treeFree(tree);
    heapFree(heap);
    return true;
}

// крайние случаи на проверку
bool testFreqTreeTreeEdgeCases(void)
{
    // Пустая куча - должно вернуть дерево с NULL-корнем
    MinHeap* h1 = heapCreate(4);
    HuffmanTree* t1 = frequencyTreeCreate(h1);
    if (!t1 || getRoot(t1) != NULL) {
        fprintf(stderr, "ERROR: Empty heap should produce tree with NULL root\n");
        if (t1)
            treeFree(t1);
        heapFree(h1);
        return false;
    }
    treeFree(t1);
    heapFree(h1);

    // Один элемент - корень дерева должен быть этим листом без детей
    MinHeap* h2 = heapCreate(4);
    heapPush(h2, nodeCreate('Q', 99));
    HuffmanTree* t2 = frequencyTreeCreate(h2);
    if (!t2 || getRoot(t2) == NULL) {
        fprintf(stderr, "ERROR: Single-node heap should produce non-empty tree\n");
        heapFree(h2);
        return false;
    }

    HuffNode* root2 = getRoot(t2);
    if (getSymbol(root2) != 'Q' || getFrequency(root2) != 99) {
        fprintf(stderr, "ERROR: Single-node tree root data mismatch\n");
        treeFree(t2);
        heapFree(h2);
        return false;
    }
    if (getLeft(root2) != NULL || getRight(root2) != NULL) {
        fprintf(stderr, "ERROR: Single-node tree root must have NULL children\n");
        treeFree(t2);
        heapFree(h2);
        return false;
    }

    treeFree(t2);
    heapFree(h2);
    return true;
}

int main(void)
{
    bool t1 = testFreqTreeHeapConsumption();
    printf("Test 1 (freqTree heap consumption): %s\n", t1 ? "PASS" : "FAIL");

    bool t2 = testFreqTreeHeapOrdering();
    printf("Test 2 (freqTree heap ordering):    %s\n", t2 ? "PASS" : "FAIL");

    bool t3 = testFreqTreeTreeStructure();
    printf("Test 3 (freqTree tree structure):   %s\n", t3 ? "PASS" : "FAIL");

    bool t4 = testFreqTreeTreeEdgeCases();
    printf("Test 4 (freqTree tree edge cases):  %s\n", t4 ? "PASS" : "FAIL");

    bool allPassed = t1 && t2 && t3 && t4;

    if (allPassed) {
        printf("All 4 integration tests passed!\n");
        return 0;
    }
    printf("Some tests failed\n");
    return 1;
}
