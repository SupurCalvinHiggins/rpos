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

static inline void uart1_init() {
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
	mmio_write(UART1_AUX_MU_IO_REG, c);
}

#endif
