/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __UNISTD_H
#define __UNISTD_H

#include <stdint.h>
#include <syscall_def.h>

int read(int fd, void *buf, uint32_t len);
int write(int fd, const void *buf, uint32_t len);
int getpid(void);
uint32_t uptime(void);			/* ms */
void exit(int status) __attribute__((noreturn));
void reboot(void) __attribute__((noreturn));

#endif /* __UNISTD_H */
