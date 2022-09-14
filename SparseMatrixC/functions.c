#include "functions.h"

void ReadSparseMatrixFromFile(int* M, SparseMatrix sm)
{
	int i;
	FILE* f;
	char filename_r[64];

	// Reading filename, checking if it exists and opening said file
	while (1)
	{
		printf("Please enter the file you want to read the sparse matrix from (with .txt extension): ");
		scanf("%s", filename_r);

		f = fopen(filename_r, "r");

		if (!f)
		{
			printf("File is not in the current directory.\n");
		}
		else
		{
			printf("File '%s' has been opened.\n", filename_r);
			break;
		}
	}

	// Reading number M of sparse matrix elements
	fscanf(f, "%d", M);

	// Checking number of sparse matrix elements
	if (*M > (M_max / 2))
	{
		printf("Number of sparse matrix elements greater than (M_max / 2).");
		printf("Program will not be able to execute matrix operations due to size limitations. Exiting program . . .\n");

		// Closing file
		fclose(f);

		exit(1);
	}

	// Reading rows, columns and values for the sparse matrix elements
	for (i = 0; i < *M; i++)
	{
		fscanf(f, "%d %d %lf", &sm[i].row, &sm[i].col, &sm[i].val);
	}

	printf("Sparse matrix was read from file '%s'.\n", filename_r);
	printf("\n");

	// Closing file
	fclose(f);
}

void WriteSparseMatrixToFile(const int M, const SparseMatrix sm)
{
	int i;
	FILE* f;
	char filename_w[64];

	// Reading filename, checking if it exists and opening said file
	while (1)
	{
		printf("Please enter a filename you want to write the sparse matrix to (with .txt extension): ");
		scanf("%s", filename_w);

		f = fopen(filename_w, "w");

		if (!f)
		{
			printf("Unable to open file. Exiting program . . .\n");
			exit(1);
		}
		else
		{
			printf("File '%s' has been opened.\n", filename_w);
			break;
		}
	}

	// Writing sparse matrix elements to the .txt file
	fprintf(f, "%d\n", M);

	for (i = 0; i < M; i++)
	{
		fprintf(f, "%d\n", sm[i].row);
		fprintf(f, "%d\n", sm[i].col);
		fprintf(f, "%lf\n", sm[i].val);
	}

	printf("Sparse matrix was written to file '%s'.\n", filename_w);
	printf("\n");

	// Closing file
	fclose(f);
}

void SortingRowWise(const int M, SparseMatrix sm)
{
	SparseElement temp;
	int i, j;
	int found;

	for (i = 1; i < M; i++)
	{
		found = 0;

		for (j = (M - 1); j >= i; j--)
		{
			if (sm[j].row < sm[j - 1].row)			// Row check
			{
				// Row
				temp.row = sm[j].row;
				sm[j].row = sm[j - 1].row;
				sm[j - 1].row = temp.row;

				// Column
				temp.col = sm[j].col;
				sm[j].col = sm[j - 1].col;
				sm[j - 1].col = temp.col;

				// Value
				temp.val = sm[j].val;
				sm[j].val = sm[j - 1].val;
				sm[j - 1].val = temp.val;

				found = 1;
			}
			else if (sm[j].row == sm[j - 1].row)
			{
				if (sm[j].col < sm[j - 1].col)		// Column check
				{
					// Row
					temp.row = sm[j].row;
					sm[j].row = sm[j - 1].row;
					sm[j - 1].row = temp.row;

					// Column
					temp.col = sm[j].col;
					sm[j].col = sm[j - 1].col;
					sm[j - 1].col = temp.col;

					// Value
					temp.val = sm[j].val;
					sm[j].val = sm[j - 1].val;
					sm[j - 1].val = temp.val;

					found = 1;
				}
			}
		}

		if (!found)
		{
			break;
		}
	}
}

void SortingColumnWise(int M, SparseMatrix sm)
{
	SparseElement temp;
	int i, j;
	int found;

	for (i = 1; i < M; i++)
	{
		found = 0;

		for (j = (M - 1); j >= i; j--)
		{
			if (sm[j].col <= sm[j - 1].col)			// Column check
			{
				// Row
				temp.row = sm[j].row;
				sm[j].row = sm[j - 1].row;
				sm[j - 1].row = temp.row;

				// Column
				temp.col = sm[j].col;
				sm[j].col = sm[j - 1].col;
				sm[j - 1].col = temp.col;

				// Value
				temp.val = sm[j].val;
				sm[j].val = sm[j - 1].val;
				sm[j - 1].val = temp.val;

				found = 1;
			}
			else if (sm[j].col == sm[j - 1].col)
			{
				if (sm[j].row < sm[j - 1].row)		// Row check
				{
					// Row
					temp.row = sm[j].row;
					sm[j].row = sm[j - 1].row;
					sm[j - 1].row = temp.row;

					// Column
					temp.col = sm[j].col;
					sm[j].col = sm[j - 1].col;
					sm[j - 1].col = temp.col;

					// Value
					temp.val = sm[j].val;
					sm[j].val = sm[j - 1].val;
					sm[j - 1].val = temp.val;

					found = 1;
				}
			}
		}

		if (!found)
		{
			break;
		}
	}
}

void AddSparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2)
{
	int i, j;
	int M = 0;
	int foundCommon = 0;
	SparseMatrix smRes;

	// Inserting first sparse matrix and adding common elements
	for (i = 0; i < M1; i++)
	{
		foundCommon = 0;

		for (j = 0; j < M2; j++)
		{
			if (sm1[i].row == sm2[j].row && sm1[i].col == sm2[j].col)
			{
				smRes[M].row = sm1[i].row;
				smRes[M].col = sm1[i].col;
				smRes[M].val = sm1[i].val + sm2[j].val;

				M += 1;
				foundCommon = 1;

				break;
			}
		}

		if (!foundCommon)
		{
			smRes[M].row = sm1[i].row;
			smRes[M].col = sm1[i].col;
			smRes[M].val = sm1[i].val;

			M += 1;
		}
	}

	// Inserting elements from the second sparse matrix 
	for (i = 0; i < M2; i++)
	{
		foundCommon = 0;

		for (j = 0; j < M1; j++)
		{
			if (sm2[i].row == sm1[j].row && sm2[i].col == sm1[j].col)
			{
				foundCommon = 1;
				break;
			}
		}

		if (!foundCommon)
		{
			smRes[M].row = sm2[i].row;
			smRes[M].col = sm2[i].col;
			smRes[M].val = sm2[i].val;

			M += 1;
		}
	}

	printf("The resulting sparse matrix will be printed.\n");
	PrintSparseMatrix(M, smRes);

	printf("You are about to write the resulting addition sparse matrix to a file.\n");
	WriteSparseMatrixToFile(M, smRes);
}

void SubtractSparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2)
{
	int i, j;
	int M = 0;
	int foundCommon = 0;
	SparseMatrix smRes;

	// Inserting first sparse matrix and subtracting common elements
	for (i = 0; i < M1; i++)
	{
		foundCommon = 0;

		for (j = 0; j < M2; j++)
		{
			if (sm1[i].row == sm2[j].row && sm1[i].col == sm2[j].col)
			{
				smRes[M].row = sm1[i].row;
				smRes[M].col = sm1[i].col;
				smRes[M].val = sm1[i].val - sm2[j].val;

				M += 1;
				foundCommon = 1;

				break;
			}
		}

		if (!foundCommon)
		{
			smRes[M].row = sm1[i].row;
			smRes[M].col = sm1[i].col;
			smRes[M].val = sm1[i].val;

			M += 1;
		}
	}

	// Inserting elements from the second sparse matrix with a minus sign
	for (i = 0; i < M2; i++)
	{
		foundCommon = 0;

		for (j = 0; j < M1; j++)
		{
			if (sm2[i].row == sm1[j].row && sm2[i].col == sm1[j].col)
			{
				foundCommon = 1;
				break;
			}
		}

		if (!foundCommon)
		{
			smRes[M].row = sm2[i].row;
			smRes[M].col = sm2[i].col;
			smRes[M].val = -sm2[i].val;

			M += 1;
		}
	}

	printf("The resulting sparse matrix will be printed.\n");
	PrintSparseMatrix(M, smRes);

	printf("You are about to write the resulting subtraction sparse matrix to a file.\n");
	WriteSparseMatrixToFile(M, smRes);
}

void MultiplySparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2)
{
	int i, j;
	int M = 0;
	SparseMatrix smRes;

	// Only the common elements will be multiplied and inserted
	// since multiplying by 0 gives 0
	for (i = 0; i < M1; i++)
	{
		for (j = 0; j < M2; j++)
		{
			if (sm1[i].row == sm2[j].row && sm1[i].col == sm2[j].col)
			{
				smRes[M].row = sm1[i].row;
				smRes[M].col = sm1[i].col;
				smRes[M].val = sm1[i].val * sm2[j].val;

				M += 1;

				break;
			}
		}
	}

	printf("The resulting sparse matrix will be printed.\n");
	PrintSparseMatrix(M, smRes);

	printf("You are about to write the resulting multiplication sparse matrix to a file.\n");
	WriteSparseMatrixToFile(M, smRes);
}

void PrintSparseMatrix(const int M, const SparseMatrix sm)
{
	int i;

	printf("~ Printing sparse matrix ~ \n");
	for (i = 0; i < M; i++)
	{
		printf("(%d, %d): %lf\n", sm[i].row, sm[i].col, sm[i].val);
	}

	printf("\n");
}
