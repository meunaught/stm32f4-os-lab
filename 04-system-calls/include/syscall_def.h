/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SYSCALL_DEF_H
#define __SYSCALL_DEF_H

/* svc numbers, kept from the course's original (OS/161-style) table */
#define SYS__exit	3
#define SYS_getpid	5
#define SYS_sem_wait	33	/* lesson 06 */
#define SYS_sem_post	34	/* lesson 06 */
#define SYS_read	50
#define SYS_write	55
#define SYS___time	113
#define SYS_reboot	119
#define SYS_yield	120	/* lesson 05 */

/* all three are the console */
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

#endif /* __SYSCALL_DEF_H */
