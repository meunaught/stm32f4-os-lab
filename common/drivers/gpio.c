/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <gpio.h>

/* ports are 0x400 apart starting at GPIOA */
static uint32_t port_index(GPIO_TypeDef *port)
{
	return ((uint32_t)port - (uint32_t)GPIOA) / 0x400U;
}

void gpio_clock_enable(GPIO_TypeDef *port)
{
	RCC->AHB1ENR |= 1u << port_index(port);
}

void gpio_init(GPIO_TypeDef *port, uint16_t pins, enum gpio_mode mode,
	       enum gpio_pull pull)
{
	for (uint32_t pin = 0; pin < 16; pin++) {
		if (!(pins & (1u << pin)))
			continue;

		uint32_t shift = pin * 2;

		port->MODER = (port->MODER & ~(3u << shift)) | ((uint32_t)mode << shift);
		port->PUPDR = (port->PUPDR & ~(3u << shift)) | ((uint32_t)pull << shift);
		port->OSPEEDR |= 2u << shift;		/* high speed */
		port->OTYPER &= ~(1u << pin);		/* push-pull */
	}
}

void gpio_set_altfn(GPIO_TypeDef *port, uint32_t pin, uint32_t af)
{
	uint32_t shift = (pin % 8) * 4;

	port->AFR[pin / 8] = (port->AFR[pin / 8] & ~(0xFu << shift)) | (af << shift);
}

void gpio_write(GPIO_TypeDef *port, uint16_t pins, int high)
{
	/* low half sets, high half clears */
	port->BSRR = high ? pins : (uint32_t)pins << 16;
}

int gpio_read(GPIO_TypeDef *port, uint32_t pin)
{
	return (port->IDR >> pin) & 1u;
}
