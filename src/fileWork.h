#pragma once
#include "minHeap.h"

typedef struct Containe Container;
Container* createContainer(MinHeap* heap, size_t qos);
size_t blockAnalyzer(char* data, size_t len, MinHeap* heap);