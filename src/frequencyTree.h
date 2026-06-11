#pragma once
#include "HuffmanTree.h"
#include "minHeap.h"

/*
 * функция построения дерева частот
 * на вход принимает минимальную кучу с элементами (символ - частота) в нужном порядке
 * возвращает указатель на созданное дерево частот
 */
HuffmanTree* frequencyTreeCreate(MinHeap* heap);
