#include "functions.h"

void ReadSparseMatrixFromFile(int& M, SparseMatrix sm)
{
	std::fstream file;
	std::string filename;

	// Reading filename
	std::cout << "Please enter the file you want to read the sparse matrix from (without the .txt extension): ";
	std::cin >> filename;
	filename += ".txt";
			
	// Opening file in read mode
	file.open(filename, std::ios::in);

	if (!file.is_open())
	{
		std::cout << "Unable to open file. Exiting program . . .\n";
		exit(1);
	}

	// Reading input file line by line
	std::string line;

	// Reading number M of sparse matrix elements
	getline(file, line);
	M = stoi(line);

	// Checking number of sparse matrix elements
	if (M > (M_MAX / 2))
	{
		std::cout << "Number of sparse matrix elements greater than " << (M_MAX / 2) << ".";
		std::cout << "Program will not be able to execute matrix operations due to size limitations. Exiting program . . .\n";

		// Closing file
		file.close();

		exit(1);
	}

	// Reading rows, columns and values for the sparse matrix elements
	for (int i = 0; i < M; i++)
	{
		// Fetching sparse matrix row
		getline(file, line);
		sm[i].row = stoi(line);

		// Fetching sparse matrix collumn
		getline(file, line);
		sm[i].col = stoi(line);

		// Fetching sparse matrix value
		getline(file, line);
		sm[i].val= stoi(line);
	}

	std::cout << "Sparse matrix was read from file '" << filename << "'.\n";
	std::cout << "\n";

	// Closing file
	file.close();
}

void WriteSparseMatrixToFile(const int M, const SparseMatrix sm)
{
	std::fstream file;
	std::string filename;

	// Reading filename
	std::cout << "Please enter the file you want to write the sparse matrix to (without the .txt extension): ";
	std::cin >> filename;
	filename += ".txt";

	// Opening file in write mode
	file.open(filename, std::ios::out);

	if (!file.is_open())
	{
		std::cout << "Unable to open file. Exiting program . . .\n";
		exit(1);
	}

	// Writing sparse matrix elements to the .txt file
	file << M << '\n';

	for (int i = 0; i < M; i++)
	{
		file << sm[i].row << '\n';
		file << sm[i].col << '\n';
		file << sm[i].val << '\n';
	}

	std::cout << "Sparse matrix was written to file '" << filename << "'.\n";
	std::cout << "\n";

	// Closing file
	file.close();
}

void SortingRowWise(const int M, SparseMatrix sm)
{
	SparseElement temp;
	int found;

	for (int i = 1; i < M; i++)
	{
		found = 0;

		for (int j = (M - 1); j >= i; j--)
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
	int found;

	for (int i = 1; i < M; i++)
	{
		found = 0;

		for (int j = (M - 1); j >= i; j--)
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
	int M = 0;
	int foundCommon = 0;
	SparseMatrix smRes;

	// Inserting first sparse matrix and adding common elements
	for (int i = 0; i < M1; i++)
	{
		foundCommon = 0;

		for (int j = 0; j < M2; j++)
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
	for (int i = 0; i < M2; i++)
	{
		foundCommon = 0;

		for (int j = 0; j < M1; j++)
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

	std::cout << "The resulting sparse matrix will be printed.\n";
	PrintSparseMatrix(M, smRes);

	std::cout << "You are about to write the resulting addition sparse matrix to a file.\n";
	WriteSparseMatrixToFile(M, smRes);
}

void SubtractSparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2)
{
	int M = 0;
	int foundCommon = 0;
	SparseMatrix smRes;

	// Inserting first sparse matrix and subtracting common elements
	for (int i = 0; i < M1; i++)
	{
		foundCommon = 0;

		for (int j = 0; j < M2; j++)
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
	for (int i = 0; i < M2; i++)
	{
		foundCommon = 0;

		for (int j = 0; j < M1; j++)
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

	std::cout << "The resulting sparse matrix will be printed.\n";
	PrintSparseMatrix(M, smRes);

	std::cout << "You are about to write the resulting subtraction sparse matrix to a file.\n";
	WriteSparseMatrixToFile(M, smRes);
}

void MultiplySparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2)
{
	int M = 0;
	SparseMatrix smRes;

	// Only the common elements will be multiplied and inserted
	// since multiplying by 0 gives 0
	for (int i = 0; i < M1; i++)
	{
		for (int j = 0; j < M2; j++)
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

	std::cout << "The resulting sparse matrix will be printed.\n";
	PrintSparseMatrix(M, smRes);

	std::cout << "You are about to write the resulting multiplication sparse matrix to a file.\n";
	WriteSparseMatrixToFile(M, smRes);
}

void PrintSparseMatrix(const int M, const SparseMatrix sm)
{
	std::cout << "~ Printing sparse matrix ~\n";
	for (int i = 0; i < M; i++)
	{
		std::cout << "(" << sm[i].row << ", " << sm[i].col << "): " << sm[i].val << "\n";
	}

	std::cout << "\n";
}
