/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <stdio.h>
#include <unistd.h>
#include <format.h>
#include <kstring.h>

#define PRINTF_MAX	128
#define SCANF_LINE	80

int printf(const char *fmt, ...)
{
	char buf[PRINTF_MAX];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vformat(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	write(STDOUT_FILENO, buf, strlen(buf));
	return n;
}

int puts(const char *s)
{
	write(STDOUT_FILENO, s, strlen(s));
	return write(STDOUT_FILENO, "\n", 1);
}

int readline(char *buf, int size)
{
	int n = read(STDIN_FILENO, buf, (uint32_t)size);

	if (n < 0)
		n = 0;
	buf[n < size ? n : size - 1] = '\0';
	return n;
}

int scanf(const char *fmt, ...)
{
	char line[SCANF_LINE];
	va_list ap;
	int n;

	readline(line, sizeof(line));
	va_start(ap, fmt);
	n = vscan(line, fmt, ap);
	va_end(ap);
	return n;
}
