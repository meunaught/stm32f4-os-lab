/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __FORMAT_H
#define __FORMAT_H

#include <stdarg.h>
#include <stddef.h>

/*
 * Shared by kprintf and the user-space printf.
 * vformat: %c %s %d %i %u %x %X %o %p %f %%, '-'/'0' flags, width, precision.
 * vscan: %d %u %x %s %c.
 */
int vformat(char *buf, size_t size, const char *fmt, va_list ap);
int format(char *buf, size_t size, const char *fmt, ...);
int vscan(const char *str, const char *fmt, va_list ap);
int scan(const char *str, const char *fmt, ...);

#endif /* __FORMAT_H */
