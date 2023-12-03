/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
	SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
	.text :
	{
		_stext = .;
		KEEP(*(.isr_vector))	/* must be first */
		*(.text*)
		*(.rodata*)
		. = ALIGN(4);
		_etext = .;
	} > FLASH

	.ARM.exidx :			/* from libgcc */
	{
		*(.ARM.exidx*)
	} > FLASH

	_la_data = LOADADDR(.data);

	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data*)
		. = ALIGN(4);
		_edata = .;
	} > SRAM AT > FLASH

	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > SRAM
}
