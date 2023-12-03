/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __CLOCK_H
#define __CLOCK_H

/* HSE 8 MHz /M -> 2 MHz xN -> 360 MHz /P -> 180 MHz */
#define HSE_CLOCK		8000000U	/* from the ST-LINK */
#define PLL_M			4
#define PLL_N			180
#define PLL_P			2
#define PLL_Q			4		/* unused */
#define PLL_R			2		/* unused */

#define SYSTEM_CORE_CLOCK	180000000U
#define APB1_CLOCK		(SYSTEM_CORE_CLOCK / 4)
#define APB2_CLOCK		(SYSTEM_CORE_CLOCK / 2)

void clock_init(void);

#endif /* __CLOCK_H */
