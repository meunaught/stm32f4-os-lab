# DUOS

A small teaching OS for the STM32F446RE (Cortex-M4) on a
[NUCLEO-F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html)
board, written from scratch: no vendor HAL, no C library, no RTOS.

Each lesson adds one OS concept on top of a shared core and builds into its
own firmware image.

| Lesson | Topics |
|---|---|
| [01 · Boot](01-boot/) | vector table, linker script, C start-up, clock tree, console, SysTick |
| [02 · GPIO and a driver](02-gpio-seven-segment/) | memory-mapped I/O, peripheral clocks, driver interface |
| [03 · Interrupts and exceptions](03-interrupts/) | NVIC, priorities, PRIMASK vs. BASEPRI, deferred work, faults |
| [04 · System calls](04-system-calls/) | user mode, MSP/PSP, `svc`, syscall ABI, crt0 |
| [05 · Scheduling](05-scheduling/) | TCBs, context switching, preemption, round robin vs. FCFS |
| [06 · Synchronization](06-synchronization/) | race conditions, counting semaphores, producer/consumer |

Only lesson 02 needs extra hardware: a common-cathode seven-segment display
on PC1–PC7.

## Layout

```
common/                     shared by every lesson
  boot/      linker.ld, startup.c, fault.c, sys_init.c
  arch/      cm4.c/.h (SysTick, NVIC), stm32_peps.h (register map)
  drivers/   clock.c, gpio.c, usart.c
  lib/       kstdio.c, format.c, kstring.c
01-boot/                    kmain.c
02-gpio-seven-segment/      kmain.c, seven_segment.c
03-interrupts/              kmain.c, button.c
04-system-calls/            kernel/, user/, app.c
05-scheduling/              lesson.mk, kernel/, user/, app.c
06-synchronization/         lesson.mk, kernel/, user/, app.c
```

Lessons 05 and 06 build on earlier lessons. `LAYERS` in their `lesson.mk`
lists the lessons underneath, and a file in a later lesson replaces the file
at the same path in an earlier one. Diff the two to see what a lesson changes.

## Build and flash

You need the [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads),
[stlink-tools](https://github.com/stlink-org/stlink) and GNU `make`/`find`.
On Windows, use Git Bash or WSL.

```bash
# Ubuntu/Debian
sudo apt install gcc-arm-none-eabi stlink-tools make
```

```bash
# macOS
brew install stlink
brew install --cask gcc-arm-embedded
```

```bash
make probe                          # check the board is visible
make LESSON=03-interrupts           # build into build/03-interrupts/
make LESSON=03-interrupts flash     # build and flash
make lessons                        # build every lesson
```

The console is the ST-LINK virtual COM port at 115200 baud, 8N1. Press RESET
after connecting. `st-flash` may need `sudo`.

In VS Code, *Build lesson* builds a lesson and *Debug lesson (ST-LINK)* debugs
it with Cortex-Debug.

## History

This started as coursework for the operating systems lab at CSE, University
of Dhaka (2023). Each assignment had its own branch (`se7en-segment`, `exti`,
`syscall`, `multitasking`, `semaphore`). They were merged into `main` and
reworked into the lessons above; the merge commits keep each branch as it was.

## Credits and license

Starter code: CSE Batch 25 and Prof. Mosaddek Tushar, University of Dhaka.
Lab work: Mahadi and Joyee. BSD-style license, see [LICENSE](LICENSE).
