#pragma once

#include "types.h"

typedef struct _varg_ptr {
	size_t *ptr;
} varg_ptr;

// inits the varg_ptr;
#define VARG_INIT(V_PTR, FIRST_ARG) V_PTR.ptr = (size_t *)&FIRST_ARG
// get's the next element of type TYPE;
#define VARG_NEXT(V_PTR, TYPE) (*(TYPE *)(++(V_PTR).ptr))
// Special handling for double/float
#define VARG_NEXT_DOUBLE(V_PTR) (*((double *)((V_PTR).ptr += 2, (V_PTR).ptr - 1)))