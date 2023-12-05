/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Lesson 01: from reset to kmain(). The README covers what runs before this. */
#include <stdint.h>
#include <cm4.h>
#include <clock.h>
#include <kstdio.h>
#include <sys_init.h>

extern uint32_t _stext, _etext, _la_data, _sdata, _edata, _sbss, _ebss, _estack;

static uint32_t answer = 42;
static uint32_t zeroed;

static void show_memory_map(void)
{
	uint32_t text = (uint32_t)&_etext - (uint32_t)&_stext;
	uint32_t data = (uint32_t)&_edata - (uint32_t)&_sdata;
	uint32_t bss = (uint32_t)&_ebss - (uint32_t)&_sbss;

	kprintf("\nMemory map (from the linker script):\n");
	kprintf("  .text  0x%08x-0x%08x %6u bytes  code + constants, in flash\n",
		(uint32_t)&_stext, (uint32_t)&_etext, text);
	kprintf("  .data  0x%08x-0x%08x %6u bytes  copied to SRAM from flash 0x%08x\n",
		(uint32_t)&_sdata, (uint32_t)&_edata, data, (uint32_t)&_la_data);
	kprintf("  .bss   0x%08x-0x%08x %6u bytes  zeroed in SRAM\n",
		(uint32_t)&_sbss, (uint32_t)&_ebss, bss);
	kprintf("  stack  starts at 0x%08x and grows down; sp is now 0x%08x\n",
		(uint32_t)&_estack, __get_MSP());

	kprintf("\nReset_Handler did its job if these hold:\n");
	kprintf("  answer = %u (initialised in .data, expect 42)\n", answer);
	kprintf("  zeroed = %u (in .bss, expect 0)\n", zeroed);
}

static void show_cpu(void)
{
	kprintf("\nCPU:\n");
	kprintf("  core clock %u MHz, SysTick every %u ms\n",
		SYSTEM_CORE_CLOCK / 1000000, 1000 / TICK_HZ);
	kprintf("  running %s on the %s stack\n",
		(__get_CONTROL() & CONTROL_nPRIV) ? "unprivileged" : "privileged",
		(__get_CONTROL() & CONTROL_SPSEL) ? "process" : "main");
	kprintf("  FPU enabled: 1.0f / 3 = %f\n", (double)(1.0f / 3));
	kprintf("  uptime so far %u ms\n", get_time_ms());
}

void kmain(void)
{
	char line[64];

	sys_init();

	kprintf("\n== Lesson 01: from reset to kmain() ==\n");
	show_memory_map();
	show_cpu();

	kprintf("\nType a line and press Enter; it comes back with a timestamp.\n");
	for (;;) {
		kprintf("> ");
		kreadline(line, sizeof(line));
		kprintf("[%u ms] %s\n", get_time_ms(), line);
	}
}
