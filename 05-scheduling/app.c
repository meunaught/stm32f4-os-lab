/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * Workers race on an unprotected counter, so under round robin some
 * increments get lost (fixed in lesson 06).
 */
#include <stdint.h>
#include <app.h>
#include <cm4.h>
#include <stdio.h>
#include <unistd.h>

static volatile uint32_t shared_count;

void worker(void *arg)
{
	uint32_t mine = 0;

	printf("pid %d starts at %u ms\n", getpid(), uptime());
	for (;;) {
		uint32_t value = shared_count;

		if (value >= COUNT_TARGET)
			break;
		shared_count = value + 1;
		mine++;
	}
	printf("pid %d done at %u ms: %u increments\n", getpid(), uptime(), mine);
	exit((int)mine);
}

/* faults: SysTick is privileged */
void rogue(void *arg)
{
	printf("pid %d (rogue) reads SysTick->CTRL from user mode...\n", getpid());
	printf("%u\n", SYSTICK->CTRL);
	printf("pid %d (rogue): not reached\n", getpid());
}
