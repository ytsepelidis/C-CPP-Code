#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void DerivativeComputation(const int n, double x, const double step, double* fValues, double* dfValues);
double IntegralComputation(const int n, double x, const double step, double* fValues, double* dfValues);

int main()
{
	int i;
	int n;
	double x;
	double step;
	double* fValues;
	double* dfValues;
	FILE* f;
	char filename[128];
	double result;

	// Reading filename, checking if it exists and opening said file
	while (1)
	{
		printf("Please enter the file you want to open (with .txt extension): ");
		scanf("%s", filename);

		f = fopen(filename, "r");

		if (!f)
		{
			printf("File is not in the current directory.\n");
		}
		else
		{
			printf("File '%s' has been opened.\n", filename);
			break;
		}
	}

	// Reading first three values from file
	fscanf(f, "%d %lf %lf", &n, &x, &step);
	printf("n: %d\n", n);
	printf("x: %lf\n", x);
	printf("step: %lf\n", step);

	// Allocating memory for f(x) and f'(x) arrays
	// and checking if allocation was successful
	fValues = (double*)malloc(sizeof(double) * n);
	if (!fValues)
	{
		printf("Memory allocation was unsuccessful. Exiting program . . .\n");
		exit(0);
	}

	dfValues = (double*)malloc(sizeof(double) * n);
	if (!dfValues)
	{
		printf("Memory allocation was unsuccessful. Exiting program . . .\n");
		exit(0);
	}

	// Calculating f(x) values and closing file
	for (i = 0; i < n; i++)
	{
		fscanf(f, "%lf", &fValues[i]);
	}

	fclose(f);

	// Calculating curve length
	result = IntegralComputation(n, x, step, fValues, dfValues);
	printf("The length of the curve is: %lf\n", result);

	// Deallocating memory
	free(fValues);
	free(dfValues);

	return 0;
}

void DerivativeComputation(const int n, double x, const double step, double* fValues, double* dfValues)
{
	int i;
	double numerator;
	double denominator = (x + step) - x;

	for (i = 0; i < (n - 1); i++)
	{
		numerator = fValues[i + 1] - fValues[i];
		dfValues[i] = numerator / denominator;

		// x += step;
	}
}

double IntegralComputation(const int n, double x, const double step, double* fValues, double* dfValues)
{
	int i;
	double multStep = (x + step) - x;
	double result = 0;

	DerivativeComputation(n, x, step, fValues, dfValues);

	for (i = 0; i < (n - 1); i++)
	{
		result += sqrt(1 + dfValues[i] * dfValues[i]) * multStep;

		// x += step;
	}

	return result;
}
