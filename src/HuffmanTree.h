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
bool isEmpty(HuffmanTree* tree);
HuffNode* getRoot(HuffmanTree* tree);
HuffNode* getLeft(const HuffNode* node);
HuffNode* getRight(const HuffNode* node);
void extendCodeLength(HuffNode* node);
void increaseFrequency(HuffNode* node);

// ---------- построение таблицы символ - длина кода ----------

typedef struct Cell Cell;

// функция обхода дерева и получения длины кода для символов
// принимает на вход дерево и его размер
// возвращает указатель на массив с указателями на ячейки
Cell** makeCells(HuffmanTree* tree, size_t quantityOfSymbols);
unsigned char cellGetLength(Cell* cell);
unsigned char cellGetSymbol(Cell* cell);