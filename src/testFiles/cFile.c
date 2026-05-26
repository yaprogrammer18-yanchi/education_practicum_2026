#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 10000

void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void quickSort(int arr[], int low, int high)
{
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void printArray(int arr[], int n, const char* name)
{
    printf("%s: [", name);
    for (int i = 0; i < n && i < 20; i++) {
        printf("%d", arr[i]);
        if (i < 19 && i < n - 1)
            printf(", ");
    }
    if (n > 20)
        printf(", ...");
    printf("]\n");
}

int main()
{

    int arr1[ARRAY_SIZE];
    int arr2[ARRAY_SIZE];
    int arr3[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (i < ARRAY_SIZE / 3)
            arr1[i] = 1;
        else if (i < 2 * ARRAY_SIZE / 3)
            arr1[i] = 2;
        else
            arr1[i] = 3;
    }
    srand(42);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr2[i] = rand() % 100;
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr3[i] = i % 10; // Цикл от 0 до 9
    }

    printArray(arr1, ARRAY_SIZE, "Массив 1 (группы)");
    printArray(arr2, ARRAY_SIZE, "Массив 2 (случайный)");
    printArray(arr3, ARRAY_SIZE, "Массив 3 (циклический)");

    int sorted1[ARRAY_SIZE];
    int sorted2[ARRAY_SIZE];
    int sorted3[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        sorted1[i] = arr1[i];
        sorted2[i] = arr2[i];
        sorted3[i] = arr3[i];
    }

    printf("Сортируем массив 1 пузырьком...\n");
    bubbleSort(sorted1, ARRAY_SIZE);

    printf("Сортируем массив 2 быстрой сортировкой...\n");
    quickSort(sorted2, 0, ARRAY_SIZE - 1);

    printf("Сортируем массив 3 быстрой сортировкой...\n");
    quickSort(sorted3, 0, ARRAY_SIZE - 1);

    printArray(sorted1, ARRAY_SIZE, "Отсорт. массив 1");
    printArray(sorted2, ARRAY_SIZE, "Отсорт. массив 2");
    printArray(sorted3, ARRAY_SIZE, "Отсорт. массив 3");

    for (int i = 0; i < 100; i++) {
        printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit. ");
        printf("Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
        if (i % 10 == 0)
            printf("\n");
    }

    for (int i = 1; i <= 20; i++) {
        for (int j = 1; j <= 20; j++) {
            printf("%4d", i * j);
        }
        printf("\n");
    }

    long long sum = 0;
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            sum += i * j;
        }
    }
    return 0;
}