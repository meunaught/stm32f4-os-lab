# 03 · Interrupts and exceptions

NVIC, priorities, PRIMASK vs. BASEPRI, pending interrupts, deferred work,
fault escalation. Uses the on-board USER button; no wiring needed.

## What happens

- **Button.** A press on PC13 goes SYSCFG → EXTI line 13 → `EXTI15_10_IRQn`
  → `EXTI15_10_Handler`. The handler clears the pending bit, debounces and
  increments a counter. The main loop does the printing (top half / bottom
  half).
- **Masking.** `p` sets PRIMASK, which blocks everything including SysTick.
  `b` sets BASEPRI, which blocks the button but not SysTick. A press while
  masked stays pending and runs when the mask is lifted; several presses
  collapse into one.
- **Faults.** `z` divides by zero with the trap and UsageFault enabled. `u`
  runs an undefined instruction with UsageFault disabled, so it escalates to
  HardFault. `common/boot/fault.c` prints the stacked registers and decodes
  the status registers.

## Read the code

| File | Look for |
|---|---|
| [`button.c`](button.c) | GPIO → SYSCFG → EXTI → NVIC setup, write-1-to-clear |
| [`kmain.c`](kmain.c) | the polling loop, PRIMASK/BASEPRI demos, fault triggers |
| [`common/arch/cm4.c`](../common/arch/cm4.c) | `nvic_enable_irq()`, `nvic_set_priority()` |
| [`common/arch/cm4.h`](../common/arch/cm4.h) | `__disable_irq()`, `__set_BASEPRI()`, `irq_save()`/`irq_restore()` |
| [`common/boot/fault.c`](../common/boot/fault.c) | fault entry stubs, the stacked frame, CFSR/HFSR decoding |

## Run it

```bash
make LESSON=03-interrupts flash
```

```
== Lesson 03: interrupts and exceptions ==
...
[2210 ms] button pressed (1 so far)
PRIMASK=1: every interrupt is held off for 5 s. Press the button now...
  SysTick was masked too: the clock advanced 0 ms in 5 s.
  Button interrupt pending while masked: yes. Presses counted: 1.
  A masked interrupt is remembered once, however often it fired.
Dividing by zero with the trap enabled...

*** UsageFault ***
  pc  0x08000a1c  lr  0x08000b31  xpsr 0x61000000
  ...
  - DIVBYZERO: integer division by zero
System halted. Press RESET to restart.
```

## Try this

- Remove the `EXTI->PR = BUTTON_LINE` acknowledge. What happens after the
  first press?
- Make the button more urgent than SysTick. What does `b` show now?
- Look up the faulting `pc` with
  `arm-none-eabi-addr2line -e build/03-interrupts/duos.elf <pc>`.

## Known limitations

- Debouncing uses the 1 ms tick, so it does nothing while PRIMASK is set.
- The console is polled, not interrupt-driven.
