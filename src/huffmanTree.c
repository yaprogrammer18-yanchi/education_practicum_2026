/*
 * Copyright (c) 2026, Кальсина Яна Вячеславовна
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct HuffNode {
    unsigned char symbol;
    unsigned long frequency;
    struct HuffNode* left;
    struct HuffNode* right;
    unsigned codelength;
} HuffNode;

typedef struct HuffmanTree {
    HuffNode* root;
} HuffmanTree;

typedef struct Cell {
    unsigned char symbol;
    uint64_t code;
    unsigned char length;
} Cell;

static void freeTreeRecursion(HuffNode* node)
{
    if (node == NULL) {
        return;
    }
    freeTreeRecursion(node->left);
    freeTreeRecursion(node->right);
    free(node);
}

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
    HuffNode* n = calloc(1, sizeof(HuffNode));
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
    return tree == NULL || tree->root == NULL;
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

void increaseFrequencyInNode(HuffNode* node)
{
    if (node == NULL) {
        return;
    }
    node->frequency++;
}

Cell* createCell(unsigned char symbol, uint64_t code, unsigned char length)
{
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (cell == NULL) {
        return NULL;
    }
    cell->symbol = symbol;
    cell->code = code;
    cell->length = length;
    return cell;
}

unsigned char cellGetSymbol(Cell* cell)
{
    if (cell == NULL) {
        return 0; // что лучше? какой символ?
    }
    return cell->symbol;
}

unsigned char cellGetLength(Cell* cell)
{
    if (cell == NULL) {
        return 0; // что лучше? какой символ?
    }
    return cell->length;
}

/*
 * внутренняя рекурсивная функция обхода дерева и формирования ячеек (символ - длина)
 * принимает принимает текущую ноду, ячейку, где будет храниться длина пути
 * индекс массива, в который будут класться структуры ячеек
 * указатель на массив указателей на cells
 */
static void collectLeafLengths(HuffNode* node, unsigned char depth, size_t* index, Cell** arr)
{
    if (!node)
        return;
    if (!node->left && !node->right) {
        unsigned char len = (depth == 0) ? 1 : depth;
        arr[*index] = createCell(node->symbol, 0, len);
        (*index)++;
        return;
    }
    collectLeafLengths(node->left, depth + 1, index, arr);
    collectLeafLengths(node->right, depth + 1, index, arr);
}

static int compareCells(const void* a, const void* b)
{
    if (a == NULL || b == NULL) {
        return 0;
    }
    const Cell* cellA = *(const Cell**)a;
    const Cell* cellB = *(const Cell**)b;
    if (cellA->length != cellB->length) {
        return (cellA->length < cellB->length) ? -1 : 1;
    }
    if (cellA->symbol != cellB->symbol) {
        return (cellA->symbol < cellB->symbol) ? -1 : 1;
    } else {
        return 0;
    }
}

Cell** makeCells(HuffmanTree* tree, size_t quantityOfSymbols)
{
    if (!tree || !tree->root)
        return NULL;

    size_t index = 0;
    Cell** arrWithCells = calloc(quantityOfSymbols, sizeof(Cell*));
    if (!arrWithCells)
        return NULL;

    collectLeafLengths(tree->root, 0, &index, arrWithCells);
    qsort(arrWithCells, quantityOfSymbols, sizeof(Cell*), compareCells);
    return arrWithCells;
}

// алгоритм построения канонических кодов:
// код первого символа = 0 * длину его кода
// предыдущий код + 1 добавляем нули справа до нужной длины
void generateCanonicalCodes(Cell** cells, size_t quantityOdCells)
{
    uint64_t buffer = 0;
    size_t currentLength = 0;
    size_t differrence = 0;
    for (size_t i = 0; i < quantityOdCells; i++) {
        differrence = cells[i]->length - currentLength;
        buffer <<= differrence;
        cells[i]->code = buffer;
        currentLength = cells[i]->length;
        buffer++;
    }
}

uint64_t cellGetCode(Cell* cell)
{
    if (cell == NULL) {
        return 0;
    }
    return cell->code;
}

void freeCellsArray(Cell** cells, size_t count)
{
    if (!cells)
        return;
    for (size_t i = 0; i < count; i++) {
        free(cells[i]);
    }
    free(cells);
}

Cell* getCellFromArray(Cell** arr, unsigned char ch, size_t count)
{
    if (!arr)
        return NULL;
    for (size_t i = 0; i < count; i++) {
        if (cellGetSymbol(arr[i]) == ch) {
            return arr[i];
        }
    }
    return NULL;
}

Cell* getCellWithCode(Cell** arr, uint64_t code, unsigned char length, size_t count)

{
    if (arr == NULL || count == 0) {
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        if (arr[i] == NULL) {
            continue;
        }

        if (cellGetCode(arr[i]) == code && (length == cellGetLength(arr[i]))) {
            return arr[i];
        }
    }
    return NULL;
}

HuffNode* buildDecodeTree(Cell** cells, size_t count)
{
    if (count == 0)
        return NULL;

    HuffNode* root = nodeCreate(0, 0);

    for (size_t i = 0; i < count; i++) {
        HuffNode* curr = root;
        uint64_t code = cellGetCode(cells[i]);
        unsigned char len = cellGetLength(cells[i]);

        for (int bit = len - 1; bit >= 0; bit--) {
            int b = (code >> bit) & 1;
            if (b == 0) {
                if (!getLeft(curr)) {
                    HuffNode* new = nodeCreate(0, 0);
                    addLeftAndRight(curr, new, getLeft(curr));
                }
                curr = getLeft(curr);
            } else {
                if (!getRight(curr)) {
                    HuffNode* new = nodeCreate(0, 0);
                    addLeftAndRight(curr, getLeft(curr), new);
                }
                curr = getRight(curr);
            }
        }
        curr->symbol = cellGetSymbol(cells[i]);
    }
    return root;
}
