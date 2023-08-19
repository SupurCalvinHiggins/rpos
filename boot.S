.section ".text.boot"

.global _start

_start:
	mrs x9, mpidr_el1
	and x9, x9, 0xff
	cbz x9, 2f
1:
	wfe
	b 1b
2:
	ldr x9, =__start
	mov sp, x9

	mov x9, 0x3f200000
	mov w10, 0x8000
	str w10, [x9]
	mov w10, 0x20
	// Set
	str w10, [x9, 0x1c]
	bl main
	// Clear
	//str w10, [x9, 0x28]
	b 1b
