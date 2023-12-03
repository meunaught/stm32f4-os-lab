/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __USART_H
#define __USART_H

#include <stdint.h>
#include <stm32_peps.h>

void usart2_init(uint32_t baud);
void usart_putc(USART_TypeDef *usart, char c);
char usart_getc(USART_TypeDef *usart);
int usart_try_getc(USART_TypeDef *usart);

#endif /* __USART_H */
