/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 02: show digits from the console and print GPIOC->ODR each time. */
#include <stdint.h>
#include <kstdio.h>
#include <sys_init.h>
#include <stm32_peps.h>
#include <seven_segment.h>

void kmain(void)
{
	sys_init();
	seven_segment_init();

	kprintf("\n== Lesson 02: GPIO and a seven-segment display ==\n");
	kprintf("GPIOC lives at 0x%08x; its output register ODR at 0x%08x.\n",
		(uint32_t)GPIOC, (uint32_t)&GPIOC->ODR);
	kprintf("Enter a digit 0-9 (anything else blanks the display).\n");

	for (;;) {
		int digit = -1;

		kprintf("> ");
		kscanf("%d", &digit);

		if (seven_segment_show(digit) == 0)
			kprintf("showing %d", digit);
		else
			kprintf("display off");
		kprintf("   GPIOC->ODR = 0x%04x\n", (unsigned)(GPIOC->ODR & 0xFFFF));
	}
}
