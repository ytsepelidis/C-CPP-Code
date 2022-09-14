#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void DerivativeComputation(const int n, double x, const double step, double* fValues, double* dfValues);
double IntegralComputation(const int n, double x, const double step, double* fValues, double* dfValues);