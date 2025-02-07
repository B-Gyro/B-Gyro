#include "klibc/memory.h"
#include "klibc/strings.h"

size_t strlenIgnoreAnsii(const char *str){
    size_t i;

	i = 0;
    if (!str)
        return 0;
    while (str[i]){
        if (str[i] == '\033')
            str += strchr(str, 'm') - str;
        else
            i++;
    }
    return i;
}

size_t strlen(const char *str){
    size_t i;

	i = 0;
    if (!str)
        return (0);
    while (str[i])
        i++;
    return i;
}

size_t	strlcpy(char *dest, const char *src, size_t size){
    while (size > 0 && *src) {
        *dest = *src;
        dest++, src++;
        size--;
    }
    *dest = 0;
    return strlen(src);
}

size_t	strlcat(char *dest, const char *src, size_t size){
	size_t	destLen, srcLen, copyLen;

    destLen = strlen(dest);
    srcLen = strlen(src);

    if (destLen + 1 >= size) // Not enough space in dst
        return destLen + srcLen;

    copyLen = size - destLen - 1;
    if (copyLen > srcLen)
        copyLen = srcLen;

    memcpy(dest + destLen, src, copyLen);
    dest[destLen + copyLen] = '\0';

    return destLen + srcLen;
}

int32_t	strchrInd(const char *s, int c){
	int32_t i;

	i = 0;
    if(!s)
        return NULL;
    while (s[i]){
        if (s[i] == (char)c)
            return i;
		i++;
	}
    return -1;
}

char *strchr(const char *s, int c){
    if (!s)
        return NULL;
    while (*s++)
        if (*s == (char)c)
            break;
    return (char *)s;
}

char *strrchr(const char *s, int c){
    size_t sLen;

	sLen = strlen(s);
    if (!s)
        return NULL;
    if ((char) c == '\0')
		return (char *)(s + sLen);
    while (sLen > 0) {
        if (s[sLen - 1] == (char)c)
            return (char *)(s + sLen);
        sLen--;
    }
    return NULL;
}
# include "klibc/print.h"

int strncmp(const char *s1, const char *s2, size_t n){
    if ((!s1 && s2) || (s1 && !s2))
        return (1);
    if (!s1 && !s2)
        return (0);
    while (--n && *s1 && *s2 && (*s1 == *s2))
        s1++, s2++;
    return *s1 - *s2;
}

int strcmp(const char *s1, const char *s2){
    if ((!s1 && s2) || (s1 && !s2))
        return (1);
    if (!s1 && !s2)
        return (0);
    while (*s1 && *s2 && (*s1 == *s2))
        s1++, s2++;
    return *s1 - *s2;
}

char *strstr(const char *haystack, const char *needle){
    if (!haystack || !needle)
        return NULL;
    if (!*needle)
        return (char *)haystack;
    size_t needleLen = strlen(needle);
    while (*haystack) {
        if (!strncmp(haystack, needle, needleLen))
            return (char *)haystack;
        haystack++;
    }
    return NULL;
}

char *strnstr(const char *haystack, const char *needle, size_t len){
    if (!haystack || !needle)
        return NULL;
    if (!*needle)
        return (char *)haystack;
    size_t needleLen = strlen(needle);
    size_t i = 0;
    while (haystack[i] && i < len) {
        if (!strncmp(haystack + i, needle, needleLen))
            return (char *)haystack + i;
        i++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim){
    static char *last;
	char		*delimFound;

    if (!str)
        str = last;
    if (!str && !last)
        return NULL;

    delimFound = strstr(str, delim);
    if (!delimFound)
        last = NULL;
    else
        *delimFound = 0, last = delimFound + strlen(delim);

    return str;
}