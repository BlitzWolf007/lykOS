#pragma once
#include <def.h>

char *itoa(int value, char *str, int base);

char *ulltoa(unsigned long long int value, char *str, int base);

size_t strlen(const char *s);

char *strcpy(char *destination, const char *source);

char *strcat(char *destination, const char *source);

char *strncat(char *destination, const char *source, size_t num);

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

void *memmove(void *dest, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);