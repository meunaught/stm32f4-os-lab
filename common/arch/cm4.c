/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <cm4.h>
#include <clock.h>

static volatile uint32_t tick_ms;

void fpu_enable(void)
{
	/* called from Reset_Handler, before any float is touched */
	SCB->CPACR |= SCB_CPACR_FPU_FULL;
	__DSB();
	__ISB();
}

void systick_init(void)
{
	SYSTICK->CTRL = 0;
	SYSTICK->LOAD = SYSTEM_CORE_CLOCK / TICK_HZ - 1;
	SYSTICK->VAL = 0;
	tick_ms = 0;
	nvic_set_priority(SysTick_IRQn, NVIC_LOWEST_PRIORITY);
	SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_TICKINT |
			SYSTICK_CTRL_ENABLE;
}

uint32_t get_time_ms(void)
{
	return tick_ms;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = tick_ms;

	while (tick_ms - start < ms)
		;
}

/* overridden by the scheduler */
__attribute__((weak)) void systick_hook(void)
{
}

void SysTick_Handler(void)
{
	tick_ms++;
	systick_hook();
}

/* ISER/ICER/ISPR/ICPR are write-1-to-act, so no read-modify-write here */
void nvic_enable_irq(IRQn_Type irq)
{
	if (irq >= 0)
		NVIC->ISER[irq >> 5] = 1u << (irq & 31);
}

void nvic_disable_irq(IRQn_Type irq)
{
	if (irq >= 0) {
		NVIC->ICER[irq >> 5] = 1u << (irq & 31);
		__DSB();
		__ISB();
	}
}

void nvic_set_pending(IRQn_Type irq)
{
	if (irq >= 0)
		NVIC->ISPR[irq >> 5] = 1u << (irq & 31);
}

void nvic_clear_pending(IRQn_Type irq)
{
	if (irq >= 0)
		NVIC->ICPR[irq >> 5] = 1u << (irq & 31);
}

int nvic_is_pending(IRQn_Type irq)
{
	if (irq < 0)
		return 0;
	return (NVIC->ISPR[irq >> 5] >> (irq & 31)) & 1u;
}

/* system exceptions (irq < 0) use SCB->SHPR, which starts at exception 4 */
void nvic_set_priority(IRQn_Type irq, uint32_t priority)
{
	uint8_t value = (uint8_t)(priority << (8 - NVIC_PRIO_BITS));

	if (irq < 0)
		SCB->SHPR[((uint32_t)irq & 0xF) - 4] = value;
	else
		NVIC->IP[irq] = value;
}

uint32_t nvic_get_priority(IRQn_Type irq)
{
	uint8_t value;

	if (irq < 0)
		value = SCB->SHPR[((uint32_t)irq & 0xF) - 4];
	else
		value = NVIC->IP[irq];
	return value >> (8 - NVIC_PRIO_BITS);
}

void cycle_counter_init(void)
{
	DEMCR |= DEMCR_TRCENA;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA;
}

void system_reset(void)
{
	__DSB();
	SCB->AIRCR = SCB_AIRCR_VECTKEY |
		     (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
		     SCB_AIRCR_SYSRESETREQ;
	__DSB();
	for (;;)
		;
}
