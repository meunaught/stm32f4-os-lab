/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * SVC, SysTick and PendSV all run at the lowest priority and can't preempt
 * each other, so the scheduler needs no locks. Higher-priority IRQs must not
 * touch scheduler state.
 */
#include <stddef.h>
#include <task.h>
#include <cm4.h>
#include <kstdio.h>

void _task_start(void (*fn)(void *), void *arg);

struct task *current_task;

static struct task tasks[MAX_TASKS];
static uint32_t stacks[MAX_TASKS][TASK_STACK_WORDS] __attribute__((aligned(8)));
static int ntasks;
static int live_tasks;

static struct task idle_task;
static uint32_t idle_stack[128] __attribute__((aligned(8)));

static struct task_queue ready_queue;
static enum sched_policy policy;
static uint32_t slice_left;
static uint32_t next_pid = FIRST_PID;

static void queue_push(struct task_queue *q, struct task *t)
{
	t->next = NULL;
	if (q->tail)
		q->tail->next = t;
	else
		q->head = t;
	q->tail = t;
}

static struct task *queue_pop(struct task_queue *q)
{
	struct task *t = q->head;

	if (t) {
		q->head = t->next;
		if (!q->head)
			q->tail = NULL;
		t->next = NULL;
	}
	return t;
}

static void request_switch(void)
{
	SCB->ICSR = SCB_ICSR_PENDSVSET;
}

/* make it look like the task was preempted right before _task_start(fn, arg) */
static void init_stack(struct task *t, uint32_t *top,
		       void (*fn)(void *), void *arg)
{
	uint32_t *sp = top;

	/* hardware frame */
	*--sp = XPSR_THUMB;			/* xpsr */
	*--sp = (uint32_t)_task_start & ~1u;	/* pc */
	*--sp = 0;				/* lr */
	*--sp = 0;				/* r12 */
	*--sp = 0;				/* r3 */
	*--sp = 0;				/* r2 */
	*--sp = (uint32_t)arg;			/* r1 */
	*--sp = (uint32_t)fn;			/* r0 */

	/* EXC_RETURN and r11..r4, popped by PendSV */
	*--sp = EXC_RETURN_THREAD_PSP;
	for (int i = 0; i < 8; i++)
		*--sp = 0;

	t->sp = sp;
}

static void idle(void *arg)
{
	for (;;)
		__WFI();
}

void sched_init(enum sched_policy p)
{
	policy = p;
	init_stack(&idle_task, &idle_stack[128], idle, NULL);
	idle_task.pid = 0;
	idle_task.state = TASK_READY;
}

struct task *task_create(void (*fn)(void *), void *arg)
{
	if (ntasks == MAX_TASKS)
		return NULL;

	struct task *t = &tasks[ntasks];

	init_stack(t, &stacks[ntasks][TASK_STACK_WORDS], fn, arg);
	ntasks++;

	t->pid = next_pid++;
	t->state = TASK_READY;
	t->arrival_ms = get_time_ms();
	live_tasks++;
	queue_push(&ready_queue, t);
	return t;
}

void sched_start(void)
{
	nvic_set_priority(SVCall_IRQn, NVIC_LOWEST_PRIORITY);
	nvic_set_priority(SysTick_IRQn, NVIC_LOWEST_PRIORITY);
	nvic_set_priority(PendSV_IRQn, NVIC_LOWEST_PRIORITY);

	/* nothing to save the first time; kmain() is never resumed */
	current_task = NULL;
	request_switch();
	__DSB();
	__ISB();
	for (;;)
		;
}

/* called from PendSV between saving and restoring */
void schedule(void)
{
	struct task *prev = current_task;

	if (prev && prev != &idle_task && prev->state == TASK_RUNNING) {
		prev->state = TASK_READY;
		queue_push(&ready_queue, prev);
	}

	struct task *next = queue_pop(&ready_queue);

	if (!next)
		next = &idle_task;

	if (!next->has_run) {
		next->has_run = 1;
		next->first_run_ms = get_time_ms();
	}
	next->state = TASK_RUNNING;
	slice_left = TIME_SLICE_MS;
	current_task = next;
}

void systick_hook(void)
{
	struct task *t = current_task;

	if (!t)
		return;

	if (t == &idle_task) {
		if (ready_queue.head)
			request_switch();
		return;
	}

	t->cpu_ms++;
	if (policy == SCHED_RR && --slice_left == 0)
		request_switch();
}

static void print_stats(void);

void task_exit(int status)
{
	struct task *t = current_task;

	t->state = TASK_TERMINATED;
	t->exit_status = status;
	t->finish_ms = get_time_ms();

	if (--live_tasks == 0)
		print_stats();
	request_switch();
}

void task_yield(void)
{
	request_switch();
}

void task_block(struct task_queue *wait_queue)
{
	current_task->state = TASK_BLOCKED;
	queue_push(wait_queue, current_task);
	request_switch();
}

void task_wake(struct task_queue *wait_queue)
{
	struct task *t = queue_pop(wait_queue);

	if (!t)
		return;
	t->state = TASK_READY;
	queue_push(&ready_queue, t);
	if (current_task == &idle_task)
		request_switch();
}

/* called from fault.c when a task faults */
int fault_kill_current_task(void)
{
	if (!current_task || current_task == &idle_task)
		return 0;

	kprintf("[kernel] killing task %u; the other tasks carry on\n",
		current_task->pid);
	task_exit(-1);
	return 1;
}

static void print_stats(void)
{
	uint32_t sum_response = 0, sum_waiting = 0, sum_turnaround = 0;
	int sum_status = 0;

	kprintf("\nAll tasks finished. Policy: %s\n\n",
		policy == SCHED_RR ? "round robin" : "first come, first served");
	kprintf("  pid  arrival  response  waiting    cpu  turnaround  exit status\n");

	for (int i = 0; i < ntasks; i++) {
		struct task *t = &tasks[i];
		uint32_t turnaround = t->finish_ms - t->arrival_ms;
		uint32_t response = t->first_run_ms - t->arrival_ms;
		uint32_t waiting = turnaround > t->cpu_ms ? turnaround - t->cpu_ms : 0;

		kprintf("%5u %8u %9u %8u %6u %11u %12d\n", t->pid, t->arrival_ms,
			response, waiting, t->cpu_ms, turnaround, t->exit_status);
		sum_response += response;
		sum_waiting += waiting;
		sum_turnaround += turnaround;
		if (t->exit_status >= 0)	/* skip killed tasks */
			sum_status += t->exit_status;
	}

	kprintf("\naverage response %u ms, waiting %u ms, turnaround %u ms\n",
		sum_response / ntasks, sum_waiting / ntasks, sum_turnaround / ntasks);
	kprintf("sum of exit statuses: %d\n", sum_status);
	kprintf("(times in ms; response = first run - arrival, "
		"waiting = turnaround - cpu)\n");
	kprintf("Press RESET to run again.\n");
}
