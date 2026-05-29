#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// структуры
typedef struct HuffNode HuffNode;
typedef struct HuffmanTree HuffmanTree;
typedef struct MinHeap MinHeap;
typedef struct Cell Cell;

// инициализация
HuffmanTree* treeCreate(void);
void treeFree(HuffmanTree* tree);
bool isEmpty(HuffmanTree* tree);

// взаимодействие с
HuffNode* nodeCreate(unsigned char symbol, unsigned long freq);
unsigned char getSymbol(HuffNode* node);
unsigned long getFrequency(HuffNode* node);
HuffNode* getRoot(HuffmanTree* tree);
HuffNode* getLeft(const HuffNode* node);
HuffNode* getRight(const HuffNode* node);
void addLeftAndRight(HuffNode* root, HuffNode* node1, HuffNode* node2);
void increaseFrequencyInNode(HuffNode* node);
void treeSetRoot(HuffmanTree* tree, HuffNode* root);

// функция обхода дерева и получения длины кода для символов
// принимает на вход дерево и его размер
// возвращает указатель на массив с указателями на ячейки
Cell** makeCells(HuffmanTree* tree, size_t quantityOfSymbols);

void generateCanonicalCodes(Cell** cells, size_t quantityOdCells);
Cell* createCell(unsigned char symbol, uint64_t code, unsigned char length);

unsigned char cellGetLength(Cell* cell);
unsigned char cellGetSymbol(Cell* cell);
uint64_t cellGetCode(Cell* cell);
void freeCellsArray(Cell** cells, size_t count);

Cell* getCellFromArray(Cell** arr, char ch, size_t count);
Cell* getCellWithCode(Cell** arr, uint64_t code, unsigned char length, size_t count);