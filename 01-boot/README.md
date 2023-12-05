# 01 · From reset to `kmain()`

Vector table, linker script, `.data`/`.bss` set-up, clock tree, console,
SysTick.

## What happens

1. On reset the CPU reads the initial stack pointer and the address of
   `Reset_Handler` from the first two words of the vector table.
2. `Reset_Handler` copies `.data` from flash to SRAM, zeroes `.bss`, enables
   the FPU and calls `kmain()`.
3. `sys_init()` switches from the 16 MHz internal oscillator to 180 MHz via
   the PLL, sets up USART2 as the console and starts a 1 ms SysTick.

## Read the code

| File | Look for |
|---|---|
| [`common/boot/linker.ld`](../common/boot/linker.ld) | `MEMORY`, section order, `_sdata`/`_ebss`/`_estack` |
| [`common/boot/startup.c`](../common/boot/startup.c) | `Reset_Handler`, the vector table, weak aliases |
| [`common/drivers/clock.c`](../common/drivers/clock.c) | HSE → PLL → 180 MHz, flash wait states |
| [`common/drivers/usart.c`](../common/drivers/usart.c) | pin alternate functions, baud-rate divisor |
| [`common/arch/cm4.c`](../common/arch/cm4.c) | `systick_init()`, `SysTick_Handler` |
| [`kmain.c`](kmain.c) | prints the memory map, checks `.data`/`.bss` |

## Run it

```bash
make LESSON=01-boot flash
```

Addresses and sizes will differ:

```
== Lesson 01: from reset to kmain() ==

Memory map (from the linker script):
  .text  0x08000000-0x080019c4   6596 bytes  code + constants, in flash
  .data  0x20000000-0x20000004      4 bytes  copied to SRAM from flash 0x080019c4
  .bss   0x20000004-0x20000010     12 bytes  zeroed in SRAM
  stack  starts at 0x20020000 and grows down; sp is now 0x2001ffd8

Reset_Handler did its job if these hold:
  answer = 42 (initialised in .data, expect 42)
  zeroed = 0 (in .bss, expect 0)
...
> hello
[5321 ms] hello
```

## Try this

- Comment out the `.data` copy loop in `Reset_Handler`. What does `answer`
  print, and why?
- Set `TICK_HZ` in `cm4.h` to 100, then fix `SysTick_Handler` so
  `get_time_ms()` stays correct.
- Find `answer` and `zeroed` in `build/01-boot/duos.map`.
