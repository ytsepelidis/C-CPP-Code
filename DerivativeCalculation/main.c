#include "functions.h"

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
