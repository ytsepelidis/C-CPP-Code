#pragma once
#include <stdio.h>
#include <stdlib.h>

double Function(double x);
double FunctionDerivativeApprox(double x, double y);
double FunctionDerivativePrecise(double x);

static const double a = 0.0001;