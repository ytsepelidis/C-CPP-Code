#include "functions.h"

int main()
{
	int choice;
	int exitFlag = 0;
	int isSorted = 0;
	int isRowSorted = 0;
	int M1;
	int M2;
	SparseMatrix sm1;
	SparseMatrix sm2;

	printf("This is a program for computing operations between sparse matrices \n");
	printf("with the I/O of said matrices occuring with files given by the user.\n");
	printf("\n");

	// Reading initial sparse matrix
	ReadSparseMatrixFromFile(&M1, sm1);
	PrintSparseMatrix(M1, sm1);

	// Choice menu loop
	while (1)
	{
		printf("~~~~ MENU ~~~~\n");
		printf("1) Sort sparse matrix by rows and columns\n");
		printf("2) Sort sparse matrix by columns and rows\n");
		printf("3) Add two sparse matrices\n");
		printf("4) Subtract two sparse matrices\n");
		printf("5) Multiply two sparse matrices\n");
		printf("0) Exit\n");

		printf("Enter your choice (0 - 5): ");
		scanf("%d", &choice);
		printf("\n");

		switch (choice)
		{
		case 1:
			SortingRowWise(M1, sm1);
			printf("Sorted sparse matrix by rows and columns.\n");
			PrintSparseMatrix(M1, sm1);

			isSorted = 1;
			isRowSorted = 1;

			break;
		case 2:
			SortingColumnWise(M1, sm1);
			printf("Sorted sparse matrix by columns and rows.\n");
			PrintSparseMatrix(M1, sm1);

			isSorted = 1;
			isRowSorted = 0;

			break;
		case 3:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				printf("The sparse matrix read from file has not been sorted.\n");
				printf("To execute the operation the matrix has to be sorted.\n");
				printf("\n");

				break;
			}

			printf("You have to provide the filename for another sparse matrix for the operation to occur.\n");
			ReadSparseMatrixFromFile(&M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				printf("Sorted the second sparse matrix by rows.\n");
				SortingRowWise(M2, sm2);
			}
			else
			{
				printf("Sorted the second sparse matrix by columns.\n");
				SortingColumnWise(M2, sm2);
			}

			printf("The sorted second sparse matrix will be printed.\n");
			PrintSparseMatrix(M2, sm2);

			AddSparseMatrices(M1, M2, sm1, sm2);

			break;
		case 4:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				printf("The sparse matrix read from file has not been sorted.\n");
				printf("To execute the operation the matrix has to be sorted.\n");
				printf("\n");

				break;
			}

			printf("You have to provide the filename for another sparse matrix for the operation to occur.\n");
			ReadSparseMatrixFromFile(&M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				printf("Sorted the second sparse matrix by rows.\n");
				SortingRowWise(M2, sm2);
			}
			else
			{
				printf("Sorted the second sparse matrix by columns.\n");
				SortingColumnWise(M2, sm2);
			}

			printf("The sorted second sparse matrix will be printed.\n");
			PrintSparseMatrix(M2, sm2);

			SubtractSparseMatrices(M1, M2, sm1, sm2);

			break;
		case 5:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				printf("The sparse matrix read from file has not been sorted.\n");
				printf("To execute the operation the matrix has to be sorted.\n");
				printf("\n");

				break;
			}

			printf("You have to provide the filename for another sparse matrix for the operation to occur.\n");
			ReadSparseMatrixFromFile(&M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				printf("Sorted the second sparse matrix by rows.\n");
				SortingRowWise(M2, sm2);
			}
			else
			{
				printf("Sorted the second sparse matrix by columns.\n");
				SortingColumnWise(M2, sm2);
			}

			printf("The sorted second sparse matrix will be printed.\n");
			PrintSparseMatrix(M2, sm2);

			MultiplySparseMatrices(M1, M2, sm1, sm2);

			break;
		case 0:
			exitFlag = 1;

			break;
		default:
			printf("Invalid input. Try again.\n");
			printf("\n");

			break;
		}

		if (exitFlag)
		{
			printf("You want to exit the program. Exiting program . . .\n");
			break;
		}

		// printf("\n");
	}

	return 0;
}
