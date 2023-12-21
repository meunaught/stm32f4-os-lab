/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __APP_H
#define __APP_H

#define NUM_WORKERS		4
#define COUNT_TARGET		200000u

#define BUFFER_SLOTS		4
#define NUM_PRODUCERS		2
#define NUM_CONSUMERS		2
#define ITEMS_PER_PRODUCER	10
#define ITEMS_PER_CONSUMER	(NUM_PRODUCERS * ITEMS_PER_PRODUCER / NUM_CONSUMERS)

struct buffer_sems {
	int empty;
	int full;
	int mutex;
};

void counter_unlocked(void *unused);
void counter_locked(void *lock);
void producer(void *sems);
void consumer(void *sems);

#endif /* __APP_H */
