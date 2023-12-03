/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __KSTDIO_H
#define __KSTDIO_H

#include <stdint.h>

/* console on USART2 */
void kputc(char c);
void kputs(const char *s);
int kprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

char kgetc(void);
int ktrygetc(void);
int kreadline(char *buf, int size);
int kscanf(const char *fmt, ...);

#endif /* __KSTDIO_H */
