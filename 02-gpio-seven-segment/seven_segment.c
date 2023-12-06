/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/*
 * Common-cathode seven-segment display, pin high = segment on.
 *
 *   top-left  PC1   top     PC2   top-right     PC3   middle PC7
 *   bot-left  PC4   bottom  PC5   bottom-right  PC6
 */
#include <seven_segment.h>
#include <gpio.h>

#define SEG_TOP_LEFT		GPIO_PIN(1)
#define SEG_TOP			GPIO_PIN(2)
#define SEG_TOP_RIGHT		GPIO_PIN(3)
#define SEG_BOTTOM_LEFT		GPIO_PIN(4)
#define SEG_BOTTOM		GPIO_PIN(5)
#define SEG_BOTTOM_RIGHT	GPIO_PIN(6)
#define SEG_MIDDLE		GPIO_PIN(7)

#define SEG_ALL	(SEG_TOP_LEFT | SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_LEFT | \
		 SEG_BOTTOM | SEG_BOTTOM_RIGHT | SEG_MIDDLE)

static const uint16_t digit_segments[10] = {
	[0] = SEG_TOP | SEG_TOP_LEFT | SEG_TOP_RIGHT |
	      SEG_BOTTOM_LEFT | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
	[1] = SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
	[2] = SEG_TOP | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM,
	[3] = SEG_TOP | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
	[4] = SEG_TOP_LEFT | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_RIGHT,
	[5] = SEG_TOP | SEG_TOP_LEFT | SEG_MIDDLE | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
	[6] = SEG_TOP | SEG_TOP_LEFT | SEG_MIDDLE |
	      SEG_BOTTOM_LEFT | SEG_BOTTOM_RIGHT | SEG_BOTTOM,
	[7] = SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_RIGHT,
	[8] = SEG_ALL,
	[9] = SEG_TOP | SEG_TOP_LEFT | SEG_TOP_RIGHT | SEG_MIDDLE |
	      SEG_BOTTOM_RIGHT | SEG_BOTTOM,
};

int seven_segment_init(void)
{
	gpio_clock_enable(GPIOC);
	gpio_init(GPIOC, SEG_ALL, GPIO_MODE_OUTPUT, GPIO_NOPULL);
	gpio_write(GPIOC, SEG_ALL, 0);
	return 0;
}

void seven_segment_exit(void)
{
	gpio_write(GPIOC, SEG_ALL, 0);
}

int seven_segment_show(int digit)
{
	gpio_write(GPIOC, SEG_ALL, 0);
	if (digit < 0 || digit > 9)
		return -1;
	gpio_write(GPIOC, digit_segments[digit], 1);
	return 0;
}
