/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* A small shell, running unprivileged. */
#include <stdint.h>
#include <cm4.h>
#include <errno.h>
#include <format.h>
#include <kstring.h>
#include <stdio.h>
#include <unistd.h>

static void help(void)
{
	puts("Commands:");
	puts("  pid         getpid()");
	puts("  uptime      uptime(): milliseconds since boot");
	puts("  echo TEXT   write() TEXT back");
	puts("  badptr      write() from an address outside RAM and flash");
	puts("  peek        read a core register directly, without asking the kernel");
	puts("  exit [N]    exit(N)");
	puts("  reboot      reboot()");
}

int main(void)
{
	char line[64];

	printf("User program running: pid %d, unprivileged, on its own stack.\n",
	       getpid());
	help();

	for (;;) {
		printf("$ ");
		readline(line, sizeof(line));

		if (strcmp(line, "pid") == 0) {
			printf("%d\n", getpid());
		} else if (strcmp(line, "uptime") == 0) {
			printf("%u ms\n", uptime());
		} else if (strncmp(line, "echo ", 5) == 0) {
			write(STDOUT_FILENO, line + 5, strlen(line + 5));
			write(STDOUT_FILENO, "\n", 1);
		} else if (strcmp(line, "badptr") == 0) {
			int r = write(STDOUT_FILENO, (const void *)0x10000000, 16);

			printf("write() returned %d; -EFAULT is %d: the kernel refused "
			       "to read that address on our behalf\n", r, -EFAULT);
		} else if (strcmp(line, "peek") == 0) {
			puts("Reading SysTick->VAL from user mode. Core registers are "
			     "privileged, so expect a fault...");
			printf("%u\n", SYSTICK->VAL);
		} else if (strncmp(line, "exit", 4) == 0) {
			int status = 0;

			scan(line + 4, "%d", &status);
			exit(status);
		} else if (strcmp(line, "reboot") == 0) {
			reboot();
		} else if (strcmp(line, "help") == 0) {
			help();
		} else if (line[0] != '\0') {
			printf("unknown command '%s' (try help)\n", line);
		}
	}
}
