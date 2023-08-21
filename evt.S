
.section ".text.evt"

.global evt

.macro handle
	// Save caller saved registers.
	stp x0, x1, [sp, -16]!
	stp x2, x3, [sp, -16]!
	stp x4, x5, [sp, -16]!
	stp x6, x7, [sp, -16]!
	stp x8, x9, [sp, -16]!
	stp x10, x11, [sp, -16]!
	stp x12, x13, [sp, -16]!
	stp x14, x15, [sp, -16]!
	stp x16, x17, [sp, -16]!
	stp x18, x19, [sp, -16]!
	// Call exception handler.
	bl handle_exception
	// Restore caller saved registers.
	ldp x18, x19, [sp, 16]!
	ldp x16, x17, [sp, 16]!
	ldp x14, x15, [sp, 16]!
	ldp x12, x13, [sp, 16]!
	ldp x10, x11, [sp, 16]!
	ldp x8, x9, [sp, 16]!
	ldp x6, x7, [sp, 16]!
	ldp x4, x5, [sp, 16]!
	ldp x2, x3, [sp, 16]!
	ldp x0, x1, [sp, 16]!
	// Return from exception.
	eret
	// Align to 128 bytes.
	.align 7
.endm

evt:
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
	handle
