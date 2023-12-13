/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <stdint.h>
#include <stm32_peps.h>

int32_t syscall(uint32_t number, uint32_t arg0, uint32_t arg1, uint32_t arg2);

int32_t sys_read(int fd, char *buf, uint32_t len);
int32_t sys_write(int fd, const char *buf, uint32_t len);
int32_t sys_getpid(void);
int32_t sys_exit(int status);
int32_t sys_uptime(void);
int32_t sys_reboot(void);

/* no MPU, so at least check user buffers are in SRAM or flash */
static inline int access_ok(const void *ptr, uint32_t len)
{
	uint32_t start = (uint32_t)ptr;
	uint32_t end = start + len;

	if (end < start)	/* wrapped */
		return 0;
	return (start >= SRAM_START && end <= SRAM_END) ||
	       (start >= FLASH_START && end <= FLASH_END);
}

#endif /* __SYSCALL_H */
