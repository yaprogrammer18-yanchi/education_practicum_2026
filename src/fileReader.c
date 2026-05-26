#include "HuffmanTree.h"
#include "frequencyTree.h"
#include "minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. размер таблицы - int то есть 4 байта
// 2. Далее идут символ (1 байт) - его длина (1 байт) подряд
// 3. начинаем цикл в котором последовательно заполняем буферные 8 бит и пишем в файл

void writeInFile(char* text, char* outFilepath, Cell** codeTable, uint32_t quantityOfSymbols)
{
    FILE* textFile = fopen(text, "rb");
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
    int ch = 0;

    while ((ch = fgetc(textFile)) != EOF) {

        unsigned char byte = (unsigned char)ch;

        Cell* cell = getCellFromArray(codeTable, byte, quantityOfSymbols);
        if (cell == NULL) {
            break;
        }

        uint64_t code = cellGetCode(cell);
        int codeLength = cellGetLength(cell);

        if (codeLength == 0) {
            break;
        }

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
    FILE* file = fopen(inputFilepath, "rb");
    if (file == NULL) {
        printf("file not found!\n");
        return;
    }

    MinHeap* heap = heapCreate(10);
    if (!heap) {
        printf("Ошибка создания кучи\n");
        return;
    }

    // побайтовый проход по файлу и построение кучи

    int ch = 0;
    size_t quantityOfSymbols = 0;

    while ((ch = fgetc(file)) != EOF) {

        unsigned char byte = (unsigned char)ch;

        HuffNode* currentNode = alreadyInHeap(heap, byte);
        if (currentNode == NULL) {
            currentNode = nodeCreate(byte, 1);
            if (currentNode != NULL) {
                heapPush(heap, currentNode);
                quantityOfSymbols++;
            } else {
                printf("Ошибка создания узла для байта %u\n", byte);
                fclose(file);
                heapFree(heap);
                return;
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

void fileDecompressAndWrite(char* compressedFilepath, char* outputFile)
{
    FILE* inFile = fopen(compressedFilepath, "rb");
    if (!inFile) {
        printf("Cannot open compressed file\n");
        return;
    }
    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        printf("Cannot open outputFile\n");
        return;
    }
    // Читаем количество символов
    uint32_t quantityOfSymbols;
    if (fread(&quantityOfSymbols, sizeof(uint32_t), 1, inFile) != 1) {
        fclose(inFile);
        fclose(outFile);
        return;
    }

    // Читаем таблицу символов и формируем массив указателей на ячейки таблицы
    Cell** arrWithCells = calloc(quantityOfSymbols, sizeof(Cell*));
    if (arrWithCells == NULL) {
        fclose(inFile);
        fclose(outFile);
        return;
    }

    for (uint32_t i = 0; i < quantityOfSymbols; i++) {
        unsigned char symbol;
        unsigned char length;
        if (fread(&symbol, 1, 1, inFile) != 1) {
            freeCellsArray(arrWithCells, i);
            fclose(inFile);
            fclose(outFile);
            return;
        }

        if (fread(&length, 1, 1, inFile) != 1) {
            freeCellsArray(arrWithCells, i);
            fclose(inFile);
            fclose(outFile);
            return;
        }

        Cell* newCell = createCell(symbol, 0, length);
        if (newCell == NULL) {
            freeCellsArray(arrWithCells, i);
            fclose(inFile);
            fclose(outFile);
            return;
        }
        arrWithCells[i] = newCell;
    }

    generateCanonicalCodes(arrWithCells, quantityOfSymbols);

    uint32_t encodedSize = 0;
    fread(&encodedSize, sizeof(uint32_t), 1, inFile);

    uint64_t bufferForSymbol = 0;
    unsigned char usedBits = 0;
    unsigned char byte = 0;
    size_t bytesRead = 0;

    while (bytesRead < encodedSize && fread(&byte, 1, 1, inFile) == 1) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            bufferForSymbol = (bufferForSymbol << 1) | bit;
            usedBits++;

            Cell* cellWithCode = getCellWithCode(arrWithCells, bufferForSymbol, usedBits, quantityOfSymbols);
            if (cellWithCode != NULL) {
                unsigned char ch = cellGetSymbol(cellWithCode);
                fwrite(&ch, sizeof(char), 1, outFile);
                bufferForSymbol = 0;
                usedBits = 0;
            }
        }
        bytesRead++;
    }
    freeCellsArray(arrWithCells, quantityOfSymbols);
    fclose(inFile);
    fclose(outFile);
}
