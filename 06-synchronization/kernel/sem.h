/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SEM_H
#define __SEM_H

#include <stdint.h>

#define MAX_SEMAPHORES	8

int ksem_create(int initial);

int32_t sys_sem_wait(int id);
int32_t sys_sem_post(int id);

#endif /* __SEM_H */
