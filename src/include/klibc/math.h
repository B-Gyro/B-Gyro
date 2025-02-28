# pragma once
# include "klibc/types.h"
# define PI 3.14159265358979323846
# define ABS(X) ((X) < 0 ? -(X) : (X))


int		floor(double x);
double	cosApproximate(double x);
double	sinApproximate(double x);
size_t  pow(uint32_t base, uint32_t exp);
