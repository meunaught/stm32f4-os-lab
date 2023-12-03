/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Polled USART. USART2 (PA2 TX, PA3 RX) goes to the ST-LINK's virtual COM port. */
#include <usart.h>
#include <gpio.h>
#include <clock.h>

void usart2_init(uint32_t baud)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	gpio_clock_enable(GPIOA);

	/* AF7 = USART2 */
	gpio_init(GPIOA, GPIO_PIN(2) | GPIO_PIN(3), GPIO_MODE_ALTFN, GPIO_NOPULL);
	gpio_set_altfn(GPIOA, 2, 7);
	gpio_set_altfn(GPIOA, 3, 7);

	/* 8N1 is the reset default; BRR = 45 MHz / 115200, rounded */
	USART2->CR1 = 0;
	USART2->BRR = (APB1_CLOCK + baud / 2) / baud;
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void usart_putc(USART_TypeDef *usart, char c)
{
	while (!(usart->SR & USART_SR_TXE))
		;
	usart->DR = (uint8_t)c;
}

char usart_getc(USART_TypeDef *usart)
{
	while (!(usart->SR & USART_SR_RXNE))
		;
	return (char)usart->DR;
}

/* -1 if nothing has arrived */
int usart_try_getc(USART_TypeDef *usart)
{
	if (!(usart->SR & USART_SR_RXNE))
		return -1;
	return (uint8_t)usart->DR;
}
