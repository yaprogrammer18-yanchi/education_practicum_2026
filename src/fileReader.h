#pragma once
#include "minHeap.h"

typedef struct Containe Container;
Container* createContainer(MinHeap* heap, size_t qos);
void fileCompressAndWrite(char* inputFilepath, char* outputFilepath);
void fileDecompressAndWrite(char* compressedFilepath, char* outputFile);