# 02 · GPIO and a device driver

Memory-mapped I/O, peripheral clock gating, and a small driver (`init`,
`exit`, `show`) that keeps register access out of the rest of the code.

## What happens

1. `seven_segment_init()` enables the port C clock (writes to an unclocked
   peripheral are ignored) and sets PC1–PC7 to output mode in `MODER`.
2. `seven_segment_show(d)` writes the digit's segments to `BSRR`, which sets
   and clears pins in a single store.
3. `kmain()` reads digits from the console and prints `GPIOC->ODR` after each.

## Read the code

| File | Look for |
|---|---|
| [`common/arch/stm32_peps.h`](../common/arch/stm32_peps.h) | `GPIO_TypeDef` and the `GPIOC` address |
| [`common/drivers/gpio.c`](../common/drivers/gpio.c) | 2 bits per pin in `MODER`/`PUPDR`, the `BSRR` halves |
| [`seven_segment.c`](seven_segment.c) | the segment table and driver interface |
| [`kmain.c`](kmain.c) | the driver used through its interface only |

## Run it

Wire a common-cathode display, one resistor per segment, to PC1–PC7 as listed
at the top of `seven_segment.c`.

```bash
make LESSON=02-gpio-seven-segment flash
```

```
== Lesson 02: GPIO and a seven-segment display ==
GPIOC lives at 0x40020800; its output register ODR at 0x40020814.
Enter a digit 0-9 (anything else blanks the display).
> 7
showing 7   GPIOC->ODR = 0x004c
> x
display off   GPIOC->ODR = 0x0000
```

## Try this

- Add the decimal point as an eighth segment with a `seven_segment_dot()` call.
- Comment out `gpio_clock_enable(GPIOC)`. Why does nothing crash?
- Add a counter mode that shows 0–9 once per second using `delay_ms()`.
