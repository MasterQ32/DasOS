#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "config.h"

#if defined(__cplusplus)
extern "C"  {
#endif

char *itoa(int value, char *str, int base);
int atoi(const char *str);
// float atof(const char *str);

static inline void *memset(void *ptr, int value, size_t num)
{
	uint8_t *it = (uint8_t*)ptr;
	while((num--) > 0)
	{
		*(it++) = (uint8_t)(value & 0xFF);
	}
	return ptr;
}

static inline void *memcpy(void *destination, const void *source, size_t num)
{
	uint8_t *to = (uint8_t*)destination;
	uint8_t *from = (uint8_t*)source;
	while((num--) > 0)
	{
		*(to++) = *(from++);
	}
	return destination;
}

void *memmove(void *destination, const void *source, size_t num);

static inline int memcmp(const void *s1, const void *s2, size_t n)
{
	const uint8_t * p1 = (const uint8_t *)s1;
	const uint8_t * p2 = (const uint8_t *)s2;
	for ( ; n-- ; p1++, p2++) {
		uint8_t u1 = *p1;
		uint8_t u2 = *p2;
		if ( u1 != u2) {
			return (u1-u2);
		}
	}
	return 0;
}

static inline char *strcpy(char *destination, const char *source)
{
	char *dst = destination;
	while(*source)
	{
		*(dst++) = *(source++);
	}
	*(dst++) = 0;
	return dst;
}

static inline char *strcat(char *destination, const char *source)
{
	char *it = destination;
	while(*it++);
	it--;
	while(*source)
	{
			*it++ = *source++;
	}
	*it = 0;
	return destination;
}

static inline size_t strlen(const char *str)
{
	size_t size = 0;
	while(*(str++) != 0) size++;
	return size;
}

static inline int strcmp(const char *p1, const char *p2)
{
	while(*p1) {
		if(*p2 == 0) {
			return 1;
		}

		if(*p2 > *p1) {
			return -1;
		}
		if(*p1 > *p2) {
			return 1;
		}
		p1++;
		p2++;
	}
	if (*p2) {
		return -1;
	}
	return 0;
}

int sprintf(char *target, const char *format, ...);

int vsprintf(char *target, const char *format, va_list vl);

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void free(void *mem);
void *realloc(void *mem, size_t size);

#if defined(__cplusplus)
}
#endif
