/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <sys_init.h>
#include <clock.h>
#include <usart.h>
#include <cm4.h>
#include <kstdio.h>

void sys_init(void)
{
	clock_init();
	usart2_init(115200);
	systick_init();

	kprintf("\n************************************\n");
	kprintf("Booting DUOS .....\n");
	kprintf("Copyright (c) 2023, CSE, DU\n");
	kprintf("CPUID %x\n", SCB->CPUID);
	kprintf("OS Version: 1.0\n");
	kprintf("Mahadi-03 & Joyee-42\n");
	kprintf("************************************\n");
}
