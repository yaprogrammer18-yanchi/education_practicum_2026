#pragma once
#include "minHeap.h"

// Функция сжатия.
// На вход принимает входной файл и файл, куда пользователь хочет, чтобы она записала сжатые данные
void fileCompressAndWrite(char* inputFilepath, char* outputFilepath);

// Функция расжатия.
// На вход принимает входной файл со сжатыми данными и файл, куда пользователь хочет, чтобы она записала расжатые данные
void fileDecompressAndWrite(char* compressedFilepath, char* outputFile);
