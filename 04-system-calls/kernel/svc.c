/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * svc_dispatch() gets the stacked frame: arguments in r0-r2, the result goes
 * back into the saved r0.
 */
#include <cm4.h>
#include <syscall.h>

void svc_dispatch(struct exception_frame *frame);

__attribute__((naked)) void SVCall_Handler(void)
{
	__asm volatile(
		"tst   lr, #4          \n"	/* PSP or MSP? */
		"ite   eq              \n"
		"mrseq r0, msp         \n"
		"mrsne r0, psp         \n"
		"b     svc_dispatch    \n");
}

void svc_dispatch(struct exception_frame *frame)
{
	/* svc is 0xDFnn and pc is just past it, so nn is at pc - 2 */
	uint8_t number = ((const uint8_t *)frame->pc)[-2];

	frame->r0 = (uint32_t)syscall(number, frame->r0, frame->r1, frame->r2);
}
