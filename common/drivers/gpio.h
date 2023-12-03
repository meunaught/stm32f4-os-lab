/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stm32_peps.h>

#define GPIO_PIN(n)	((uint16_t)(1u << (n)))

enum gpio_mode {
	GPIO_MODE_INPUT  = 0,
	GPIO_MODE_OUTPUT = 1,
	GPIO_MODE_ALTFN  = 2,
	GPIO_MODE_ANALOG = 3,
};

enum gpio_pull {
	GPIO_NOPULL   = 0,
	GPIO_PULLUP   = 1,
	GPIO_PULLDOWN = 2,
};

void gpio_clock_enable(GPIO_TypeDef *port);
void gpio_init(GPIO_TypeDef *port, uint16_t pins, enum gpio_mode mode,
	       enum gpio_pull pull);
void gpio_set_altfn(GPIO_TypeDef *port, uint32_t pin, uint32_t af);
void gpio_write(GPIO_TypeDef *port, uint16_t pins, int high);
int gpio_read(GPIO_TypeDef *port, uint32_t pin);

#endif /* __GPIO_H */
