#include "selection_sort.h"

void selection_sort(int *arr, int size)
{
        if (!arr || size <= 0) return;

        for (int i = 0; i < size-1; i++)
        {
                int min_index = i;
                for (int j = i+1; j < size; j++)
                {
                        if (arr[j] < arr[min_index])
                                min_index = j;
                }

                int temp = arr[i];
                arr[i] = arr[min_index];
                arr[min_index] = temp;
        }
}

void selection_sort_recursive(int *arr, int size, int index)
{
        if (!arr || size <= 0 || index < 0 || index >= size) return;

        if (index == size - 1) return;

        int min_index = index;
        for (int i = index + 1; i < size; i++)
        {
                if (arr[i] < arr[min_index])
                        min_index = i;
        }

        int temp = arr[index];
        arr[index] = arr[min_index];
        arr[min_index] = temp;

        selection_sort_recursive(arr, size, index + 1);
}
