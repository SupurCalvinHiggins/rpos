.section ".text.boot"

.global _start

_start:
	// x9 is the CPU core number.
	mrs x9, mpidr_el1
	and x9, x9, 0xff

	// Halt all cores except for CPU0.
	cbz x9, 2f
1:
	wfe
	b 1b
2:
	// Set up the stack pointer.
	ldr x9, =__start
	mov sp, x9

	// Zero the BSS.
	ldr x9, =__bss_start
	ldr x10, =__bss_end
3:
	cmp x9, x10
	beq 4f
	str xzr, [x9]
	add x9, x9, 0x8
	b 3b

4:
	// Set GPIO pin 5.
	mov x9, 0x3f200000
	mov w10, 0x8000
	str w10, [x9]
	mov w10, 0x20
	str w10, [x9, 0x1c]

	// Set up the EVT. Note that we start in EL2.
	ldr x9, =__evt_start
	msr vbar_el1, x9
	msr vbar_el2, x9

	// Clear GPIO pin 5.
	//mov x9, 0x3f200000
	//mov w10, 0x20
	//str w10, [x9, 0x28]

	// Branch to main.
	bl main

	// If main returns, halt the core.
	b 1b
