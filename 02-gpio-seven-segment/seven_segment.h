/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __SEVEN_SEGMENT_H
#define __SEVEN_SEGMENT_H

int seven_segment_init(void);
void seven_segment_exit(void);
int seven_segment_show(int digit);	/* -1 and blank if not 0-9 */

#endif /* __SEVEN_SEGMENT_H */
