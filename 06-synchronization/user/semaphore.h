/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

/* ids come from the kernel; -EINVAL for a bad id */
int sem_wait(int sem);
int sem_post(int sem);

#endif /* __SEMAPHORE_H */
