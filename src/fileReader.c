#include "frequencyTree.h"
#include "huffmanTree.h"
#include "minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeInFile(char* text, char* outFilepath, Cell** codeTable, uint16_t quantityOfSymbols)
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

    // запись количества уникальных символов
    fwrite(&quantityOfSymbols, sizeof(uint16_t), 1, outFile);

    // запись: таблица символ-длина 2 байта - 1 строчка таблицы
    for (uint16_t i = 0; i < quantityOfSymbols; i++) {
        unsigned char s = cellGetSymbol(codeTable[i]);
        unsigned char l = cellGetLength(codeTable[i]);
        fwrite(&s, sizeof(char), 1, outFile);
        fwrite(&l, sizeof(char), 1, outFile);
    }
    // предварительная запись нулевого размера в заголовок
    long sizePos = ftell(outFile);
    uint32_t encodedSize = 0;
    fwrite(&encodedSize, sizeof(uint32_t), 1, outFile);

    uint8_t buffer = 0;
    char usedBits = 0;
    int ch = 0;

    // проход по тексту, сопоставление символу его кода, заполнение буфера на 8 бит, запись буфера в файл
    while ((ch = fgetc(textFile)) != EOF) {

        unsigned char byte = (unsigned char)ch;
        Cell* cell = getCellFromArray(codeTable, (unsigned char)byte, quantityOfSymbols);
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
            encodedSize++;
            if (usedBits == 8) {
                fwrite(&buffer, sizeof(uint8_t), 1, outFile);
                buffer = 0;
                usedBits = 0;
            }
        }
    }
    // добавление паддинга, если последний байт получился неполным
    if (usedBits > 0) {
        buffer = buffer << (8 - usedBits);
        fwrite(&buffer, sizeof(uint8_t), 1, outFile);
    }
    // запись размера сжатого файла в битах
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

    int ch = 0;
    size_t quantityOfSymbols = 0;
    // побайтовый проход по файлу и построение кучи
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

    if (quantityOfSymbols == 0) {
        printf("Файл пуст\n");
        heapFree(heap);
        return;
    }

    HuffmanTree* tree = frequencyTreeCreate(heap);
    Cell** cellArr = makeCells(tree, quantityOfSymbols);
    generateCanonicalCodes(cellArr, quantityOfSymbols);
    writeInFile(inputFilepath, outputFilepath, cellArr, (uint16_t)quantityOfSymbols);
    heapFree(heap);
    treeFree(tree);
    freeCellsArray(cellArr, quantityOfSymbols);
}

void fileDecompressAndWrite(char* compressedFilepath, char* outputFile)
{
    FILE* inFile = fopen(compressedFilepath, "rb");
    if (!inFile) {
        printf("Невозможно открыть сжатый файл\n");
        return;
    }
    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        printf("Невозможно открыть файл для записи\n");
        return;
    }

    uint16_t quantityOfSymbols;
    if (fread(&quantityOfSymbols, sizeof(uint16_t), 1, inFile) != 1) {
        fclose(inFile);
        fclose(outFile);
        return;
    }

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

    HuffNode* decodeTree = buildDecodeTree(arrWithCells, quantityOfSymbols);
    HuffNode* current = decodeTree;

    uint32_t bytesToRead = (encodedSize + 7) / 8;
    unsigned char byte = 0;
    size_t bytesRead = 0;
    uint32_t bitsProcessed = 0;

    while (bytesRead < bytesToRead && fread(&byte, 1, 1, inFile) == 1) {
        for (int i = 7; i >= 0; i--) {
            if (bitsProcessed >= encodedSize)
                break;

            int bit = (byte >> i) & 1;
            bitsProcessed++;

            current = (bit == 0) ? getLeft(current) : getRight(current);

            if (current && !getLeft(current) && !getRight(current)) {
                unsigned char sym = getSymbol(current);
                fwrite(&sym, 1, 1, outFile);
                current = decodeTree;
            }
        }
        bytesRead++;
    }
    HuffmanTree* tmpTree = treeCreate();
    treeSetRoot(tmpTree, decodeTree);
    treeFree(tmpTree);
    freeCellsArray(arrWithCells, quantityOfSymbols);
    fclose(inFile);
    fclose(outFile);
}
