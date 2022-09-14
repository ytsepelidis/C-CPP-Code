#include "functions.h"

int main()
{
	srand((unsigned int)time(NULL));
	int rand_bound = 1000;
	int integers[n_max];
	int A[n_max];
	int B[n_max];
	int n;
	int i;
	int swapsA, swapsB;

	printf("This is a program for executing and comparing two different sorting\n");
	printf("algorithms between two same size arrays.\n");
	printf("\n");

	// Reading integer n
	while (1)
	{
		printf("Please enter an integer between (0, %d] for the array size: ", n_max);
		scanf_s("%d", &n);

		if (n > 0 && n <= n_max)
		{
			printf("You entered the integer %d.\n\n", n);
			break;
		}

		printf("Integer must be in the range (0, %d]\n", n_max);
	}

	// Filling array
	for (i = 0; i < n; i++)
	{
		integers[i] = ((long long)rand_bound * rand()) / (1 + (long long)RAND_MAX);

		// OR manually
		// printf("Enter integer for pos %d: ", i);
		// scanf_s("%d", &integers[i]);
	}

	// Copying array
	for (i = 0; i < n; i++)
	{
		A[i] = integers[i];
		B[i] = integers[i];
	}

	// Sorting the two arrays and checking if they are equal
	swapsA = SortingAlgorithmA(n, A);
	swapsB = SortingAlgorithmB(n, B);
	CheckingArrays(n, A, B);

	// Printing the original array and one of the copies
	printf("Printing original array:\n");
	PrintArray(n, integers);

	if (rand() % 2)
	{
		printf("Printing array A:\n");
		PrintArray(n, A);
	}
	else
	{
		printf("Printing array B:\n");
		PrintArray(n, B);
	}

	// Printing number of swaps
	printf("Sorting algorithm A swapped values a total of %d times.\n", swapsA);
	printf("Sorting algorithm B swapped values a total of %d times.\n", swapsB);

	return 0;
}
