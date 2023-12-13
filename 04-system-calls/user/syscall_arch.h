/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SYSCALL_ARCH_H
#define __SYSCALL_ARCH_H

#include <stdint.h>

/*
 * Args in r0-r2, result in r0. Macros because n is encoded in the svc
 * instruction and has to be a constant.
 */
#define syscall3(n, a, b, c) ({						\
	register uint32_t _r0 __asm__("r0") = (uint32_t)(a);		\
	register uint32_t _r1 __asm__("r1") = (uint32_t)(b);		\
	register uint32_t _r2 __asm__("r2") = (uint32_t)(c);		\
	__asm__ volatile("svc %[nr]"					\
			 : "+r"(_r0)					\
			 : [nr] "i"(n), "r"(_r1), "r"(_r2)		\
			 : "memory");					\
	(int32_t)_r0;							\
})
#define syscall2(n, a, b)	syscall3(n, a, b, 0)
#define syscall1(n, a)		syscall3(n, a, 0, 0)
#define syscall0(n)		syscall3(n, 0, 0, 0)

#endif /* __SYSCALL_ARCH_H */
