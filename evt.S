
.section ".text.evt"

.global evt

.macro handle
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bl handle_exception
	eret
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
