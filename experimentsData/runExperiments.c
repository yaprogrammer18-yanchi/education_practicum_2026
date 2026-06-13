#define _POSIX_C_SOURCE 199309L
#include "../src/fileReader.h"
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define RUNS 10
#define MAX_PATH 1024

static double calcMean(const double* arr, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return n > 0 ? sum / n : 0.0;
}

static double calcStd(const double* arr, int n, double m)
{
    if (n < 2)
        return 0.0;
    double sumSq = 0.0;
    for (int i = 0; i < n; i++)
        sumSq += (arr[i] - m) * (arr[i] - m);
    return sqrt(sumSq / (n - 1));
}

static long getFileSize(const char* path)
{
    struct stat st;
    return stat(path, &st) == 0 ? st.st_size : -1;
}

static int filesIdentical(const char* f1, const char* f2)
{
    long s1 = getFileSize(f1), s2 = getFileSize(f2);
    if (s1 != s2 || s1 < 0)
        return 0;
    FILE *a = fopen(f1, "rb"), *b = fopen(f2, "rb");
    if (!a || !b) {
        if (a)
            fclose(a);
        if (b)
            fclose(b);
        return 0;
    }
    char buf1[4096], buf2[4096];
    size_t n;
    while ((n = fread(buf1, 1, sizeof(buf1), a)) > 0) {
        fread(buf2, 1, n, b);
        if (memcmp(buf1, buf2, n) != 0) {
            fclose(a);
            fclose(b);
            return 0;
        }
    }
    fclose(a);
    fclose(b);
    return 1;
}

static void benchmarkFormatDir(const char* dirPath, const char* fmtName)
{
    DIR* dir = opendir(dirPath);
    if (!dir) {
        perror("opendir");
        return;
    }

    char csvPath[MAX_PATH];
    snprintf(csvPath, sizeof(csvPath), "results/%s_bench.csv", fmtName);
    FILE* csv = fopen(csvPath, "w");
    if (!csv) {
        perror("fopen csv");
        closedir(dir);
        return;
    }

    fprintf(csv, "filename,size_kb,orig_bytes,comp_bytes,ratio_pct,"
                 "comp_mean_ms,comp_std_ms,decomp_mean_ms,decomp_std_ms,roundtrip_ok\n");

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char filePath[MAX_PATH], hufPath[MAX_PATH], restPath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
        snprintf(hufPath, sizeof(hufPath), "%s.huf", filePath);
        snprintf(restPath, sizeof(restPath), "%s.rest", filePath);

        struct stat st;
        if (stat(filePath, &st) != 0 || !S_ISREG(st.st_mode))
            continue;

        printf(" %s (%.1f KB) -> ", entry->d_name, st.st_size / 1024.0);

        double compT[RUNS], decompT[RUNS];
        long compSize = 0;

        for (int i = 0; i < RUNS; i++) {
            struct timespec t0, t1, t2;
            clock_gettime(CLOCK_MONOTONIC, &t0);
            fileCompressAndWrite((char*)filePath, hufPath);
            clock_gettime(CLOCK_MONOTONIC, &t1);
            fileDecompressAndWrite(hufPath, restPath);
            clock_gettime(CLOCK_MONOTONIC, &t2);

            compT[i] = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1e6;
            decompT[i] = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_nsec - t1.tv_nsec) / 1e6;
            if (i == 0)
                compSize = getFileSize(hufPath);
        }

        int ok = filesIdentical(filePath, restPath);
        remove(restPath);
        remove(hufPath);

        double cM = calcMean(compT, RUNS);
        double cS = calcStd(compT, RUNS, cM);
        double dM = calcMean(decompT, RUNS);
        double dS = calcStd(decompT, RUNS, dM);

        double ratio = st.st_size > 0 ? (1.0 - (double)compSize / st.st_size) * 100.0 : 0.0;
        double sizeKb = st.st_size / 1024.0;

        fprintf(csv, "%s,%.2f,%ld,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
            entry->d_name, sizeKb, st.st_size, compSize, ratio,
            cM, cS, dM, dS, ok);

        printf("ratio: %.1f%% | time: %.1f±%.1f ms\n", ratio, cM, cS);
    }

    fclose(csv);
    closedir(dir);
}

int main(int argc, char* argv[])
{
    fprintf(stderr, "[DEBUG] Program started. argc=%d\n", argc);

    if (argc != 2) {
        fprintf(stderr, "Ошибка: нужно указать путь к папке с форматами.\n");
        fprintf(stderr, "Usage: %s <path_to_test_data_folders>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fprintf(stderr, "[DEBUG] Target dir: %s\n", argv[1]);

    if (mkdir("results", 0755) != 0 && errno != EEXIST) {
        perror("mkdir results");
        return EXIT_FAILURE;
    }

    DIR* base = opendir(argv[1]);
    if (!base) {
        fprintf(stderr, "Не удалось открыть директорию: %s\n", argv[1]);
        perror("opendir");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "Директория открыта. Сканирование форматов...\n");

    printf("Запуск эксперимента. Форматы: ");
    fflush(stdout);

    struct dirent* entry;
    int foundFormats = 0;
    while ((entry = readdir(base)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char dirPath[MAX_PATH];
        snprintf(dirPath, sizeof(dirPath), "%s/%s", argv[1], entry->d_name);

        struct stat st;
        if (stat(dirPath, &st) == 0 && S_ISDIR(st.st_mode)) {
            printf("[%s] ", entry->d_name);
            fflush(stdout);
            benchmarkFormatDir(dirPath, entry->d_name);
            foundFormats++;
        }
    }
    printf("\n");

    if (foundFormats == 0) {
        fprintf(stderr, "В указанной директории не найдено подпапок с тестовыми данными.\n");
    } else {
        printf("\n Эксперимент завершён. Таблицы сохранены в ./results/\n");
    }

    closedir(base);
    return EXIT_SUCCESS;
}