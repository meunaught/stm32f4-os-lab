/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

#ifndef __ERRNO_H
#define __ERRNO_H

/* syscalls return these negated */
#define ENOSYS	1	/* function not implemented */
#define ENOMEM	3	/* out of memory */
#define EAGAIN	4	/* operation would block */
#define EFAULT	6	/* bad memory reference */
#define EINVAL	8	/* invalid argument */
#define EPERM	9	/* operation not permitted */
#define EBADF	30	/* bad file number */

#endif /* __ERRNO_H */
