# pragma once
# include "klibc/types.h"
# define PI 3.14159265358979323846
# define ABS(X) ((X) < 0 ? -(X) : (X))

typedef struct vec3{
	double x;
	double y;
	double z;
}_vec3;

typedef struct vec2{
	double x;
	double y;
} _vec2;

int		floor(double x);
double	cosApproximate(double x);
double	sinApproximate(double x);
double	tanApproximate(double x);
size_t  pow(uint32_t base, uint32_t exp);
