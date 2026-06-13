/*
 * Copyright (c) 2026, Кальсина Яна Вячеславовна
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "huffmanTree.h"
#include "minHeap.h"

/*
 * функция построения дерева частот
 * на вход принимает минимальную кучу с элементами (символ - частота) в нужном порядке
 * возвращает указатель на созданное дерево частот
 */
HuffmanTree* frequencyTreeCreate(MinHeap* heap);
