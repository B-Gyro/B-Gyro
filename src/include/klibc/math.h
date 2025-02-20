# pragma once
# include "klibc/types.h"
# define PI 3.14159265358979323846
# define ABS(X) ((X) < 0 ? -(X) : (X))


double degToRad(double degrees);
double cosApproximate(double x);
double sinApproximate(double x);
double power(double base, int exp);
size_t factorial(int n);
