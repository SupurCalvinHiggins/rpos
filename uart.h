#ifndef H_UART
#define H_UART

#include "int.h"
#include "mmio.h"
#include "gpio.h"
#include "assert.h"

static const u64 UART1_BASE = MMIO_BASE + 0x215000;
static const u64 UART1_AUX_IRQ = UART1_BASE + 0x0;
static const u64 UART1_AUX_ENABLES = UART1_BASE + 0x4;
static const u64 UART1_AUX_MU_IO_REG = UART1_BASE + 0x40;
static const u64 UART1_AUX_MU_IER_REG = UART1_BASE + 0x44;
static const u64 UART1_AUX_MU_IIR_REG = UART1_BASE + 0x48;
static const u64 UART1_AUX_MU_LCR_REG = UART1_BASE + 0x4C;
static const u64 UART1_AUX_MU_MCR_REG = UART1_BASE + 0x50;
static const u64 UART1_AUX_MU_LSR_REG = UART1_BASE + 0x54;
static const u64 UART1_AUX_MU_MSR_REG = UART1_BASE + 0x58;
static const u64 UART1_AUX_MU_SCRATCH = UART1_BASE + 0x5C;
static const u64 UART1_AUX_MU_CNTL_REG = UART1_BASE + 0x60;
static const u64 UART1_AUX_MU_STAT_REG = UART1_BASE + 0x64;
static const u64 UART1_AUX_MU_BAUD_REG = UART1_BASE + 0x68;

static void uart1_init() {
	// Set UART pins to UART mode.
	gpio_set_mode(14, GPIO_MODE_ALT5);
	gpio_set_mode(15, GPIO_MODE_ALT5);

	// Set clock for UART pins.
	gpio_set_clock((1 << 14) | (1 << 15));

	// Enable the UART.
	mmio_write(UART1_AUX_ENABLES, 0x1);

	// Disable flow control, transmit and receive.
	mmio_write(UART1_AUX_MU_CNTL_REG, 0x0);
	mmio_write(UART1_AUX_MU_MCR_REG, 0X0);

	// Disable UART interrupts.
	mmio_write(UART1_AUX_MU_IER_REG, 0x0);

	// Set UART 8-bit mode.
	mmio_write(UART1_AUX_MU_LCR_REG, 0x3);

	// Set UART baudrate to 115200.
	mmio_write(UART1_AUX_MU_BAUD_REG, 270);

	// Enable transmit and receive.
	mmio_write(UART1_AUX_MU_CNTL_REG, 0x3);
}

static inline void uart1_putc(u8 c) {
	while (!(mmio_read(UART1_AUX_MU_STAT_REG) & (1 << 1)));
	mmio_write(UART1_AUX_MU_IO_REG, c);
}

static void uart1_puts(u8* s) {
	while (*s != 0) uart1_putc(*s++);
}

static void uart1_putu(u64 n) {
	uart1_putc('0');
	uart1_putc('x');
	const u8 nibble_to_char[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};
	const u64 mask = 0xFL << 60;
	for (int i = 0; i < 16; ++i) {
		const u8 c = nibble_to_char[(n & mask) >> 60];
		uart1_putc(c);
		n <<= 4;
	}
}

#endif
