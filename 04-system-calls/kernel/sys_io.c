/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <kstdio.h>

int32_t sys_write(int fd, const char *buf, uint32_t len)
{
	if (fd != STDOUT_FILENO && fd != STDERR_FILENO)
		return -EBADF;
	if (!access_ok(buf, len))
		return -EFAULT;

	for (uint32_t i = 0; i < len; i++)
		kputc(buf[i]);
	return (int32_t)len;
}

/* returns one line, without the newline; busy-waits for input */
int32_t sys_read(int fd, char *buf, uint32_t len)
{
	if (fd != STDIN_FILENO)
		return -EBADF;
	if (!access_ok(buf, len))
		return -EFAULT;
	if (len == 0)
		return 0;

	return kreadline(buf, (int)len);
}
