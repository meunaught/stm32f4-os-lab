/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __STDIO_H
#define __STDIO_H

/* formatting happens in user space; only the text goes through write() */
int printf(const char *fmt, ...);
int puts(const char *s);
int readline(char *buf, int size);
int scanf(const char *fmt, ...);

#endif /* __STDIO_H */
