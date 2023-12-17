/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* every task starts here (see init_stack() in kernel/task.c) */
#include <unistd.h>

void _task_start(void (*fn)(void *), void *arg)
{
	fn(arg);
	exit(0);
}
