/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>

int32_t syscall(uint32_t number, uint32_t arg0, uint32_t arg1, uint32_t arg2)
{
	switch (number) {
	case SYS_read:		return sys_read((int)arg0, (char *)arg1, arg2);
	case SYS_write:		return sys_write((int)arg0, (const char *)arg1, arg2);
	case SYS_getpid:	return sys_getpid();
	case SYS__exit:		return sys_exit((int)arg0);
	case SYS___time:	return sys_uptime();
	case SYS_reboot:	return sys_reboot();
	default:		return -ENOSYS;
	}
}
