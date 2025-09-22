#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

#include "../src/selection_sort.h"

#define NTESTS 10000

int main()
{   
    void* lib = dlopen("./libSelectionSort.so", RTLD_LAZY);
    if (!lib)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return 1;
    }

    int (*selection_sort)(int*, int) = dlsym(lib, "selection_sort");
    int (*selection_sort_recursive)(int*, int, int) = dlsym(lib, "selection_sort_recursive");

    char* error;
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "dlsym error: %s\n", error);
        dlclose(lib);
        return 1;
    }

    printf("\nИспользование dlopen.\n");
    
    const int arr_size = 300;
    int arr1[arr_size];
    int arr2[arr_size];
    srand(time(NULL));

    double total1 = 0;
    double total2 = 0;
    
    for (int i = 0; i < NTESTS; i++)
    {
        for (int j = 0; j < arr_size; j++)
        {
            int x = rand() % 10000;
            arr1[j] = x;
            arr2[j] = x;
        }
    
        clock_t start1 = clock();
        selection_sort(arr1, arr_size);
        clock_t end1 = clock();
        total1 += ((double)(end1 - start1))/CLOCKS_PER_SEC;

        clock_t start2 = clock();
        selection_sort_recursive(arr2, arr_size, 0);
        clock_t end2 = clock();
        total2 += ((double)(end2 - start2))/CLOCKS_PER_SEC;
    }

    
    printf("\nСреднее значение времени работы функций на основании %d тестов.\n", NTESTS);
    printf("Время выполнения `selection_sort`: %.8f сек\n", total1 / NTESTS);
    printf("Время выполнения `selection_sort_recursive`: %.8f сек\n", total2 / NTESTS);

    dlclose(lib);
    return 0;
}
