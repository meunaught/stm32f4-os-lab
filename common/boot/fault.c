/*
 * DUOS: a teaching operating system for the STM32F446RE (Cortex-M4).
 * Copyright (c) 2022, Computer Science and Engineering, University of Dhaka.
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar.
 * See LICENSE at the top of the repository for terms.
 */

/* Fault handlers: print the stacked frame and decode CFSR/HFSR. */
#include <cm4.h>
#include <kstdio.h>

#define FAULT_HARD	3
#define FAULT_MEMMANAGE	4
#define FAULT_BUS	5
#define FAULT_USAGE	6

/*
 * Naked so nothing is pushed before we grab the frame. lr bit 2 says which
 * stack it is on. Branch, not call, so lr still holds EXC_RETURN.
 */
#define FAULT_ENTRY(handler, number)				\
	__attribute__((naked)) void handler(void)		\
	{							\
		__asm volatile(					\
			"tst   lr, #4             \n"		\
			"ite   eq                 \n"		\
			"mrseq r0, msp            \n"		\
			"mrsne r0, psp            \n"		\
			"mov   r1, lr             \n"		\
			"movs  r2, #" #number "   \n"		\
			"b     fault_report       \n");		\
	}

FAULT_ENTRY(HardFault_Handler, 3)
FAULT_ENTRY(MemManage_Handler, 4)
FAULT_ENTRY(BusFault_Handler, 5)
FAULT_ENTRY(UsageFault_Handler, 6)

static const char *const fault_names[] = {
	[FAULT_HARD] = "HardFault",
	[FAULT_MEMMANAGE] = "MemManage fault",
	[FAULT_BUS] = "BusFault",
	[FAULT_USAGE] = "UsageFault",
};

static const struct {
	uint32_t bit;
	const char *meaning;
} cfsr_bits[] = {
	{ 1u << 0,  "IACCVIOL: instruction fetch from a protected or non-executable address" },
	{ 1u << 1,  "DACCVIOL: data access violation" },
	{ 1u << 8,  "IBUSERR: bus error while fetching an instruction" },
	{ 1u << 9,  "PRECISERR: bus error on a data access (address in BFAR)" },
	{ 1u << 10, "IMPRECISERR: bus error on a data access (address unknown)" },
	{ 1u << 12, "STKERR: bus error while stacking registers for an exception" },
	{ 1u << 16, "UNDEFINSTR: undefined instruction" },
	{ 1u << 17, "INVSTATE: invalid execution state (e.g. branch to an even address)" },
	{ 1u << 18, "INVPC: invalid EXC_RETURN value on exception return" },
	{ 1u << 19, "NOCP: FPU instruction while the FPU is disabled" },
	{ 1u << 24, "UNALIGNED: unaligned memory access" },
	{ 1u << 25, "DIVBYZERO: integer division by zero" },
};

/* overridden by the scheduler to kill only the faulting task */
__attribute__((weak)) int fault_kill_current_task(void)
{
	return 0;
}

void fault_report(struct exception_frame *frame, uint32_t exc_return,
		  uint32_t exception)
{
	uint32_t cfsr = SCB->CFSR;
	uint32_t hfsr = SCB->HFSR;
	int from_task = (exc_return & EXC_RETURN_SPSEL) != 0;

	kprintf("\n*** %s ***\n", fault_names[exception]);
	kprintf("  pc  0x%08x  lr  0x%08x  xpsr 0x%08x\n",
		frame->pc, frame->lr, frame->xpsr);
	kprintf("  r0  0x%08x  r1  0x%08x  r2   0x%08x  r3 0x%08x  r12 0x%08x\n",
		frame->r0, frame->r1, frame->r2, frame->r3, frame->r12);
	kprintf("  running on the %s\n",
		from_task ? "process stack (PSP): a user task faulted"
			  : "main stack (MSP): kernel or handler code faulted");
	kprintf("  CFSR 0x%08x  HFSR 0x%08x\n", cfsr, hfsr);

	for (unsigned i = 0; i < sizeof(cfsr_bits) / sizeof(cfsr_bits[0]); i++)
		if (cfsr & cfsr_bits[i].bit)
			kprintf("  - %s\n", cfsr_bits[i].meaning);
	if (cfsr & SCB_CFSR_MMARVALID)
		kprintf("  - MMFAR 0x%08x\n", SCB->MMFAR);
	if (cfsr & SCB_CFSR_BFARVALID)
		kprintf("  - BFAR 0x%08x\n", SCB->BFAR);
	if (hfsr & SCB_HFSR_FORCED)
		kprintf("  - FORCED: escalated to HardFault because the specific "
			"fault handler is disabled\n");

	/* sticky, write 1 to clear */
	SCB->CFSR = cfsr;
	SCB->HFSR = hfsr;

	if (from_task && fault_kill_current_task())
		return;

	kprintf("System halted. Press RESET to restart.\n");
	for (;;)
		;
}
