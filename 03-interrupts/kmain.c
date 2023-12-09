/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 03: interrupts and exceptions. */
#include <stdint.h>
#include <button.h>
#include <clock.h>
#include <cm4.h>
#include <kstdio.h>
#include <sys_init.h>

/* SysTick above the button so BASEPRI can mask just the button */
#define SYSTICK_PRIORITY	4
#define BUTTON_PRIORITY		8

/* DWT keeps counting with interrupts masked, SysTick doesn't */
static void spin_seconds(uint32_t seconds)
{
	for (uint32_t s = 0; s < seconds; s++) {
		uint32_t start = cycle_count();

		while (cycle_count() - start < SYSTEM_CORE_CLOCK)
			;
	}
}

static void help(void)
{
	kprintf("\nPress the blue USER button at any time.\n");
	kprintf("Keys:\n");
	kprintf("  p  mask ALL interrupts for 5 s (PRIMASK)\n");
	kprintf("  b  mask only the button for 5 s (BASEPRI)\n");
	kprintf("  s  raise the button interrupt from software\n");
	kprintf("  z  divide by zero (UsageFault)\n");
	kprintf("  u  undefined instruction with UsageFault disabled (escalates to HardFault)\n");
	kprintf("  r  reset the board\n");
	kprintf("  h  this help\n");
}

static void demo_primask(void)
{
	uint32_t before = button_presses();
	uint32_t t0 = get_time_ms();

	kprintf("PRIMASK=1: every interrupt is held off for 5 s. Press the button now...\n");
	__disable_irq();
	spin_seconds(5);
	int pending = nvic_is_pending(EXTI15_10_IRQn);
	uint32_t t1 = get_time_ms();
	__enable_irq();

	kprintf("  SysTick was masked too: the clock advanced %u ms in 5 s.\n", t1 - t0);
	kprintf("  Button interrupt pending while masked: %s. Presses counted: %u.\n",
		pending ? "yes" : "no", button_presses() - before);
	kprintf("  A masked interrupt is remembered once, however often it fired.\n");
}

static void demo_basepri(void)
{
	uint32_t t0 = get_time_ms();

	kprintf("BASEPRI=%u: priorities %u-15 are masked for 5 s. Press the button now...\n",
		BUTTON_PRIORITY, BUTTON_PRIORITY);
	__set_BASEPRI(BUTTON_PRIORITY);
	spin_seconds(5);
	int pending = nvic_is_pending(EXTI15_10_IRQn);
	uint32_t t1 = get_time_ms();
	__set_BASEPRI(0);

	kprintf("  SysTick (priority %u) kept running: the clock advanced %u ms.\n",
		SYSTICK_PRIORITY, t1 - t0);
	kprintf("  Button interrupt pending while masked: %s.\n", pending ? "yes" : "no");
}

static void demo_divide_by_zero(void)
{
	volatile int zero = 0;
	volatile int result;

	kprintf("Dividing by zero with the trap enabled...\n");
	SCB->CCR |= SCB_CCR_DIV_0_TRP;		/* otherwise x/0 is just 0 */
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA;
	result = 100 / zero;
	(void)result;
}

static void demo_escalation(void)
{
	kprintf("Executing an undefined instruction with UsageFault disabled...\n");
	SCB->SHCSR &= ~SCB_SHCSR_USGFAULTENA;
	__asm volatile("udf #0");
}

void kmain(void)
{
	uint32_t reported = 0;

	sys_init();
	cycle_counter_init();
	nvic_set_priority(SysTick_IRQn, SYSTICK_PRIORITY);
	button_init(BUTTON_PRIORITY);

	kprintf("\n== Lesson 03: interrupts and exceptions ==\n");
	help();

	for (;;) {
		uint32_t presses = button_presses();

		if (presses != reported) {
			reported = presses;
			kprintf("[%u ms] button pressed (%u so far)\n", get_time_ms(), presses);
		}

		switch (ktrygetc()) {
		case 'p': demo_primask(); break;
		case 'b': demo_basepri(); break;
		case 's': button_trigger_in_software(); break;
		case 'z': demo_divide_by_zero(); break;
		case 'u': demo_escalation(); break;
		case 'r': system_reset();
		case 'h': help(); break;
		default: break;
		}
	}
}
