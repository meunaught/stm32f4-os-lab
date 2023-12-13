/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 04: system calls. */
#include <cm4.h>
#include <kstdio.h>
#include <sys_init.h>
#include <task.h>

void _start(void);	/* user/crt0.c */

void kmain(void)
{
	sys_init();

	/* SysTick has to preempt a read() that is waiting for input */
	nvic_set_priority(SVCall_IRQn, NVIC_LOWEST_PRIORITY);
	nvic_set_priority(SysTick_IRQn, NVIC_LOWEST_PRIORITY - 1);

	kprintf("\n== Lesson 04: system calls ==\n");
	kprintf("[kernel] privileged, on the main stack (sp 0x%08x)\n", __get_MSP());
	kprintf("[kernel] starting the user program: unprivileged, process stack\n\n");

	task_start_user(_start);
}
