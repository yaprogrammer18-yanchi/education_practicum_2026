#include "../src/huffmanTree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool testTreeCreateAndRoot(void)
{
    HuffmanTree* tree = treeCreate();
    if (!tree) {
        fprintf(stderr, "ERROR: treeCreate failed\n");
        return false;
    }
    if (getRoot(tree) != NULL) {
        fprintf(stderr, "ERROR: new tree root must be NULL\n");
        treeFree(tree);
        return false;
    }

    HuffNode* node = nodeCreate('A', 10);
    if (!node) {
        fprintf(stderr, "ERROR: nodeCreate failed\n");
        treeFree(tree);
        return false;
    }
    treeSetRoot(tree, node);

    if (getRoot(tree) != node) {
        fprintf(stderr, "ERROR: root not set correctly\n");
        treeFree(tree);
        return false;
    }

    treeFree(tree);
    return true;
}

bool testNodeCreationAndFree(void)
{
    HuffNode* n = nodeCreate('X', 42);
    if (!n) {
        fprintf(stderr, "ERROR: nodeCreate failed\n");
        return false;
    }
    if (getSymbol(n) != 'X' || getFrequency(n) != 42) {
        fprintf(stderr, "ERROR: node symbol/freq mismatch\n");
        free(n);
        return false;
    }
    if (getLeft(n) != NULL || getRight(n) != NULL) {
        fprintf(stderr, "ERROR: new node must have NULL children\n");
        free(n);
        return false;
    }
    free(n);
    return true;
}

bool testTreeStructure(void)
{
    HuffNode* root = nodeCreate('#', 15);
    HuffNode* left = nodeCreate('L', 5);
    HuffNode* right = nodeCreate('R', 10);
    addLeftAndRight(root, left, right);

    if (getLeft(root) != left || getRight(root) != right) {
        fprintf(stderr, "ERROR: children not linked correctly\n");
        free(root);
        free(left);
        free(right);
        return false;
    }

    HuffmanTree* tree = treeCreate();
    treeSetRoot(tree, root);

    if (tree == NULL || getRoot(tree) == NULL) {
        fprintf(stderr, "ERROR: root is NULL in non-empty tree\n");
        treeFree(tree);
        return false;
    }

    treeFree(tree);
    return true;
}

bool testCanonicalCodesGeneration(void)
{
    size_t count = 3;
    Cell** cells = calloc(count, sizeof(Cell*));
    if (!cells)
        return false;

    cells[0] = createCell('A', 0, 1);
    cells[1] = createCell('B', 0, 2);
    cells[2] = createCell('C', 0, 2);

    generateCanonicalCodes(cells, count);

    bool ok = true;
    if (cellGetLength(cells[0]) != 1 || cellGetCode(cells[0]) != 0) {
        fprintf(stderr, "ERROR: Cell A canonical code mismatch\n");
        ok = false;
    }
    if (cellGetLength(cells[1]) != 2 || cellGetCode(cells[1]) != 2) {
        fprintf(stderr, "ERROR: Cell B canonical code mismatch\n");
        ok = false;
    }
    if (cellGetLength(cells[2]) != 2 || cellGetCode(cells[2]) != 3) {
        fprintf(stderr, "ERROR: Cell C canonical code mismatch\n");
        ok = false;
    }

    freeCellsArray(cells, count);
    return ok;
}

bool testCellLookups(void)
{
    size_t count = 2;
    Cell** cells = calloc(count, sizeof(Cell*));
    cells[0] = createCell('X', 0, 1);
    cells[1] = createCell('Y', 2, 2);

    bool ok = true;
    Cell* found = getCellFromArray(cells, 'Y', count);
    if (!found || cellGetSymbol(found) != 'Y') {
        fprintf(stderr, "ERROR: getCellFromArray failed\n");
        ok = false;
    }

    found = getCellWithCode(cells, 2, 2, count);
    if (!found || cellGetSymbol(found) != 'Y') {
        fprintf(stderr, "ERROR: getCellWithCode failed\n");
        ok = false;
    }

    if (getCellFromArray(cells, 'Z', count) != NULL) {
        fprintf(stderr, "ERROR: should return NULL for missing symbol\n");
        ok = false;
    }
    if (getCellWithCode(cells, 3, 3, count) != NULL) {
        fprintf(stderr, "ERROR: should return NULL for missing code\n");
        ok = false;
    }

    freeCellsArray(cells, count);
    return ok;
}

int testsForHuffmanTree(void)
{
    printf("=== Running HuffmanTree Tests ===\n");
    bool ok1 = testTreeCreateAndRoot();
    printf("TestTreeCreateAndRoot:       %s\n", ok1 ? "PASS" : "FAIL");
    bool ok2 = testNodeCreationAndFree();
    printf("TestNodeCreationAndFree:     %s\n", ok2 ? "PASS" : "FAIL");
    bool ok3 = testTreeStructure();
    printf("TestTreeStructure:           %s\n", ok3 ? "PASS" : "FAIL");
    bool ok4 = testCanonicalCodesGeneration();
    printf("TestCanonicalCodesGeneration:%s\n", ok4 ? "PASS" : "FAIL");
    bool ok5 = testCellLookups();
    printf("TestCellLookups:             %s\n", ok5 ? "PASS" : "FAIL");

    if (ok1 && ok2 && ok3 && ok4 && ok5) {
        printf("All HuffmanTree tests passed!\n");
        return 0;
    }
    printf("Some HuffmanTree tests FAILED\n");
    return 1;
}

int main(void) { return testsForHuffmanTree(); }
