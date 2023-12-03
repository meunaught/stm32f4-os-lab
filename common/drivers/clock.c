/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* 180 MHz from the PLL. Sequence from RM0390, "Entering Over-drive mode". */
#include <clock.h>
#include <stm32_peps.h>

#define RCC_CR_HSEON		(1u << 16)
#define RCC_CR_HSERDY		(1u << 17)
#define RCC_CR_PLLON		(1u << 24)
#define RCC_CR_PLLRDY		(1u << 25)
#define RCC_PLLCFGR_SRC_HSE	(1u << 22)
#define RCC_CFGR_SW_Msk		(3u << 0)
#define RCC_CFGR_SW_PLL		(2u << 0)
#define RCC_CFGR_SWS_Msk	(3u << 2)
#define RCC_CFGR_SWS_PLL	(2u << 2)
#define RCC_CFGR_PPRE1_DIV4	(5u << 10)
#define RCC_CFGR_PPRE2_DIV2	(4u << 13)
#define PWR_CR_VOS_SCALE1	(3u << 14)
#define PWR_CR_ODEN		(1u << 16)
#define PWR_CR_ODSWEN		(1u << 17)
#define PWR_CSR_ODRDY		(1u << 16)
#define PWR_CSR_ODSWRDY		(1u << 17)
#define FLASH_ACR_LATENCY_5WS	(5u << 0)
#define FLASH_ACR_PRFTEN	(1u << 8)
#define FLASH_ACR_ICEN		(1u << 9)
#define FLASH_ACR_DCEN		(1u << 10)

void clock_init(void)
{
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS_SCALE1;

	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (((PLL_P / 2) - 1) << 16) |
		       RCC_PLLCFGR_SRC_HSE | (PLL_Q << 24) | (PLL_R << 28);
	RCC->CR |= RCC_CR_PLLON;

	/* over-drive is needed above 168 MHz */
	PWR->CR |= PWR_CR_ODEN;
	while (!(PWR->CSR & PWR_CSR_ODRDY))
		;
	PWR->CR |= PWR_CR_ODSWEN;
	while (!(PWR->CSR & PWR_CSR_ODSWRDY))
		;

	/* 5 wait states at 180 MHz; APB1 max 45 MHz, APB2 max 90 MHz */
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN |
		     FLASH_ACR_LATENCY_5WS;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

	while (!(RCC->CR & RCC_CR_PLLRDY))
		;
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
		;
}
