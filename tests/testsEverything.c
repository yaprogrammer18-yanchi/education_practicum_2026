#include "../src/HuffmanTree.h"
#include "../src/fileReader.h"
#include "../src/frequencyTree.h"
#include "../src/minHeap.h"
#include "string.h"
#include <stdlib.h>

int testEverything()
{
    fileCompressAndWrite("src/tryFile.txt", "src/tryOut.bin");
    fileDecompressAndWrite("src/tryOut.bin", "src/decompressedOut.txt");
    return 0;
}

int main()
{
    testEverything();
    return 0;
}