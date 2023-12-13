/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * void enter_user_mode(void (*entry)(void), uint32_t *stack_top)
 *
 * Switch thread mode to PSP + unprivileged and jump to entry. No way back.
 */
	.syntax unified
	.cpu cortex-m4
	.thumb
	.text

	.global	enter_user_mode
	.type	enter_user_mode, %function
enter_user_mode:
	msr	psp, r1
	movs	r2, #3			/* SPSEL | nPRIV */
	msr	control, r2
	isb
	movs	r2, #0
	mov	lr, r2			/* returning from entry faults */
	bx	r0
	.size	enter_user_mode, . - enter_user_mode
