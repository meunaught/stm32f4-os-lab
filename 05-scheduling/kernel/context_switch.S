/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * Context switch. A switched-out task's stack, top down:
 *
 *	xpsr, pc, lr, r12, r3, r2, r1, r0	hardware
 *	[s16-s31]				if the task used the FPU
 *	EXC_RETURN, r11, ..., r4		task->sp points at r4
 */
	.syntax unified
	.cpu cortex-m4
	.fpu fpv4-sp-d16
	.thumb
	.text

	.global	PendSV_Handler
	.type	PendSV_Handler, %function
PendSV_Handler:
	ldr	r3, =current_task
	ldr	r2, [r3]
	cbz	r2, first_switch

	mrs	r0, psp
	tst	lr, #0x10		/* FPU context? */
	it	eq
	vstmdbeq r0!, {s16-s31}
	stmdb	r0!, {r4-r11, lr}
	str	r0, [r2]
	b	pick_next

first_switch:
	/* tasks run unprivileged */
	mrs	r0, control
	orr	r0, r0, #1
	msr	control, r0
	isb

pick_next:
	bl	schedule

	ldr	r3, =current_task
	ldr	r2, [r3]
	ldr	r0, [r2]
	ldmia	r0!, {r4-r11, lr}
	tst	lr, #0x10
	it	eq
	vldmiaeq r0!, {s16-s31}
	msr	psp, r0
	bx	lr
	.size	PendSV_Handler, . - PendSV_Handler
