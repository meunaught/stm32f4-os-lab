/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>

void button_init(uint32_t priority);
uint32_t button_presses(void);
void button_trigger_in_software(void);

#endif /* __BUTTON_H */
