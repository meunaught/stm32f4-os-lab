/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <unistd.h>
#include <syscall_arch.h>

int read(int fd, void *buf, uint32_t len)
{
	return syscall3(SYS_read, fd, buf, len);
}

int write(int fd, const void *buf, uint32_t len)
{
	return syscall3(SYS_write, fd, buf, len);
}

int getpid(void)
{
	return syscall0(SYS_getpid);
}

uint32_t uptime(void)
{
	return (uint32_t)syscall0(SYS___time);
}

void exit(int status)
{
	syscall1(SYS__exit, status);
	for (;;)
		;
}

void reboot(void)
{
	syscall0(SYS_reboot);
	for (;;)
		;
}
