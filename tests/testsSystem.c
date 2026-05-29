#include "../src/fileReader.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// функция проверки того, что содержимое файлов совпадает
static bool filesIdentical(const char* f1, const char* f2)
{
    FILE *a = fopen(f1, "rb"), *b = fopen(f2, "rb");
    if (!a || !b) {
        if (a)
            fclose(a);
        if (b)
            fclose(b);
        return false;
    }

    fseek(a, 0, SEEK_END);
    fseek(b, 0, SEEK_END);
    long s1 = ftell(a), s2 = ftell(b);
    if (s1 != s2) {
        fclose(a);
        fclose(b);
        return false;
    }
    rewind(a);
    rewind(b);
    char buf1[4096], buf2[4096];
    size_t n;
    while ((n = fread(buf1, 1, sizeof(buf1), a)) > 0) {
        fread(buf2, 1, n, b);
        if (memcmp(buf1, buf2, n) != 0) {
            fclose(a);
            fclose(b);
            return false;
        }
    }
    fclose(a);
    fclose(b);
    return true;
}

// Базовый тест: сжимает файл, разжимает файл, сравнивает результат и оригинал, они должны быть индентичны
bool testBaseOccasion(void)
{
    const char* content = "Huffman coding test with repeating patterns for compression analysis. ";
    FILE* f = fopen("test_in.txt", "wb");
    for (int i = 0; i < 1000; i++)
        fprintf(f, "%s", content);
    fclose(f);

    fileCompressAndWrite("test_in.txt", "test_out.huf");
    fileDecompressAndWrite("test_out.huf", "test_rest.txt");

    bool ok = filesIdentical("test_in.txt", "test_rest.txt");
    remove("test_in.txt");
    remove("test_out.huf");
    remove("test_rest.txt");
    return ok;
}

// Тест бинарного файла со случайными байтами
bool testBinaryFileWithRandomBytes(void)
{
    FILE* f = fopen("test_bin.dat", "wb");
    srand(42);
    for (int i = 0; i < 50000; i++)
        fputc(rand() % 256, f);
    fclose(f);

    fileCompressAndWrite("test_bin.dat", "test_bin.huf");
    fileDecompressAndWrite("test_bin.huf", "test_bin_rest.dat");

    bool ok = filesIdentical("test_bin.dat", "test_bin_rest.dat");
    remove("test_bin.dat");
    remove("test_bin.huf");
    remove("test_bin_rest.dat");
    return ok;
}

// Тестирование файла из одного символа (крайний случай)
bool testSingleCharacter(void)
{
    FILE* f = fopen("test_single.dat", "wb");

    for (int i = 0; i < 10000; i++)
        fputc('X', f);
    fclose(f);

    fileCompressAndWrite("test_single.dat", "test_single.huf");
    fileDecompressAndWrite("test_single.huf", "test_single_rest.dat");

    bool ok = filesIdentical("test_single.dat", "test_single_rest.dat");
    remove("test_single.dat");
    remove("test_single.huf");
    remove("test_single_rest.dat");
    return ok;
}
