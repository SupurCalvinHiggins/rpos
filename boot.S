.section ".text.boot"

/* ---------------------------------------------------------------------------------- */
/* Kernel startup entry point.                                                        */
/* ---------------------------------------------------------------------------------- */

.global _start
_start:

/* ---------------------------------------------------------------------------------- */
/* Switch to single core mode.                                                        */
/* ---------------------------------------------------------------------------------- */

	// Get the current CPU number.
	mrs x9, mpidr_el1
	and x9, x9, 0xff

	// Halt all cores except for CPU0.
	cbz x9, 2f
1:
	wfe
	b 1b

/* ---------------------------------------------------------------------------------- */
/* Set debug boot flag.                                                               */
/* ---------------------------------------------------------------------------------- */

	// Set GPIO pin 5. This will be cleared when the kernel loads.
	mov x9, 0x3f200000
	mov w10, 0x8000
	str w10, [x9]
	mov w10, 0x20
	str w10, [x9, 0x1c]

/* ---------------------------------------------------------------------------------- */
/* Zero the BSS.                                                                      */
/* ---------------------------------------------------------------------------------- */

2:
	// Load the start and end of the BSS.
	ldr x9, =__bss_start
	ldr x10, =__bss_end
3:
	// While start != end, zero the first element and increment start.
	cmp x9, x10
	beq 4f
	str xzr, [x9]
	add x9, x9, 0x8
	b 3b

/* ---------------------------------------------------------------------------------- */
/* Initialize the stack pointer.                                                      */
/* ---------------------------------------------------------------------------------- */

4:
	// The kernel stack will start above the code and grow upwards.
	ldr x9, =__start
	mov sp, x9

/* ---------------------------------------------------------------------------------- */
/* Switch to EL1                                                                      */
/* ---------------------------------------------------------------------------------- */

	// Load the current exception level.
	mrs x9, CurrentEL

	// If we are in EL3, drop to EL2 and then EL1.
	cmp x9, 0xC
	beq 5f

	// If we are in EL2, drop to EL1.
	cmp x9, 0x8
	beq 6f

	// Otherwise, we must be at EL1.
	b 7f

/* ---------------------------------------------------------------------------------- */
/* Switch from EL3 to EL2                                                             */
/* ---------------------------------------------------------------------------------- */

5:

/* ---------------------------------------------------------------------------------- */
/* Switch from EL2 to EL1                                                             */
/* ---------------------------------------------------------------------------------- */

6:

	// Disable the MMU in EL1.
	msr sctlr_el1, xzr

	// Enable Aarch64 mode in EL1.
	ldr x9, =0x80000002
	msr hcr_el2, x9

	// Set the EL1 processor state.
	mov x9, 0x3c5
	msr spsr_el2, x9

	// Set the address to return to.
	adr x9, 7f
	msr elr_el2, x9

	// Set the EL1 stack pointer.
	mov x9, sp
	msr sp_el1, x9

	// Return to EL1.
	eret

/* ---------------------------------------------------------------------------------- */
/* Enable exceptions and interrupts.                                                  */
/* ---------------------------------------------------------------------------------- */

7:
	// Set up the EVT.
	ldr x9, =__evt_start
	msr vbar_el1, x9

/* ---------------------------------------------------------------------------------- */
/* Start the kernel.                                                                  */
/* ---------------------------------------------------------------------------------- */

	// Branch to main.
	bl main

	// If main returns, halt the core.
	b 1b

/* ---------------------------------------------------------------------------------- */
/* Unused debug code.                                                                 */
/* ---------------------------------------------------------------------------------- */
	
	// Clear GPIO pin 5.
	mov x9, 0x3f200000
	mov w10, 0x20
	str w10, [x9, 0x28]