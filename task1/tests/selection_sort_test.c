#include <stdio.h>

#include "../src/selection_sort.h"

void print_arr(const int* arr, int size)
{
    if (!arr || size <= 0) return;
    
    for (int i = 0; i < size; i++)
    {
	    printf("%d", arr[i]);
    }
    printf("\n");
}

int main()
{
	int arr[5][5] = {
		{1, 2, 3, 4, 5},
		{5, 4, 3, 2, 1},
		{1, 3, 2, 4, 5},
		{1, 5, 2, 4, 3},
		{4, 2, 3, 5, 1}
	};
	
	printf("\nTesting `selection_sort` function\n");
	for (int i = 0; i < 5; i++)
	{
		printf("input: ");
		print_arr(arr[i], 5);
		
		selection_sort(arr[i], 5);
		
		printf("output: ");
		print_arr(arr[i], 5);
	}
	printf("Test ended");

	int arr1[5][5] = {
			{1, 2, 3, 4, 5},
			{5, 4, 3, 2, 1},
			{1, 3, 2, 4, 5},
			{1, 5, 2, 4, 3},
			{4, 2, 3, 5, 1}
    };

	printf("\nTesting `selection_sort_recursive` function\n");
    for (int i = 0; i < 5; i++)
    {
        printf("input: ");
        print_arr(arr1[i], 5);

        selection_sort_recursive(arr1[i], 5, 0);

        printf("output: ");
        print_arr(arr1[i], 5);
    }
    printf("Test ended");
	
	return 0;
}
