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
    srand(42); // NOLINT(cert-msc50-cpp)
    for (int i = 0; i < 50000; i++)
        fputc(rand() % 256, f); // NOLINT(cert-msc50-cpp)
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

    fileCompressAndWrite("test_stesingle.dat", "test_single.huf");
    fileDecompressAndWrite("test_single.huf", "test_single_rest.dat");

    bool ok = filesIdentical("test_single.dat", "test_single_rest.dat");
    remove("test_single.dat");
    remove("test_single.huf");
    remove("test_single_rest.dat");
    return ok;
}

static long getFileSize(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fclose(f);
    return sz;
}

// функция принимает файл, сжимает, разжимает, смотрит, совадают ли исходный и обработанный файл
static bool runRoundtrip(const char* orig, const char* huf, const char* rest)
{
    if (getFileSize(orig) < 0) {
        printf("Файл %s не найден.\n", orig);
        return true;
    }

    fileCompressAndWrite((char*)orig, (char*)huf);
    fileDecompressAndWrite((char*)huf, (char*)rest);

    bool ok = filesIdentical(orig, rest);
    long s1 = getFileSize(orig);
    long s2 = getFileSize(huf);

    if (ok && s1 > 0 && s2 >= 0) {
        double ratio = (1.0 - (double)s2 / (double)s1) * 100.0;
        printf("Сжато: %ld B до %ld B (%.1f%%)\n", s1, s2, ratio);
    } else if (!ok) {
        printf("Files are not the same after decompression!\n");
    }
    remove(huf);
    remove(rest);
    return ok;
}

int main(void)
{
    bool r1 = runRoundtrip("tests/testFiles/cFile.c", "cFile.huf", "cFile_rest.c");
    printf("test cFile: %s\n", r1 ? "PASS" : "FAIL");
    bool r2 = runRoundtrip("tests/testFiles/kirilicText.txt", "kirilicText.huf", "kirilicText_rest.txt");
    printf("test kirilicText: %s\n", r2 ? "PASS" : "FAIL");
    bool r3 = runRoundtrip("tests/testFiles/latinText.txt", "latinText.huf", "latinText_rest.txt");
    printf("test latinText: %s\n", r3 ? "PASS" : "FAIL");
    bool r4 = runRoundtrip("tests/testFiles/musicFile.wav", "musicFile.huf", "musicFile_rest.wav");
    printf("test musicFile: %s\n", r4 ? "PASS" : "FAIL");
    bool r5 = runRoundtrip("tests/testFiles/photoFile.bmp", "photoFile.huf", "photoFile_rest.bmp");
    printf("test photoFile.bmp: %s\n", r5 ? "PASS" : "FAIL");

    if (r1 && r2 && r3 && r4 && r5) {
        printf("All 5 system tests passed!.\n");
        return 0;
    }
    printf("Some tests failed.\n");
    return 1;
}
