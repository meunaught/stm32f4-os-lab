/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __TASK_H
#define __TASK_H

#include <stdint.h>

/* only one task in this lesson */
#define FIRST_PID		1000
#define USER_STACK_WORDS	512

enum task_state {
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED,
	TASK_TERMINATED,
};

struct task {
	uint32_t *sp;		/* used from lesson 05 */
	uint32_t pid;
	enum task_state state;
	int exit_status;
};

extern struct task *current_task;

void task_start_user(void (*entry)(void)) __attribute__((noreturn));
void task_exit(int status);

#endif /* __TASK_H */
