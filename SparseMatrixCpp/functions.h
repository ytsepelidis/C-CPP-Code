#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>

constexpr auto M_MAX = 50;

typedef struct
{
	int row;
	int col;
	double val;
} SparseElement;

typedef SparseElement SparseMatrix[M_MAX];

void ReadSparseMatrixFromFile(int& M, SparseMatrix sm);
void WriteSparseMatrixToFile(const int M, const SparseMatrix sm);
void SortingRowWise(const int M, SparseMatrix sm);
void SortingColumnWise(const int M, SparseMatrix sm);
void AddSparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2);
void SubtractSparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const SparseMatrix sm2);
void MultiplySparseMatrices(const int M1, const int M2, const SparseMatrix sm1, const  SparseMatrix sm2);
void PrintSparseMatrix(const int M, const SparseMatrix sm);
