#include "../src/HuffmanTree.h"
#include "../src/minHeap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool testHeapEmptyInitial(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }
    bool ok = true;
    if (!heapEmpty(heap)) {
        fprintf(stderr, "ERROR: heap should be empty\n");
        ok = false;
    }
    if (heapPop(heap) != NULL) {
        fprintf(stderr, "ERROR: pop from empty heap should return NULL\n");
        ok = false;
    }
    heapFree(heap);
    return ok;
}

bool testHeapPushAndSize(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }

    unsigned long freqs[] = { 5, 1, 3, 8, 2 };
    unsigned char syms[] = { 'a', 'b', 'c', 'd', 'e' };
    size_t count = sizeof(freqs) / sizeof(freqs[0]);

    bool ok = true;

    for (size_t i = 0; i < count; ++i) {
        HuffNode* node = nodeCreate(syms[i], freqs[i]);
        if (!node) {
            fprintf(stderr, "nodeCreate failed\n");
            ok = false;
            break;
        }
        if (!heapPush(heap, node)) {
            fprintf(stderr, "heapPush failed on i=%zu\n", i);
            ok = false;
            break;
        }
    }
    if (ok && heapSize(heap) != count) {
        fprintf(stderr, "ERROR: expected size %zu, got %zu\n",
            count, heapSize(heap));
        ok = false;
    }
    HuffNode* node;
    while ((node = heapPop(heap)) != NULL) {
        free(node);
    }
    heapFree(heap);
    return ok;
}

bool testHeapPopOrder(void)
{
    MinHeap* heap = heapCreate(4);
    if (!heap) {
        fprintf(stderr, "heapCreate failed\n");
        return false;
    }
    unsigned long freqs[] = { 5, 1, 3, 8, 2 };
    unsigned char syms[] = { 'a', 'b', 'c', 'd', 'e' };
    size_t count = sizeof(freqs) / sizeof(freqs[0]);

    for (size_t i = 0; i < count; ++i) {
        HuffNode* node = nodeCreate(syms[i], freqs[i]);
        if (!node || !heapPush(heap, node)) {
            fprintf(stderr, "ERROR: preparing heap for pop order test\n");
            heapFree(heap);
            return false;
        }
    }

    unsigned long expected[] = { 1, 2, 3, 5, 8 };
    bool ok = true;

    for (size_t i = 0; i < count; ++i) {
        HuffNode* min = heapPop(heap);
        if (!min) {
            fprintf(stderr, "ERROR: pop returned NULL at i=%zu\n", i);
            ok = false;
            break;
        }
        printf("pop %zu: freq=%lu, symbol=%c\n",
            i, getFrequency(min), getSymbol(min));
        if (getFrequency(min) != expected[i]) {
            fprintf(stderr, "ERROR: expected freq %lu, got %lu\n",
                expected[i], getFrequency(min));
            ok = false;
        }
        free(min);
    }

    if (!heapEmpty(heap)) {
        fprintf(stderr, "ERROR: heap should be empty after pops\n");
        ok = false;
    }

    heapFree(heap);
    return ok;
}

int testsForMinHeap(void)
{
    bool okEmpty = testHeapEmptyInitial();
    bool okPush = testHeapPushAndSize();
    bool okOrder = testHeapPopOrder();

    if (okEmpty && okPush && okOrder) {
        printf("All heap tests passed!\n");
        return 0;
    } else {
        printf("Some heap tests FAILED\n");
        return 1;
    }
}

int main(void)
{
    testsForMinHeap();
    return 0;
}
