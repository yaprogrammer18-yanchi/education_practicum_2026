/*
 * Copyright (c) 2026, Кальсина Яна Вячеславовна
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fileReader.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printUsage(const char* prog)
{
    fprintf(stderr, "Использование: %s [-c|-d] <входной_файл> <выходной_файл>\n", prog);
    fprintf(stderr, "  -c    Сжать входной файл\n");
    fprintf(stderr, "  -d    Разжать входной файл\n");
    fprintf(stderr, "  -h    Показать сообщение с инструкцией\n");
}

int main(int argc, char* argv[])
{
    int opt;
    char mode = 0;

    while ((opt = getopt(argc, argv, "cdh")) != -1) {
        switch (opt) {
        case 'c':
        case 'd':
            mode = (char)opt;
            break;
        case 'h':
            printUsage(argv[0]);
            return EXIT_SUCCESS;
        default:
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (!mode || argc - optind != 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const char* input = argv[optind];
    const char* output = argv[optind + 1];

    if (mode == 'c') {
        printf("Сжатие: %s -> %s\n", input, output);
        fileCompressAndWrite((char*)input, (char*)output);
    } else {
        printf("Разжатие: %s -> %s\n", input, output);
        fileDecompressAndWrite((char*)input, (char*)output);
    }
    printf("Выполнено.\n");
    return EXIT_SUCCESS;
}
