/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <stdint.h>
#include <app.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

static volatile uint32_t shared_count;

/* makes the race show up on every run */
static void widen_race_window(void)
{
	for (volatile int i = 0; i < 50; i++)
		;
}

void counter_unlocked(void *unused)
{
	uint32_t mine = 0;

	for (;;) {
		uint32_t value = shared_count;

		if (value >= COUNT_TARGET)
			break;
		widen_race_window();
		shared_count = value + 1;
		mine++;
	}
	printf("pid %d: %u increments\n", getpid(), mine);
	exit((int)mine);
}

void counter_locked(void *lock)
{
	int mutex = (int)(uintptr_t)lock;
	uint32_t mine = 0;

	for (;;) {
		sem_wait(mutex);
		uint32_t value = shared_count;

		if (value >= COUNT_TARGET) {
			sem_post(mutex);
			break;
		}
		widen_race_window();
		shared_count = value + 1;
		sem_post(mutex);
		mine++;
	}
	printf("pid %d: %u increments\n", getpid(), mine);
	exit((int)mine);
}

/* bounded buffer */
static int buffer[BUFFER_SLOTS];
static int in, out;

static void busy_ms(uint32_t ms)
{
	uint32_t start = uptime();

	while (uptime() - start < ms)
		;
}

void producer(void *arg)
{
	const struct buffer_sems *s = arg;

	for (int item = 1; item <= ITEMS_PER_PRODUCER; item++) {
		sem_wait(s->empty);
		sem_wait(s->mutex);
		buffer[in] = item;
		in = (in + 1) % BUFFER_SLOTS;
		sem_post(s->mutex);
		sem_post(s->full);
		printf("pid %d produced %d\n", getpid(), item);
	}
	exit(0);
}

void consumer(void *arg)
{
	const struct buffer_sems *s = arg;
	int sum = 0;

	for (int n = 0; n < ITEMS_PER_CONSUMER; n++) {
		sem_wait(s->full);
		sem_wait(s->mutex);
		int item = buffer[out];
		out = (out + 1) % BUFFER_SLOTS;
		sem_post(s->mutex);
		sem_post(s->empty);

		printf("        pid %d consumed %d\n", getpid(), item);
		sum += item;
		busy_ms(20);			/* slow consumer so producers block */
	}
	exit(sum);
}
