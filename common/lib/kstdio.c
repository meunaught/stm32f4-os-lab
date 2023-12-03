/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <kstdio.h>
#include <format.h>
#include <usart.h>

#define CONSOLE		USART2
#define KPRINTF_MAX	256
#define KSCANF_LINE	80

void kputc(char c)
{
	if (c == '\n')
		usart_putc(CONSOLE, '\r');	/* terminals expect CR LF */
	usart_putc(CONSOLE, c);
}

void kputs(const char *s)
{
	while (*s)
		kputc(*s++);
}

int kprintf(const char *fmt, ...)
{
	char buf[KPRINTF_MAX];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vformat(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	kputs(buf);
	return n;
}

char kgetc(void)
{
	return usart_getc(CONSOLE);
}

int ktrygetc(void)
{
	return usart_try_getc(CONSOLE);
}

/* CR, LF and CR LF each end the line once */
int kreadline(char *buf, int size)
{
	static char prev;
	int n = 0;

	if (size <= 0)
		return 0;

	for (;;) {
		char c = kgetc();

		if (c == '\n' && prev == '\r') {	/* second half of CR LF */
			prev = c;
			continue;
		}
		prev = c;

		if (c == '\r' || c == '\n')
			break;
		if (c == '\b' || c == 0x7F) {		/* backspace / delete */
			if (n > 0) {
				n--;
				kputs("\b \b");
			}
			continue;
		}
		if (c >= ' ' && n < size - 1) {
			buf[n++] = c;
			kputc(c);			/* echo */
		}
	}
	kputc('\n');
	buf[n] = '\0';
	return n;
}

int kscanf(const char *fmt, ...)
{
	char line[KSCANF_LINE];
	va_list ap;
	int n;

	kreadline(line, sizeof(line));
	va_start(ap, fmt);
	n = vscan(line, fmt, ap);
	va_end(ap);
	return n;
}
