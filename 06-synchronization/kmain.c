/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 06: synchronization with semaphores. */
#include <stddef.h>
#include <stdint.h>
#include <app.h>
#include <kstdio.h>
#include <sem.h>
#include <sys_init.h>
#include <task.h>

static struct buffer_sems buffer_sems;

void kmain(void)
{
	int choice = 0;

	sys_init();

	kprintf("\n== Lesson 06: synchronization ==\n");
	kprintf("1) shared counter without a lock (lost updates)\n");
	kprintf("2) shared counter protected by a semaphore used as a mutex\n");
	kprintf("3) producers and consumers sharing a %d-slot buffer\n", BUFFER_SLOTS);
	kprintf("choice> ");
	kscanf("%d", &choice);

	sched_init(SCHED_RR);

	switch (choice) {
	case 2: {
		int mutex = ksem_create(1);

		for (int i = 0; i < NUM_WORKERS; i++)
			task_create(counter_locked, (void *)(uintptr_t)mutex);
		kprintf("Workers exit with their increment counts; with the lock "
			"they add up to exactly %u.\n\n", COUNT_TARGET);
		break;
	}
	case 3:
		buffer_sems.empty = ksem_create(BUFFER_SLOTS);
		buffer_sems.full = ksem_create(0);
		buffer_sems.mutex = ksem_create(1);
		for (int i = 0; i < NUM_PRODUCERS; i++)
			task_create(producer, &buffer_sems);
		for (int i = 0; i < NUM_CONSUMERS; i++)
			task_create(consumer, &buffer_sems);
		kprintf("Consumers exit with the sum of the items they took; "
			"together that is %d.\n\n",
			NUM_PRODUCERS * ITEMS_PER_PRODUCER * (ITEMS_PER_PRODUCER + 1) / 2);
		break;
	default:
		for (int i = 0; i < NUM_WORKERS; i++)
			task_create(counter_unlocked, NULL);
		kprintf("Workers exit with their increment counts; a sum above %u "
			"means updates were lost.\n\n", COUNT_TARGET);
		break;
	}

	sched_start();
}
