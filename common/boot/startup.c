/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#include <stdint.h>
#include <cm4.h>

/* from linker.ld */
extern uint32_t _estack;
extern uint32_t _la_data, _sdata, _edata, _sbss, _ebss;

void kmain(void);

void Reset_Handler(void)
{
	uint32_t *src = &_la_data;
	for (uint32_t *dst = &_sdata; dst < &_edata;)
		*dst++ = *src++;

	for (uint32_t *dst = &_sbss; dst < &_ebss;)
		*dst++ = 0;

	fpu_enable();

	kmain();

	for (;;)
		;
}

void Default_Handler(void)
{
	for (;;)
		;
}

#define WEAK_DEFAULT	__attribute__((weak, alias("Default_Handler")))

void NMI_Handler(void) WEAK_DEFAULT;
void HardFault_Handler(void) WEAK_DEFAULT;
void MemManage_Handler(void) WEAK_DEFAULT;
void BusFault_Handler(void) WEAK_DEFAULT;
void UsageFault_Handler(void) WEAK_DEFAULT;
void SVCall_Handler(void) WEAK_DEFAULT;
void DebugMonitor_Handler(void) WEAK_DEFAULT;
void PendSV_Handler(void) WEAK_DEFAULT;
void SysTick_Handler(void) WEAK_DEFAULT;
void WWDG_Handler(void) WEAK_DEFAULT;
void PVD_Handler(void) WEAK_DEFAULT;
void TAMP_STAMP_Handler(void) WEAK_DEFAULT;
void RTC_WKUP_Handler(void) WEAK_DEFAULT;
void FLASH_Handler(void) WEAK_DEFAULT;
void RCC_Handler(void) WEAK_DEFAULT;
void EXTI0_Handler(void) WEAK_DEFAULT;
void EXTI1_Handler(void) WEAK_DEFAULT;
void EXTI2_Handler(void) WEAK_DEFAULT;
void EXTI3_Handler(void) WEAK_DEFAULT;
void EXTI4_Handler(void) WEAK_DEFAULT;
void DMA1_Stream0_Handler(void) WEAK_DEFAULT;
void DMA1_Stream1_Handler(void) WEAK_DEFAULT;
void DMA1_Stream2_Handler(void) WEAK_DEFAULT;
void DMA1_Stream3_Handler(void) WEAK_DEFAULT;
void DMA1_Stream4_Handler(void) WEAK_DEFAULT;
void DMA1_Stream5_Handler(void) WEAK_DEFAULT;
void DMA1_Stream6_Handler(void) WEAK_DEFAULT;
void ADC_Handler(void) WEAK_DEFAULT;
void CAN1_TX_Handler(void) WEAK_DEFAULT;
void CAN1_RX0_Handler(void) WEAK_DEFAULT;
void CAN1_RX1_Handler(void) WEAK_DEFAULT;
void CAN1_SCE_Handler(void) WEAK_DEFAULT;
void EXTI9_5_Handler(void) WEAK_DEFAULT;
void TIM1_BRK_TIM9_Handler(void) WEAK_DEFAULT;
void TIM1_UP_TIM10_Handler(void) WEAK_DEFAULT;
void TIM1_TRG_COM_TIM11_Handler(void) WEAK_DEFAULT;
void TIM1_CC_Handler(void) WEAK_DEFAULT;
void TIM2_Handler(void) WEAK_DEFAULT;
void TIM3_Handler(void) WEAK_DEFAULT;
void TIM4_Handler(void) WEAK_DEFAULT;
void I2C1_EV_Handler(void) WEAK_DEFAULT;
void I2C1_ER_Handler(void) WEAK_DEFAULT;
void I2C2_EV_Handler(void) WEAK_DEFAULT;
void I2C2_ER_Handler(void) WEAK_DEFAULT;
void SPI1_Handler(void) WEAK_DEFAULT;
void SPI2_Handler(void) WEAK_DEFAULT;
void USART1_Handler(void) WEAK_DEFAULT;
void USART2_Handler(void) WEAK_DEFAULT;
void USART3_Handler(void) WEAK_DEFAULT;
void EXTI15_10_Handler(void) WEAK_DEFAULT;
void RTC_Alarm_Handler(void) WEAK_DEFAULT;
void OTG_FS_WKUP_Handler(void) WEAK_DEFAULT;
void TIM8_BRK_TIM12_Handler(void) WEAK_DEFAULT;
void TIM8_UP_TIM13_Handler(void) WEAK_DEFAULT;
void TIM8_TRG_COM_TIM14_Handler(void) WEAK_DEFAULT;
void TIM8_CC_Handler(void) WEAK_DEFAULT;
void DMA1_Stream7_Handler(void) WEAK_DEFAULT;
void FMC_Handler(void) WEAK_DEFAULT;
void SDIO_Handler(void) WEAK_DEFAULT;
void TIM5_Handler(void) WEAK_DEFAULT;
void SPI3_Handler(void) WEAK_DEFAULT;
void UART4_Handler(void) WEAK_DEFAULT;
void UART5_Handler(void) WEAK_DEFAULT;
void TIM6_DAC_Handler(void) WEAK_DEFAULT;
void TIM7_Handler(void) WEAK_DEFAULT;
void DMA2_Stream0_Handler(void) WEAK_DEFAULT;
void DMA2_Stream1_Handler(void) WEAK_DEFAULT;
void DMA2_Stream2_Handler(void) WEAK_DEFAULT;
void DMA2_Stream3_Handler(void) WEAK_DEFAULT;
void DMA2_Stream4_Handler(void) WEAK_DEFAULT;
void CAN2_TX_Handler(void) WEAK_DEFAULT;
void CAN2_RX0_Handler(void) WEAK_DEFAULT;
void CAN2_RX1_Handler(void) WEAK_DEFAULT;
void CAN2_SCE_Handler(void) WEAK_DEFAULT;
void OTG_FS_Handler(void) WEAK_DEFAULT;
void DMA2_Stream5_Handler(void) WEAK_DEFAULT;
void DMA2_Stream6_Handler(void) WEAK_DEFAULT;
void DMA2_Stream7_Handler(void) WEAK_DEFAULT;
void USART6_Handler(void) WEAK_DEFAULT;
void I2C3_EV_Handler(void) WEAK_DEFAULT;
void I2C3_ER_Handler(void) WEAK_DEFAULT;
void OTG_HS_EP1_OUT_Handler(void) WEAK_DEFAULT;
void OTG_HS_EP1_IN_Handler(void) WEAK_DEFAULT;
void OTG_HS_WKUP_Handler(void) WEAK_DEFAULT;
void OTG_HS_Handler(void) WEAK_DEFAULT;
void DCMI_Handler(void) WEAK_DEFAULT;
void FPU_Handler(void) WEAK_DEFAULT;
void SPI4_Handler(void) WEAK_DEFAULT;
void SAI1_Handler(void) WEAK_DEFAULT;
void SAI2_Handler(void) WEAK_DEFAULT;
void QuadSPI_Handler(void) WEAK_DEFAULT;
void HDMI_CEC_Handler(void) WEAK_DEFAULT;
void SPDIF_Rx_Handler(void) WEAK_DEFAULT;
void FMPI2C1_Handler(void) WEAK_DEFAULT;
void FMPI2C1_ERR_Handler(void) WEAK_DEFAULT;

__attribute__((section(".isr_vector"), used))
const uint32_t vector_table[] = {
	(uint32_t)&_estack,
	(uint32_t)&Reset_Handler,
	(uint32_t)&NMI_Handler,
	(uint32_t)&HardFault_Handler,
	(uint32_t)&MemManage_Handler,
	(uint32_t)&BusFault_Handler,
	(uint32_t)&UsageFault_Handler,
	0,	/* reserved */
	0,	/* reserved */
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&SVCall_Handler,
	(uint32_t)&DebugMonitor_Handler,
	0,	/* reserved */
	(uint32_t)&PendSV_Handler,
	(uint32_t)&SysTick_Handler,
	(uint32_t)&WWDG_Handler,
	(uint32_t)&PVD_Handler,
	(uint32_t)&TAMP_STAMP_Handler,
	(uint32_t)&RTC_WKUP_Handler,
	(uint32_t)&FLASH_Handler,
	(uint32_t)&RCC_Handler,
	(uint32_t)&EXTI0_Handler,
	(uint32_t)&EXTI1_Handler,
	(uint32_t)&EXTI2_Handler,
	(uint32_t)&EXTI3_Handler,
	(uint32_t)&EXTI4_Handler,
	(uint32_t)&DMA1_Stream0_Handler,
	(uint32_t)&DMA1_Stream1_Handler,
	(uint32_t)&DMA1_Stream2_Handler,
	(uint32_t)&DMA1_Stream3_Handler,
	(uint32_t)&DMA1_Stream4_Handler,
	(uint32_t)&DMA1_Stream5_Handler,
	(uint32_t)&DMA1_Stream6_Handler,
	(uint32_t)&ADC_Handler,
	(uint32_t)&CAN1_TX_Handler,
	(uint32_t)&CAN1_RX0_Handler,
	(uint32_t)&CAN1_RX1_Handler,
	(uint32_t)&CAN1_SCE_Handler,
	(uint32_t)&EXTI9_5_Handler,
	(uint32_t)&TIM1_BRK_TIM9_Handler,
	(uint32_t)&TIM1_UP_TIM10_Handler,
	(uint32_t)&TIM1_TRG_COM_TIM11_Handler,
	(uint32_t)&TIM1_CC_Handler,
	(uint32_t)&TIM2_Handler,
	(uint32_t)&TIM3_Handler,
	(uint32_t)&TIM4_Handler,
	(uint32_t)&I2C1_EV_Handler,
	(uint32_t)&I2C1_ER_Handler,
	(uint32_t)&I2C2_EV_Handler,
	(uint32_t)&I2C2_ER_Handler,
	(uint32_t)&SPI1_Handler,
	(uint32_t)&SPI2_Handler,
	(uint32_t)&USART1_Handler,
	(uint32_t)&USART2_Handler,
	(uint32_t)&USART3_Handler,
	(uint32_t)&EXTI15_10_Handler,
	(uint32_t)&RTC_Alarm_Handler,
	(uint32_t)&OTG_FS_WKUP_Handler,
	(uint32_t)&TIM8_BRK_TIM12_Handler,
	(uint32_t)&TIM8_UP_TIM13_Handler,
	(uint32_t)&TIM8_TRG_COM_TIM14_Handler,
	(uint32_t)&TIM8_CC_Handler,
	(uint32_t)&DMA1_Stream7_Handler,
	(uint32_t)&FMC_Handler,
	(uint32_t)&SDIO_Handler,
	(uint32_t)&TIM5_Handler,
	(uint32_t)&SPI3_Handler,
	(uint32_t)&UART4_Handler,
	(uint32_t)&UART5_Handler,
	(uint32_t)&TIM6_DAC_Handler,
	(uint32_t)&TIM7_Handler,
	(uint32_t)&DMA2_Stream0_Handler,
	(uint32_t)&DMA2_Stream1_Handler,
	(uint32_t)&DMA2_Stream2_Handler,
	(uint32_t)&DMA2_Stream3_Handler,
	(uint32_t)&DMA2_Stream4_Handler,
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&CAN2_TX_Handler,
	(uint32_t)&CAN2_RX0_Handler,
	(uint32_t)&CAN2_RX1_Handler,
	(uint32_t)&CAN2_SCE_Handler,
	(uint32_t)&OTG_FS_Handler,
	(uint32_t)&DMA2_Stream5_Handler,
	(uint32_t)&DMA2_Stream6_Handler,
	(uint32_t)&DMA2_Stream7_Handler,
	(uint32_t)&USART6_Handler,
	(uint32_t)&I2C3_EV_Handler,
	(uint32_t)&I2C3_ER_Handler,
	(uint32_t)&OTG_HS_EP1_OUT_Handler,
	(uint32_t)&OTG_HS_EP1_IN_Handler,
	(uint32_t)&OTG_HS_WKUP_Handler,
	(uint32_t)&OTG_HS_Handler,
	(uint32_t)&DCMI_Handler,
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&FPU_Handler,
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&SPI4_Handler,
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&SAI1_Handler,
	0,	/* reserved */
	0,	/* reserved */
	0,	/* reserved */
	(uint32_t)&SAI2_Handler,
	(uint32_t)&QuadSPI_Handler,
	(uint32_t)&HDMI_CEC_Handler,
	(uint32_t)&SPDIF_Rx_Handler,
	(uint32_t)&FMPI2C1_Handler,
	(uint32_t)&FMPI2C1_ERR_Handler,
};
