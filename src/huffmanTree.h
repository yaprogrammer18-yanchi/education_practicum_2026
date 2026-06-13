/*
 * Copyright (c) 2026, Кальсина Яна Вячеславовна
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// структуры
typedef struct HuffNode HuffNode;
typedef struct HuffmanTree HuffmanTree;
typedef struct MinHeap MinHeap;
typedef struct Cell Cell;

// создание нового пустого дерева Хаффмана
// возвращает указатель на структуру дерева
HuffmanTree* treeCreate(void);

// освобождение памяти, занятой деревом
void treeFree(HuffmanTree* tree);

// проверка, пусто ли дерево
// возвращает true, если корень равен NULL или само дерево NULL
bool isEmpty(HuffmanTree* tree);

// создание нового узла дерева с заданным символом и частотой
// возвращает указатель на созданный узел
HuffNode* nodeCreate(unsigned char symbol, unsigned long freq);

// получение символа, хранящегося в узле
// принимает указатель на структуру узла
unsigned char getSymbol(HuffNode* node);

// получение частоты символа в узле
// принимает указатель на структуру узла
unsigned long getFrequency(HuffNode* node);

// получение указателя на корневой узел дерева
// на вход принимает указатель на структуру дерева
HuffNode* getRoot(HuffmanTree* tree);

// получение левого дочернего узла
// принимает указатель на структуру узла
HuffNode* getLeft(const HuffNode* node);

// получение правого дочернего узла
// принимает указатель на структуру узла
HuffNode* getRight(const HuffNode* node);

// назначение левого и правого потомков родительскому узлу
// принимает указатели на структуры корня и двух структур узлов
void addLeftAndRight(HuffNode* root, HuffNode* node1, HuffNode* node2);

// увеличение счётчика частоты в узле на 1
// принимает указатель на структуру узла
// ничего не возвращает
void increaseFrequencyInNode(HuffNode* node);

// установка нового корневого узла для дерева
// принимает указатель на структуру дерева и структуру узла
void treeSetRoot(HuffmanTree* tree, HuffNode* root);

// функция обхода дерева и получения длины кода для символов
// принимает на вход дерево и его размер
// возвращает указатель на массив с указателями на ячейки
Cell** makeCells(HuffmanTree* tree, size_t quantityOfSymbols);

// генерация канонических кодов Хаффмана на основе длин кодов
// принимает отсортированный массив ячеек и количество элементов
void generateCanonicalCodes(Cell** cells, size_t quantityOdCells);

// создание новой ячейки таблицы кодов
// принимает символ, числовое значение кода и длину
// возвращает указатель на созданную ячейку
Cell* createCell(unsigned char symbol, uint64_t code, unsigned char length);

// получение длины канонического кода из ячейки
// принимает указатель на структуру ячейки
unsigned char cellGetLength(Cell* cell);

// получение символа из ячейки таблицы
// принимает указатель на структуру ячейки
unsigned char cellGetSymbol(Cell* cell);

// получение числового значения канонического кода из ячейки
// принимает указатель на структуру ячейки
uint64_t cellGetCode(Cell* cell);

// освобождение памяти массива ячеек и самих ячеек
// принимает указатель на массив с указателями на ячейки и размер массива
void freeCellsArray(Cell** cells, size_t count);

// поиск ячейки по символу в массиве таблицы кодов
// принимает массив, искомый символ и размер массива
// возвращает указатель на найденную ячейку или NULL
Cell* getCellFromArray(Cell** arr, unsigned char ch, size_t count);

// поиск ячейки по значению кода и его длине
// принимает массив, искомый код, длину и размер массива
// возвращает указатель на найденную ячейку или NULL
Cell* getCellWithCode(Cell** arr, uint64_t code, unsigned char length, size_t count);

// функция построения дерева Хаффмана по таблице кодов
// на вход принимает указатель на таблицу кодов и ее размер
// возвращает ноду с нужным символом
HuffNode* buildDecodeTree(Cell** cells, size_t count);
