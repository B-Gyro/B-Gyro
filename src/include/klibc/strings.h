# pragma once
# include "klibc/types.h"

size_t	strlen(const char *str);
size_t	strlenIgnoreAnsii(const char *str);

size_t  strlcpy(char *dest, const char *src, size_t size);
size_t  strlcat(char *dest, const char *src, size_t size);

char	*strchr(const char *s, int c);
char	*strrchr(const char *s, int c);
int32_t	strchrInd(const char *s, int c);

int	strcmp(const char *s1, const char *s2);
int	strncmp(const char *s1, const char *s2, size_t n);

char	*strnstr(const char *big, const char *little, size_t len);
char	*strtok(char *str, const char *delim);