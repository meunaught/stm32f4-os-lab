/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 05: multitasking and scheduling. */
#include <stddef.h>
#include <stdint.h>
#include <app.h>
#include <kstdio.h>
#include <sys_init.h>
#include <task.h>

void kmain(void)
{
	int choice = 0;

	sys_init();

	kprintf("\n== Lesson 05: scheduling ==\n");
	kprintf("1) round robin: preemptive, %u ms time slices\n", TIME_SLICE_MS);
	kprintf("2) first come, first served: each task runs until it exits\n");
	kprintf("3) round robin with a misbehaving task\n");
	kprintf("choice> ");
	kscanf("%d", &choice);

	sched_init(choice == 2 ? SCHED_FCFS : SCHED_RR);

	for (int i = 0; i < NUM_WORKERS; i++) {
		task_create(worker, NULL);
		if (choice == 3 && i == 1)
			task_create(rogue, NULL);
	}

	kprintf("Each worker exits with its number of increments; a sum above %u\n"
		"means updates were lost. Starting the scheduler.\n\n", COUNT_TARGET);
	sched_start();
}
