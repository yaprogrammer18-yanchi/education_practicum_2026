#include "HuffmanTree.h"
#include "minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// прочитала - проанализировала - построила дерево частот - построила таблицу символ -
// его код(каноническим алгоритмом) - закодировала текст - отправила в бинарник заголовок блока
// - таблицу символ - его длина - закодированный текст

// функция построения канонических кодов
// функция кодирования
// функция записи в файл

/*
 * функция анализа блока
 * принимает массив с символами, его длину и кучу, в которой будут храниться символы с частотами
 * возвращает размер кучи
 * работает за О(len * heap->size)
 */
size_t blockAnalyzer(char* data, size_t len, MinHeap* heap)
{
    if (!data || !heap || len <= 0) {
        return -1;
    }
    size_t index = 0;
    size_t quantityOfSymbols = 0;
    HuffNode* currentNode = NULL;
    while (index < len) {
        currentNode = alreadyInHeap(heap, data[index]);
        if (currentNode == NULL) {
            currentNode = nodeCreate(data[index], 1);
            if (currentNode != NULL) {
                heapPush(heap, currentNode);
                quantityOfSymbols++;
            }
        } else {
            increaseFrequency(heap, currentNode);
        }
        index++;
    }
    return quantityOfSymbols;
}
