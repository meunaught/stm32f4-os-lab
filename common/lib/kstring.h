/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __KSTRING_H
#define __KSTRING_H

#include <stddef.h>

/* gcc can emit memset/memcpy calls on its own, even with -nostdlib */
size_t strlen(const char *s);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t n);
void *memset(void *dst, int c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);

#endif /* __KSTRING_H */
