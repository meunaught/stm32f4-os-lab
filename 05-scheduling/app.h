/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __APP_H
#define __APP_H

#define NUM_WORKERS	5
#define COUNT_TARGET	10000000u

void worker(void *arg);
void rogue(void *arg);

#endif /* __APP_H */
