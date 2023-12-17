/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <format.h>
#include <kstring.h>
#include <stdint.h>

/* drops what doesn't fit but keeps counting, like snprintf */
struct out {
	char *buf;
	size_t size;
	size_t len;
};

static void put(struct out *o, char c)
{
	if (o->len + 1 < o->size)
		o->buf[o->len] = c;
	o->len++;
}

/* zero padding goes after a leading '-' */
static void emit(struct out *o, const char *s, size_t len, int width,
		 int left, char pad)
{
	int fill = width > (int)len ? width - (int)len : 0;

	if (left) {
		for (size_t i = 0; i < len; i++)
			put(o, s[i]);
		while (fill--)
			put(o, ' ');
		return;
	}
	if (pad == '0' && len > 0 && s[0] == '-') {
		put(o, '-');
		s++;
		len--;
	}
	while (fill--)
		put(o, pad);
	for (size_t i = 0; i < len; i++)
		put(o, s[i]);
}

static size_t utoa(char *out, unsigned v, unsigned base, int upper)
{
	const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	char rev[32];
	size_t n = 0;

	do {
		rev[n++] = digits[v % base];
		v /= base;
	} while (v);
	for (size_t i = 0; i < n; i++)
		out[i] = rev[n - 1 - i];
	return n;
}

static size_t itoa(char *out, int v)
{
	if (v < 0) {
		out[0] = '-';
		return 1 + utoa(out + 1, 0u - (unsigned)v, 10, 0);
	}
	return utoa(out, (unsigned)v, 10, 0);
}

static size_t ftoa(char *out, double v, int prec)
{
	size_t n = 0;
	double scale = 1.0;

	if (prec > 9)
		prec = 9;
	if (v < 0) {
		out[n++] = '-';
		v = -v;
	}
	for (int i = 0; i < prec; i++)
		scale *= 10.0;
	v += 0.5 / scale;			/* round at the last digit shown */
	if (v > 4294967295.0)
		v = 4294967295.0;		/* integer part must fit 32 bits */

	unsigned whole = (unsigned)v;
	unsigned frac = (unsigned)((v - whole) * scale);

	n += utoa(out + n, whole, 10, 0);
	if (prec > 0) {
		char digits[12];
		size_t k = utoa(digits, frac, 10, 0);

		out[n++] = '.';
		for (size_t i = k; i < (size_t)prec; i++)
			out[n++] = '0';		/* 0.05 -> "05", not "5" */
		for (size_t i = 0; i < k; i++)
			out[n++] = digits[i];
	}
	return n;
}

int vformat(char *buf, size_t size, const char *fmt, va_list ap)
{
	struct out o = { buf, size, 0 };
	char tmp[32];

	for (; *fmt; fmt++) {
		if (*fmt != '%') {
			put(&o, *fmt);
			continue;
		}
		fmt++;

		int left = 0, width = 0, prec = -1;
		char pad = ' ';

		for (;; fmt++) {
			if (*fmt == '-')
				left = 1;
			else if (*fmt == '0')
				pad = '0';
			else
				break;
		}
		while (*fmt >= '0' && *fmt <= '9')
			width = width * 10 + (*fmt++ - '0');
		if (*fmt == '.') {
			prec = 0;
			for (fmt++; *fmt >= '0' && *fmt <= '9'; fmt++)
				prec = prec * 10 + (*fmt - '0');
		}

		const char *s = tmp;
		size_t len;

		switch (*fmt) {
		case 'c':
			tmp[0] = (char)va_arg(ap, int);
			len = 1;
			break;
		case 's':
			s = va_arg(ap, const char *);
			if (!s)
				s = "(null)";
			len = strlen(s);
			if (prec >= 0 && (size_t)prec < len)
				len = (size_t)prec;
			break;
		case 'd':
		case 'i':
			len = itoa(tmp, va_arg(ap, int));
			break;
		case 'u':
			len = utoa(tmp, va_arg(ap, unsigned), 10, 0);
			break;
		case 'x':
		case 'X':
			len = utoa(tmp, va_arg(ap, unsigned), 16, *fmt == 'X');
			break;
		case 'o':
			len = utoa(tmp, va_arg(ap, unsigned), 8, 0);
			break;
		case 'p':
			tmp[0] = '0';
			tmp[1] = 'x';
			len = 2 + utoa(tmp + 2, (unsigned)(uintptr_t)va_arg(ap, void *), 16, 0);
			break;
		case 'f':
			len = ftoa(tmp, va_arg(ap, double), prec < 0 ? 6 : prec);
			break;
		case '%':
			tmp[0] = '%';
			len = 1;
			break;
		case '\0':
			fmt--;		/* lone '%' at the end: stop */
			continue;
		default:		/* unknown conversion: print it as is */
			put(&o, '%');
			put(&o, *fmt);
			continue;
		}
		emit(&o, s, len, width, left, pad);
	}

	if (size > 0)
		buf[o.len < size ? o.len : size - 1] = '\0';
	return (int)o.len;
}

int format(char *buf, size_t size, const char *fmt, ...)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vformat(buf, size, fmt, ap);
	va_end(ap);
	return n;
}

static int is_blank(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static int digit_value(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

int vscan(const char *in, const char *fmt, va_list ap)
{
	int converted = 0;

	for (; *fmt; fmt++) {
		if (is_blank(*fmt)) {
			while (is_blank(*in))
				in++;
			continue;
		}
		if (*fmt != '%') {		/* literal text must match */
			if (*in != *fmt)
				break;
			in++;
			continue;
		}

		fmt++;
		if (*fmt != 'c')
			while (is_blank(*in))
				in++;
		if (*in == '\0')
			break;

		switch (*fmt) {
		case 'd':
		case 'u':
		case 'x': {
			unsigned base = (*fmt == 'x') ? 16 : 10;
			const char *start;
			unsigned v = 0;
			int neg = 0, d;

			if (*fmt == 'd' && (*in == '-' || *in == '+'))
				neg = (*in++ == '-');
			start = in;
			while ((d = digit_value(*in)) >= 0 && (unsigned)d < base) {
				v = v * base + (unsigned)d;
				in++;
			}
			if (in == start)
				return converted;	/* not a number */
			if (*fmt == 'd')
				*va_arg(ap, int *) = neg ? -(int)v : (int)v;
			else
				*va_arg(ap, unsigned *) = v;
			break;
		}
		case 's': {
			char *dst = va_arg(ap, char *);

			while (*in && !is_blank(*in))
				*dst++ = *in++;
			*dst = '\0';
			break;
		}
		case 'c':
			*va_arg(ap, char *) = *in++;
			break;
		default:
			return converted;
		}
		converted++;
	}
	return converted;
}

int scan(const char *str, const char *fmt, ...)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vscan(str, fmt, ap);
	va_end(ap);
	return n;
}
