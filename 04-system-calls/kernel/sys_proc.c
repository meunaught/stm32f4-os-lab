/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <syscall.h>
#include <cm4.h>
#include <kstdio.h>
#include <task.h>

int32_t sys_getpid(void)
{
	return (int32_t)current_task->pid;
}

int32_t sys_exit(int status)
{
	task_exit(status);
	return 0;
}

int32_t sys_uptime(void)
{
	return (int32_t)get_time_ms();
}

int32_t sys_reboot(void)
{
	kprintf("[kernel] rebooting\n");
	system_reset();
}
