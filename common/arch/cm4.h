/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Cortex-M4 core peripherals (SCB, SysTick, NVIC, DWT) and intrinsics. */
#ifndef __CM4_H
#define __CM4_H

#include <stdint.h>

/* System Control Block */
typedef struct {
	volatile uint32_t CPUID;	/* 0x00 CPUID base */
	volatile uint32_t ICSR;		/* 0x04 interrupt control and state */
	volatile uint32_t VTOR;		/* 0x08 vector table offset */
	volatile uint32_t AIRCR;	/* 0x0C application interrupt and reset control */
	volatile uint32_t SCR;		/* 0x10 system control */
	volatile uint32_t CCR;		/* 0x14 configuration and control */
	volatile uint8_t  SHPR[12];	/* 0x18 priorities of system exceptions 4-15 */
	volatile uint32_t SHCSR;	/* 0x24 system handler control and state */
	volatile uint32_t CFSR;		/* 0x28 configurable fault status (MemManage, BusFault, UsageFault) */
	volatile uint32_t HFSR;		/* 0x2C HardFault status */
	volatile uint32_t DFSR;		/* 0x30 debug fault status */
	volatile uint32_t MMFAR;	/* 0x34 MemManage fault address */
	volatile uint32_t BFAR;		/* 0x38 BusFault address */
	volatile uint32_t AFSR;		/* 0x3C auxiliary fault status */
	volatile uint32_t PFR[2];	/* 0x40 processor features */
	volatile uint32_t DFR;		/* 0x48 debug features */
	volatile uint32_t AFR;		/* 0x4C auxiliary features */
	volatile uint32_t MMFR[4];	/* 0x50 memory model features */
	volatile uint32_t ISAR[5];	/* 0x60 instruction set attributes */
	uint32_t RESERVED[5];
	volatile uint32_t CPACR;	/* 0x88 coprocessor access control */
} SCB_TypeDef;

#define SCB			((SCB_TypeDef *)0xE000ED00)

#define SCB_ICSR_PENDSVSET	(1u << 28)	/* make PendSV pending */
#define SCB_AIRCR_VECTKEY	(0x05FAu << 16)	/* AIRCR ignores writes without this key */
#define SCB_AIRCR_PRIGROUP_Msk	(7u << 8)
#define SCB_AIRCR_SYSRESETREQ	(1u << 2)
#define SCB_CCR_DIV_0_TRP	(1u << 4)	/* fault on integer division by zero */
#define SCB_SHCSR_MEMFAULTENA	(1u << 16)
#define SCB_SHCSR_BUSFAULTENA	(1u << 17)
#define SCB_SHCSR_USGFAULTENA	(1u << 18)
#define SCB_HFSR_FORCED		(1u << 30)	/* a configurable fault escalated to HardFault */
#define SCB_CFSR_MMARVALID	(1u << 7)	/* MMFAR holds the faulting address */
#define SCB_CFSR_BFARVALID	(1u << 15)	/* BFAR holds the faulting address */
#define SCB_CPACR_FPU_FULL	(0xFu << 20)	/* CP10 + CP11 (the FPU): full access */

/* SysTick */
typedef struct {
	volatile uint32_t CTRL;		/* control and status */
	volatile uint32_t LOAD;		/* reload value */
	volatile uint32_t VAL;		/* current value */
	volatile uint32_t CALIB;	/* calibration */
} SysTick_TypeDef;

#define SYSTICK			((SysTick_TypeDef *)0xE000E010)

#define SYSTICK_CTRL_ENABLE	(1u << 0)
#define SYSTICK_CTRL_TICKINT	(1u << 1)	/* interrupt on reaching zero */
#define SYSTICK_CTRL_CLKSOURCE	(1u << 2)	/* count processor clock cycles */

#define TICK_HZ			1000U

/* NVIC */
typedef struct {
	volatile uint32_t ISER[8];	/* 0x000 set enable */
	uint32_t RESERVED0[24];
	volatile uint32_t ICER[8];	/* 0x080 clear enable */
	uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];	/* 0x100 set pending */
	uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];	/* 0x180 clear pending */
	uint32_t RESERVED3[24];
	volatile uint32_t IABR[8];	/* 0x200 active */
	uint32_t RESERVED4[56];
	volatile uint8_t  IP[240];	/* 0x300 priority, one byte per IRQ */
	uint32_t RESERVED5[644];
	volatile uint32_t STIR;		/* 0xE00 software trigger */
} NVIC_TypeDef;

#define NVIC			((NVIC_TypeDef *)0xE000E100)

/* only the top 4 bits of each priority byte exist: 0 (highest) to 15 */
#define NVIC_PRIO_BITS		4
#define NVIC_LOWEST_PRIORITY	((1u << NVIC_PRIO_BITS) - 1)

typedef enum {
  /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M4 Hard Fault Interrupt                                  */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
  DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
  DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
  DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
  DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
  DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
  DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
  TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
  TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
  TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
  TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare global interrupt                             */
  DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
  FMC_IRQn                    = 48,     /*!< FMC global Interrupt                                              */
  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
  DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
  DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
  DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
  DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
  CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
  CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
  CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
  CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
  DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
  DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
  USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
  OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
  OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
  OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
  DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
  FPU_IRQn                    = 81,     /*!< FPU global interrupt                                              */
  SPI4_IRQn                   = 84,     /*!< SPI4 global Interrupt                                             */
  SAI1_IRQn                   = 87,     /*!< SAI1 global Interrupt                                             */
  SAI2_IRQn                   = 91,     /*!< SAI2 global Interrupt                                             */
  QUADSPI_IRQn                = 92,     /*!< QuadSPI global Interrupt                                          */
  CEC_IRQn                    = 93,     /*!< CEC global Interrupt                                              */
  SPDIF_RX_IRQn               = 94,     /*!< SPDIF-RX global Interrupt                                          */
  FMPI2C1_EV_IRQn             = 95,     /*!< FMPI2C1 Event Interrupt                                           */
  FMPI2C1_ER_IRQn             = 96      /*!< FMPI2C1 Error Interrupt                                           */
} IRQn_Type;

/* DWT cycle counter */
typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t CYCCNT;
} DWT_TypeDef;

#define DWT			((DWT_TypeDef *)0xE0001000)
#define DEMCR			(*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL_CYCCNTENA	(1u << 0)
#define DEMCR_TRCENA		(1u << 24)

#define CONTROL_nPRIV		(1u << 0)
#define CONTROL_SPSEL		(1u << 1)

#define EXC_RETURN_THREAD_PSP	0xFFFFFFFDu
#define EXC_RETURN_SPSEL	(1u << 2)
#define EXC_RETURN_NO_FPU	(1u << 4)	/* clear if FPU regs were stacked */

#define XPSR_THUMB		(1u << 24)

/* pushed by hardware on exception entry */
struct exception_frame {
	uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};

static inline uint32_t __get_CONTROL(void)
{
	uint32_t v;
	__asm volatile("mrs %0, control" : "=r"(v));
	return v;
}

static inline uint32_t __get_MSP(void)
{
	uint32_t v;
	__asm volatile("mrs %0, msp" : "=r"(v));
	return v;
}

static inline uint32_t __get_PSP(void)
{
	uint32_t v;
	__asm volatile("mrs %0, psp" : "=r"(v));
	return v;
}

/* PRIMASK masks everything but NMI/HardFault, BASEPRI only priority >= n */
static inline void __enable_irq(void)
{
	__asm volatile("cpsie i" ::: "memory");
}

static inline void __disable_irq(void)
{
	__asm volatile("cpsid i" ::: "memory");
}

static inline uint32_t __get_PRIMASK(void)
{
	uint32_t v;
	__asm volatile("mrs %0, primask" : "=r"(v));
	return v;
}

static inline void __set_PRIMASK(uint32_t v)
{
	__asm volatile("msr primask, %0" : : "r"(v) : "memory");
}

static inline uint32_t __get_BASEPRI(void)
{
	uint32_t v;
	__asm volatile("mrs %0, basepri" : "=r"(v));
	return v;
}

/* 0 turns BASEPRI masking off */
static inline void __set_BASEPRI(uint32_t priority)
{
	uint32_t v = priority << (8 - NVIC_PRIO_BITS);
	__asm volatile("msr basepri, %0" : : "r"(v) : "memory");
}

static inline uint32_t irq_save(void)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

static inline void irq_restore(uint32_t primask)
{
	__set_PRIMASK(primask);
}

static inline void __DSB(void)
{
	__asm volatile("dsb" ::: "memory");
}

static inline void __ISB(void)
{
	__asm volatile("isb" ::: "memory");
}

static inline void __WFI(void)
{
	__asm volatile("wfi");
}

void fpu_enable(void);

void systick_init(void);
uint32_t get_time_ms(void);
void delay_ms(uint32_t ms);
void systick_hook(void);

void nvic_enable_irq(IRQn_Type irq);
void nvic_disable_irq(IRQn_Type irq);
void nvic_set_priority(IRQn_Type irq, uint32_t priority);
uint32_t nvic_get_priority(IRQn_Type irq);
void nvic_set_pending(IRQn_Type irq);
void nvic_clear_pending(IRQn_Type irq);
int nvic_is_pending(IRQn_Type irq);

void cycle_counter_init(void);

static inline uint32_t cycle_count(void)
{
	return DWT->CYCCNT;
}

void system_reset(void) __attribute__((noreturn));

#endif /* __CM4_H */
