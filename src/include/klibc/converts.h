# pragma once
#include "klibc/types.h"

ssize_t atoiS(const char *s, size_t *index);
size_t	uatoiS(const char *s, size_t *index);
size_t	aHextoiS(const char *s, size_t *index);
size_t	uatoi(const char *s, size_t *index);
// ssize_t atoi(const char *s);