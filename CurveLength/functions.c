#include "functions.h"

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