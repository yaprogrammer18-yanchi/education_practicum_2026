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

void extendCodeLength(HuffNode* node)
{
    if (node == NULL) {
        return;
    }
    node->codelength++;
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

void increaseFrequency(HuffNode* node)
{
    if (node == NULL) {
        return;
    }
    node->frequency++;
}

// --- обход дерева - формирование таблицы сивол - длина - канонический код

typedef struct Cell {
    unsigned char symbol;
    uint8_t* codeBytes;
    unsigned char length;
} Cell;

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
void InorderRecursion(HuffNode* node, size_t* path, size_t* index, Cell** arrWithCells)
{
    if (node == NULL) {
        return;
    }
    (*path)++;
    InorderRecursion(node->left, path, index, arrWithCells);
    (*path)--;

    if (node != NULL && node->left == NULL && node->right == NULL) {
        Cell* newCell = calloc(1, sizeof(Cell));
        if (newCell == NULL) {
            return;
        }
        newCell->symbol = node->symbol;
        newCell->length = *path;
        arrWithCells[*index] = newCell;
        (*index)++;
        return;
    }
    (*path)++;
    InorderRecursion(node->right, path, index, arrWithCells);
    (*path)--;
    if (node != NULL && node->left == NULL && node->right == NULL) {
        Cell* newCell = calloc(1, sizeof(Cell));
        if (newCell == NULL) {
            return;
        }
        newCell->symbol = node->symbol;
        newCell->length = *path;
        arrWithCells[*index] = newCell;
        (*path)--;
        (*index)++;
        return;
    }
}

int compareCells(const void* a, const void* b)
{
    if (a == NULL || b == NULL) {
        return 0;
    }

    const Cell* cellA = *(const Cell**)a;
    const Cell* cellB = *(const Cell**)b;

    if (cellA->length < cellB->length) {
        return 1;
    } else if (cellA->length > cellB->length) {
        return -1;
    } else if ((cellA->length == cellB->length) && (cellA->symbol < cellB->symbol)) {
        return -1;
    } else if ((cellA->length == cellB->length) && (cellA->symbol < cellB->symbol)) {
        return -1;
    } else if ((cellA->length == cellB->length) && (cellA->symbol > cellB->symbol)) {
        return 1;
    } else {
        return 0;
    }
}

// функция обхода дерева и получения длины кода для символов
// принимает на вход дерево и его размер
// возвращает указатель на отсортированный по убыванию массив с указателями на ячейки
Cell** makeCells(HuffmanTree* tree, size_t quantityOfSymbols)
{
    size_t path = 0;
    size_t index = 0;

    Cell** arrWithCells = calloc(quantityOfSymbols, sizeof(Cell*));
    if (arrWithCells != NULL) {
        InorderRecursion(tree->root, &path, &index, arrWithCells);
    } else {
        return NULL;
    }
    qsort(arrWithCells, quantityOfSymbols, sizeof(Cell*), compareCells);
    return arrWithCells;
}
