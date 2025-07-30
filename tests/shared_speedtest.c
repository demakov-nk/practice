#include <stdio.h>
#include <time.h>

#include "../src/selection_sort.h"

int main()
{
    printf("\nДИНАМИЧЕСКАЯ линковка.\n");
    printf("Сортировка массива: {1000, 999, 998, ..., 1}\n");

    const int N = 1000;
    int arr1[N];
    int arr2[N];
    for (int i = 0; i < N; i++)
    {
        arr2[i] = arr1[i] = N - i;
    }
    
    clock_t start1 = clock();
    selection_sort(arr1, N);
    clock_t end1 = clock();
    double time1 = ((double)(end1 - start1))/CLOCKS_PER_SEC;
    printf("Время выполнения `selection_sort`: %.6f сек\n", time1);

    clock_t start2 = clock();
    selection_sort_recursive(arr2, N, 0);
    clock_t end2 = clock();
    double time2 = ((double)(end2 - start2))/CLOCKS_PER_SEC;
    printf("Время выполнения `selection_sort_recursive`: %.6f сек\n", time2);

    return 0;
}
