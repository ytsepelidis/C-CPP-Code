#include "functions.h"

double Function(double x)
{
	if (x < -10.0)
	{
		return (8 * (x * x)) + x + 3;
	}
	else if (x >= -10.0 && x <= 10.0)
	{
		if (x >= (double)-7 / 80)
		{
			return (80 * x) + 7;
		}
		else
		{
			return -((80 * x) + 7);
		}
	}
	else
	{
		return (x * x * x) - (2 * (x * x)) + 7;
	}
}

double FunctionDerivativeApprox(double x, double y)
{
	double x1 = x + a;
	double y1 = Function(x1);

	double lim = ((y1 - y) / a);

	return lim;
}

double FunctionDerivativePrecise(double x)
{
	if (x < -10.0)
	{
		return (16 * x) + 1;
	}
	else if (x > -10 && x < (double)-7 / 80)
	{
		return -80.0;
	}
	else if (x > (double)-7 / 80 && x < 10)
	{
		return 80.0;
	}
	else
	{
		return 3 * (x * x) - 4 * x;
	}
}