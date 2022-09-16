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

	std::cout << "This is a program for computing operations between sparse matrices \n";
	std::cout << "with the I/O of said matrices occuring with files given by the user.\n";
	std::cout << "\n";

	// Reading initial sparse matrix
	ReadSparseMatrixFromFile(M1, sm1);
	PrintSparseMatrix(M1, sm1);

	// Choice menu loop
	while (1)
	{
		std::cout << "~~~~ MENU ~~~~\n";
		std::cout << "1) Sort sparse matrix by rows and columns\n";
		std::cout << "2) Sort sparse matrix by columns and rows\n";
		std::cout << "3) Add two sparse matrices\n";
		std::cout << "4) Subtract two sparse matrices\n";
		std::cout << "5) Multiply two sparse matrices\n";
		std::cout << "0) Exit\n";

		std::cout << "Enter your choice (0 - 5): ";
		std::cin >> choice;
		std::cout << "\n";

		switch (choice)
		{
		case 1:
			SortingRowWise(M1, sm1);
			std::cout << "Sorted sparse matrix by rows and columns.\n";
			PrintSparseMatrix(M1, sm1);

			isSorted = 1;
			isRowSorted = 1;

			break;
		case 2:
			SortingColumnWise(M1, sm1);
			std::cout << "Sorted sparse matrix by columns and rows.\n";
			PrintSparseMatrix(M1, sm1);

			isSorted = 1;
			isRowSorted = 0;

			break;
		case 3:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				std::cout << "The sparse matrix read from file has not been sorted.\n";
				std::cout << "To execute the operation the matrix has to be sorted.\n";
				std::cout << "\n";

				break;
			}

			std::cout << "You have to provide the filename for another sparse matrix for the operation to occur.\n";
			ReadSparseMatrixFromFile(M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				std::cout << "Sorted the second sparse matrix by rows.\n";
				SortingRowWise(M2, sm2);
			}
			else
			{
				std::cout << "Sorted the second sparse matrix by columns.\n";
				SortingColumnWise(M2, sm2);
			}

			std::cout << "The sorted second sparse matrix will be printed.\n";
			PrintSparseMatrix(M2, sm2);

			AddSparseMatrices(M1, M2, sm1, sm2);

			break;
		case 4:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				std::cout << "The sparse matrix read from file has not been sorted.\n";
				std::cout << "To execute the operation the matrix has to be sorted.\n";
				std::cout << "\n";

				break;
			}

			std::cout << "You have to provide the filename for another sparse matrix for the operation to occur.\n";
			ReadSparseMatrixFromFile(M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				std::cout << "Sorted the second sparse matrix by rows.\n";
				SortingRowWise(M2, sm2);
			}
			else
			{
				std::cout << "Sorted the second sparse matrix by columns.\n";
				SortingColumnWise(M2, sm2);
			}

			std::cout << "The sorted second sparse matrix will be printed.\n";
			PrintSparseMatrix(M2, sm2);

			SubtractSparseMatrices(M1, M2, sm1, sm2);

			break;
		case 5:
			// Checking whether sparse matrix is sorted
			if (!isSorted)
			{
				std::cout << "The sparse matrix read from file has not been sorted.\n";
				std::cout << "To execute the operation the matrix has to be sorted.\n";
				std::cout << "\n";

				break;
			}

			std::cout << "You have to provide the filename for another sparse matrix for the operation to occur.\n";
			ReadSparseMatrixFromFile(M2, sm2);

			// Sorting second sparse matrix in the same manner
			if (isRowSorted)
			{
				std::cout << "Sorted the second sparse matrix by rows.\n";
				SortingRowWise(M2, sm2);
			}
			else
			{
				std::cout << "Sorted the second sparse matrix by columns.\n";
				SortingColumnWise(M2, sm2);
			}

			std::cout << "The sorted second sparse matrix will be printed.\n";
			PrintSparseMatrix(M2, sm2);

			MultiplySparseMatrices(M1, M2, sm1, sm2);

			break;
		case 0:
			exitFlag = 1;

			break;
		default:
			std::cout << "Invalid input. Try again.\n";
			std::cout << "\n";

			break;
		}

		if (exitFlag)
		{
			std::cout << "You want to exit the program. Exiting program . . .\n";
			break;
		}

		std::cout << "\n";
	}

	return 0;
}
