#include "functions.h"

int SortingAlgorithmA(int n, int* A)
{
	int i, j;
	int temp;
	int swapsA = 0;

	printf("Sorting algorithm A called.\n");
	for (i = n - 1; i >= 0; i--)
	{
		for (j = 0; j <= i; j++)
		{
			if (A[i] < A[j])
			{
				temp = A[i];
				A[i] = A[j];
				A[j] = temp;

				swapsA += 1;
			}
		}
	}

	return swapsA;
}

int SortingAlgorithmB(int n, int* B)
{
	int i;
	int hasSwapped = 0;
	int temp;
	int swapsB = 0;

	printf("Sorting algorithm B called.\n");
	while (1)
	{
		for (i = 0; i < n - 1; i++)
		{
			if (B[i] > B[i + 1])
			{
				temp = B[i];
				B[i] = B[i + 1];
				B[i + 1] = temp;

				swapsB += 1;
				hasSwapped = 1;
			}
		}

		if (hasSwapped)
		{
			hasSwapped = 0;
			continue;
		}
		else
		{
			break;
		}
	}

	return swapsB;
}

void PrintArray(int n, int* arr)
{
	int i;

	for (i = 0; i < n; i++)
	{
		printf("Array[%d]:\t%d\n", i, arr[i]);
	}

	printf("\n");
}

void CheckingArrays(int n, int* A, int* B)
{
	int i;

	printf("Checking whether the two arrays are identical . . .\n");

	for (i = 0; i < n; i++)
	{
		if (A[i] != B[i])
		{
			printf("The two arrays are not the same. Exiting program . . .\n");
			exit(1);
		}
	}

	printf("The two arrays are identical.\n");
	printf("\n");
}
