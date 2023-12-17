/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __TASK_H
#define __TASK_H

#include <stdint.h>

#define FIRST_PID		1000
#define MAX_TASKS		12
#define TASK_STACK_WORDS	256
#define TIME_SLICE_MS		10

enum task_state {
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED,
	TASK_TERMINATED,
};

enum sched_policy {
	SCHED_RR,
	SCHED_FCFS,		/* runs until exit, block or yield */
};

struct task {
	uint32_t *sp;		/* must be first, see context_switch.S */
	uint32_t pid;
	enum task_state state;
	int exit_status;
	struct task *next;

	uint32_t arrival_ms;
	uint32_t first_run_ms;
	uint32_t finish_ms;
	uint32_t cpu_ms;
	int has_run;
};

struct task_queue {
	struct task *head;
	struct task *tail;
};

extern struct task *current_task;

void sched_init(enum sched_policy policy);
struct task *task_create(void (*fn)(void *), void *arg);
void sched_start(void) __attribute__((noreturn));

/* these only pend PendSV; the switch happens when the syscall returns */
void task_exit(int status);
void task_yield(void);
void task_block(struct task_queue *wait_queue);
void task_wake(struct task_queue *wait_queue);

#endif /* __TASK_H */
