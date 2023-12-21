/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * Counting semaphores. These run in the SVC handler, which can't be
 * preempted by SysTick or PendSV (see 05-scheduling/kernel/task.c), so
 * they're atomic. Posting from an IRQ would need irq_save()/irq_restore().
 */
#include <stddef.h>
#include <sem.h>
#include <errno.h>
#include <task.h>

struct semaphore {
	int in_use;
	int count;
	struct task_queue waiters;
};

static struct semaphore sems[MAX_SEMAPHORES];

static struct semaphore *lookup(int id)
{
	if (id < 0 || id >= MAX_SEMAPHORES || !sems[id].in_use)
		return NULL;
	return &sems[id];
}

int ksem_create(int initial)
{
	for (int id = 0; id < MAX_SEMAPHORES; id++) {
		if (!sems[id].in_use) {
			sems[id].in_use = 1;
			sems[id].count = initial;
			return id;
		}
	}
	return -ENOMEM;
}

int32_t sys_sem_wait(int id)
{
	struct semaphore *s = lookup(id);

	if (!s)
		return -EINVAL;

	if (s->count > 0) {
		s->count--;
		return 0;
	}

	/* sem_post() hands the unit straight to us, so just return 0 on wakeup */
	task_block(&s->waiters);
	return 0;
}

int32_t sys_sem_post(int id)
{
	struct semaphore *s = lookup(id);

	if (!s)
		return -EINVAL;

	/* hand off directly to keep waiters FIFO */
	if (s->waiters.head)
		task_wake(&s->waiters);
	else
		s->count++;
	return 0;
}
