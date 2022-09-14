#include <stdio.h>
#include <stdlib.h>

double Function(double x);
double FunctionDerivativeApprox(double x, double y);
double FunctionDerivativePrecise(double x);

const double a = 0.0001;

int main()
{
	double x, y;

	printf("Enter a value for x: ");
	scanf_s("%lf", &x);
	printf("x = %.2lf\n\n", x);

	y = Function(x);
	printf("Function f(x):\n");
	printf("f(%.2lf) = %lf\n\n", x, y);

	if (x == -10.0 || x == 10.0 || x == (double)-7 / 80)
	{
		printf("The function's derivative is undefined for x = -10 or -0.0875 or 10.\n");
		printf("The program, therefore, will not calculate the function's derivative and will end now.\n");

		return 0;
	}

	double lim = FunctionDerivativeApprox(x, y);
	printf("Approximate f'(x) Derivative:\n");
	printf("f'(%.2lf) = %.2lf\n\n", x, lim);

	double df = FunctionDerivativePrecise(x);
	printf("Precise f'(x) Derivative:\n");
	printf("f'(%.2lf) = %.2lf\n\n", x, df);

	return 0;
}

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
