/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <task.h>
#include <cm4.h>
#include <kstdio.h>

void enter_user_mode(void (*entry)(void), uint32_t *stack_top) __attribute__((noreturn));

static uint32_t user_stack[USER_STACK_WORDS] __attribute__((aligned(8)));

static struct task process = {
	.pid = FIRST_PID,
	.state = TASK_READY,
};

struct task *current_task;

void task_start_user(void (*entry)(void))
{
	current_task = &process;
	process.state = TASK_RUNNING;
	enter_user_mode(entry, &user_stack[USER_STACK_WORDS]);
}

void task_exit(int status)
{
	current_task->state = TASK_TERMINATED;
	current_task->exit_status = status;

	kprintf("\n[kernel] process %u exited with status %d\n",
		current_task->pid, status);
	kprintf("[kernel] nothing else to run (lesson 05 adds a scheduler). "
		"Press RESET.\n");
	for (;;)
		__WFI();
}
