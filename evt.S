/* --------------------------------------------------------------------------------------------- */
/* Initialize the exception vector tables (EVTs) for the BCM2837 procesor with four Cortex-A53   */
/* cores. Exception levels EL1, EL2 and EL3 have independent EVTs for a total of three EVTs.     */
/* Each EVT contains 16 entries with 32 instructions. ARMv8 instructions have a fixed 4 byte     */
/* width so each entry is 128 bytes. The EVTs must be 2048 byte aligned. Prior to calling the    */
/* exception handler, the caller saved registers (x0-x18) must be saved to the kernel stack.     */
/* These registers must be restored after the exception handler returns. Note that exceptions    */
/* are disabled when entering an exception handler and are restored after an eret.               */
/* --------------------------------------------------------------------------------------------- */

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
