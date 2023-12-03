/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* STM32F446RE memory map and the peripheral registers we use. */
#ifndef __STM32_PEPS_H
#define __STM32_PEPS_H

#include <stdint.h>

#define FLASH_START	0x08000000U
#define FLASH_END	(FLASH_START + 512U * 1024U)
#define SRAM_START	0x20000000U
#define SRAM_END	(SRAM_START + 128U * 1024U)

typedef struct {
	volatile uint32_t ACR;		/* 0x00 access control (wait states, caches) */
	volatile uint32_t KEYR;		/* 0x04 key */
	volatile uint32_t OPTKEYR;	/* 0x08 option key */
	volatile uint32_t SR;		/* 0x0C status */
	volatile uint32_t CR;		/* 0x10 control */
	volatile uint32_t OPTCR;	/* 0x14 option control */
} FLASH_TypeDef;

typedef struct {
	volatile uint32_t CR;		/* 0x00 power control */
	volatile uint32_t CSR;		/* 0x04 power control/status */
} PWR_TypeDef;

typedef struct {
	volatile uint32_t CR;		/* 0x00 clock control */
	volatile uint32_t PLLCFGR;	/* 0x04 PLL configuration */
	volatile uint32_t CFGR;		/* 0x08 clock configuration */
	volatile uint32_t CIR;		/* 0x0C clock interrupt */
	volatile uint32_t AHB1RSTR;	/* 0x10 AHB1 peripheral reset */
	volatile uint32_t AHB2RSTR;	/* 0x14 AHB2 peripheral reset */
	volatile uint32_t AHB3RSTR;	/* 0x18 AHB3 peripheral reset */
	uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;	/* 0x20 APB1 peripheral reset */
	volatile uint32_t APB2RSTR;	/* 0x24 APB2 peripheral reset */
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;	/* 0x30 AHB1 peripheral clock enable */
	volatile uint32_t AHB2ENR;	/* 0x34 AHB2 peripheral clock enable */
	volatile uint32_t AHB3ENR;	/* 0x38 AHB3 peripheral clock enable */
	uint32_t RESERVED2;
	volatile uint32_t APB1ENR;	/* 0x40 APB1 peripheral clock enable */
	volatile uint32_t APB2ENR;	/* 0x44 APB2 peripheral clock enable */
	uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;	/* 0x50 clock enables in low-power mode */
	volatile uint32_t AHB2LPENR;	/* 0x54 */
	volatile uint32_t AHB3LPENR;	/* 0x58 */
	uint32_t RESERVED4;
	volatile uint32_t APB1LPENR;	/* 0x60 */
	volatile uint32_t APB2LPENR;	/* 0x64 */
	uint32_t RESERVED5[2];
	volatile uint32_t BDCR;		/* 0x70 backup domain control */
	volatile uint32_t CSR;		/* 0x74 clock control and status */
	uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;	/* 0x80 spread spectrum */
	volatile uint32_t PLLI2SCFGR;	/* 0x84 PLLI2S configuration */
	volatile uint32_t PLLSAICFGR;	/* 0x88 PLLSAI configuration */
	volatile uint32_t DCKCFGR;	/* 0x8C dedicated clocks */
	volatile uint32_t CKGATENR;	/* 0x90 clock gating */
	volatile uint32_t DCKCFGR2;	/* 0x94 dedicated clocks 2 */
} RCC_TypeDef;

typedef struct {
	volatile uint32_t MODER;	/* 0x00 mode: input/output/alternate/analog */
	volatile uint32_t OTYPER;	/* 0x04 output type: push-pull/open-drain */
	volatile uint32_t OSPEEDR;	/* 0x08 output speed */
	volatile uint32_t PUPDR;	/* 0x0C pull-up/pull-down */
	volatile uint32_t IDR;		/* 0x10 input data */
	volatile uint32_t ODR;		/* 0x14 output data */
	volatile uint32_t BSRR;		/* 0x18 bit set/reset */
	volatile uint32_t LCKR;		/* 0x1C configuration lock */
	volatile uint32_t AFR[2];	/* 0x20 alternate function, pins 0-7 and 8-15 */
} GPIO_TypeDef;

typedef struct {
	volatile uint32_t SR;		/* 0x00 status */
	volatile uint32_t DR;		/* 0x04 data */
	volatile uint32_t BRR;		/* 0x08 baud rate */
	volatile uint32_t CR1;		/* 0x0C control 1 */
	volatile uint32_t CR2;		/* 0x10 control 2 */
	volatile uint32_t CR3;		/* 0x14 control 3 */
	volatile uint32_t GTPR;		/* 0x18 guard time and prescaler */
} USART_TypeDef;

typedef struct {
	volatile uint32_t MEMRMP;	/* 0x00 memory remap */
	volatile uint32_t PMC;		/* 0x04 peripheral mode configuration */
	volatile uint32_t EXTICR[4];	/* 0x08 which port drives each EXTI line */
	uint32_t RESERVED0[2];
	volatile uint32_t CMPCR;	/* 0x20 compensation cell */
	uint32_t RESERVED1[2];
	volatile uint32_t CFGR;		/* 0x2C configuration */
} SYSCFG_TypeDef;

typedef struct {
	volatile uint32_t IMR;		/* 0x00 interrupt mask (1 = enabled) */
	volatile uint32_t EMR;		/* 0x04 event mask */
	volatile uint32_t RTSR;		/* 0x08 rising edge trigger */
	volatile uint32_t FTSR;		/* 0x0C falling edge trigger */
	volatile uint32_t SWIER;	/* 0x10 software interrupt */
	volatile uint32_t PR;		/* 0x14 pending (write 1 to clear) */
} EXTI_TypeDef;

#define PWR	((PWR_TypeDef *)0x40007000)
#define USART2	((USART_TypeDef *)0x40004400)
#define USART3	((USART_TypeDef *)0x40004800)
#define USART1	((USART_TypeDef *)0x40011000)
#define USART6	((USART_TypeDef *)0x40011400)
#define SYSCFG	((SYSCFG_TypeDef *)0x40013800)
#define EXTI	((EXTI_TypeDef *)0x40013C00)
#define GPIOA	((GPIO_TypeDef *)0x40020000)
#define GPIOB	((GPIO_TypeDef *)0x40020400)
#define GPIOC	((GPIO_TypeDef *)0x40020800)
#define GPIOD	((GPIO_TypeDef *)0x40020C00)
#define GPIOE	((GPIO_TypeDef *)0x40021000)
#define GPIOF	((GPIO_TypeDef *)0x40021400)
#define GPIOG	((GPIO_TypeDef *)0x40021800)
#define GPIOH	((GPIO_TypeDef *)0x40021C00)
#define RCC	((RCC_TypeDef *)0x40023800)
#define FLASH	((FLASH_TypeDef *)0x40023C00)

#define RCC_APB1ENR_USART2EN	(1u << 17)
#define RCC_APB1ENR_PWREN	(1u << 28)
#define RCC_APB2ENR_SYSCFGEN	(1u << 14)

#define USART_SR_RXNE		(1u << 5)
#define USART_SR_TXE		(1u << 7)
#define USART_CR1_RE		(1u << 2)
#define USART_CR1_TE		(1u << 3)
#define USART_CR1_UE		(1u << 13)

#endif /* __STM32_PEPS_H */
