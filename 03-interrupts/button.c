/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * USER button B1 on PC13 (pulled up on the board, so a press is a falling
 * edge): PC13 -> SYSCFG -> EXTI line 13 -> EXTI15_10_IRQn.
 */
#include <button.h>
#include <cm4.h>
#include <gpio.h>
#include <stm32_peps.h>

#define BUTTON_PIN	13
#define BUTTON_LINE	(1u << BUTTON_PIN)
#define EXTICR_PORT_C	2u
#define DEBOUNCE_MS	50

static volatile uint32_t press_count;
static volatile uint32_t last_press_ms;

void button_init(uint32_t priority)
{
	gpio_clock_enable(GPIOC);
	gpio_init(GPIOC, GPIO_PIN(BUTTON_PIN), GPIO_MODE_INPUT, GPIO_NOPULL);

	/* port C -> EXTI line 13 */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	uint32_t shift = (BUTTON_PIN % 4) * 4;
	SYSCFG->EXTICR[BUTTON_PIN / 4] =
		(SYSCFG->EXTICR[BUTTON_PIN / 4] & ~(0xFu << shift)) |
		(EXTICR_PORT_C << shift);

	EXTI->FTSR |= BUTTON_LINE;
	EXTI->RTSR &= ~BUTTON_LINE;
	EXTI->PR = BUTTON_LINE;
	EXTI->IMR |= BUTTON_LINE;

	nvic_set_priority(EXTI15_10_IRQn, priority);
	nvic_enable_irq(EXTI15_10_IRQn);
}

void EXTI15_10_Handler(void)
{
	if (!(EXTI->PR & BUTTON_LINE))
		return;

	/* write 1 to clear; |= would clear other pending lines too */
	EXTI->PR = BUTTON_LINE;

	uint32_t now = get_time_ms();

	if (now - last_press_ms >= DEBOUNCE_MS) {
		last_press_ms = now;
		press_count++;
	}
}

uint32_t button_presses(void)
{
	return press_count;
}

void button_trigger_in_software(void)
{
	EXTI->SWIER = BUTTON_LINE;
}
