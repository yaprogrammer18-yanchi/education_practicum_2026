#include "HuffmanTree.h"
#include "frequencyTree.h"
#include "minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// вспомагательная функция для отладки
void printBinary(uint64_t code)
{
    if (code == 0) {
        printf("0");
        return;
    }
    int started = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t bit = (code >> i) & 1;
        if (bit || started) {
            printf("%ld", bit);
            started = 1;
        }
    }
}

// 1. размер таблицы - int то есть 4 байта
// 2. Далее идут символ (1 байт) - его длина (1 байт) подряд
// 3. начинаем цикл в котором последовательно заполняем буферные 8 бит и пишем в файл

void writeInFile(char* text, char* outFilepath, Cell** codeTable, uint32_t quantityOfSymbols)
{
    FILE* textFile = fopen(text, "r");
    if (textFile == NULL) {
        printf("file not found!\n");
        return;
    }
    FILE* outFile = fopen(outFilepath, "wb");
    if (outFile == NULL) {
        printf("file not found!\n");
        return;
    }
    fwrite(&quantityOfSymbols, sizeof(uint32_t), 1, outFile);

    // запись: таблица символ-длина 2 байта - 1 строчка таблицы
    for (uint32_t i = 0; i < quantityOfSymbols; i++) {
        char s = cellGetSymbol(codeTable[i]);
        char l = cellGetLength(codeTable[i]);
        fwrite(&s, sizeof(char), 1, outFile);
        fwrite(&l, sizeof(char), 1, outFile);
    }
    long sizePos = ftell(outFile);
    uint32_t encodedSize = 0;
    fwrite(&encodedSize, sizeof(uint32_t), 1, outFile);

    uint8_t buffer = 0;
    char usedBits = 0;
    char ch = 0;

    while ((ch = fgetc(textFile)) != EOF) {
        Cell* cell = getCellFromArray(codeTable, (char)ch, quantityOfSymbols);
        uint64_t code = cellGetCode(cell);
        int codeLength = cellGetLength(cell);
        for (int i = codeLength - 1; i >= 0; i--) {
            uint8_t bit = (code >> i) & 1;
            buffer = (buffer << 1) | bit;
            usedBits++;
            if (usedBits == 8) {
                fwrite(&buffer, sizeof(uint8_t), 1, outFile);
                encodedSize++;
                buffer = 0;
                usedBits = 0;
            }
        }
    }
    if (usedBits > 0) {
        buffer = buffer << (8 - usedBits);
        fwrite(&buffer, sizeof(uint8_t), 1, outFile);
        encodedSize++;
    }
    fseek(outFile, sizePos, SEEK_SET);
    fwrite(&encodedSize, sizeof(uint32_t), 1, outFile);

    fclose(textFile);
    fclose(outFile);
}

void fileCompressAndWrite(char* inputFilepath, char* outputFilepath)
{
    FILE* file = fopen(inputFilepath, "r");
    if (file == NULL) {
        printf("file not found!\n");
        return;
    }

    MinHeap* heap = heapCreate(10);
    if (!heap) {
        printf("Ошибка создания кучи\n");
        return;
    }

    // посимвольный проход по файлу и построение кучи

    int ch = 0;
    size_t quantityOfSymbols = 0;

    while ((ch = fgetc(file)) != EOF) {
        HuffNode* currentNode = alreadyInHeap(heap, (char)ch);
        if (currentNode == NULL) {
            currentNode = nodeCreate((char)ch, 1);
            if (currentNode != NULL) {
                heapPush(heap, currentNode);
                quantityOfSymbols++;
            }
        } else {
            increaseFrequency(heap, currentNode);
        }
    }
    fclose(file);

    // ------------------

    if (quantityOfSymbols == 0) {
        printf("Файл пуст\n");
        heapFree(heap);
        return;
    }
    HuffmanTree* tree = frequencyTreeCreate(heap);
    Cell** cellArr = makeCells(tree, quantityOfSymbols);
    generateCanonicalCodes(cellArr, quantityOfSymbols);
    writeInFile(inputFilepath, outputFilepath, cellArr, (uint32_t)quantityOfSymbols);
    heapFree(heap);
    treeFree(tree);
    freeCellsArray(cellArr, quantityOfSymbols);
}
