#ifndef H_GPIO
#define H_GPIO

#include "int.h"
#include "mmio.h"
#include "assert.h"

static const u64 GPIO_BASE = MMIO_BASE + 0x200000;
static const u64 GPIO_GPFSEL0 = GPIO_BASE + 0x0;
static const u64 GPIO_GPFSEL1 = GPIO_BASE + 0x4;
static const u64 GPIO_GPFSEL2 = GPIO_BASE + 0x8;
static const u64 GPIO_GPFSEL3 = GPIO_BASE + 0xC;
static const u64 GPIO_GPFSEL4 = GPIO_BASE + 0x10;
static const u64 GPIO_GPFSEL5 = GPIO_BASE + 0x14;
static const u64 GPIO_GPSET0 = GPIO_BASE + 0x1C;
static const u64 GPIO_GPSET1 = GPIO_BASE + 0x20;
static const u64 GPIO_GPCLR0 = GPIO_BASE + 0x28;
static const u64 GPIO_GPCLR1 = GPIO_BASE + 0x2C;
static const u64 GPIO_GPLEV0 = GPIO_BASE + 0x34;
static const u64 GPIO_GPLEV1 = GPIO_BASE + 0x38;
static const u64 GPIO_GPEDS0 = GPIO_BASE + 0x40;
static const u64 GPIO_GPEDS1 = GPIO_BASE + 0x44;
static const u64 GPIO_GPREN0 = GPIO_BASE + 0x4C;
static const u64 GPIO_GPREN1 = GPIO_BASE + 0x50;
static const u64 GPIO_GPFEN0 = GPIO_BASE + 0x58;
static const u64 GPIO_GPFEN1 = GPIO_BASE + 0x5C;
static const u64 GPIO_GPHEN0 = GPIO_BASE + 0x64;
static const u64 GPIO_GPLEN0 = GPIO_BASE + 0x68;
static const u64 GPIO_GPLEN1 = GPIO_BASE + 0x70;
static const u64 GPIO_GPAREN0 = GPIO_BASE + 0x74;
static const u64 GPIO_GPAREN1 = GPIO_BASE + 0x7C;
static const u64 GPIO_GPAFEN0 = GPIO_BASE + 0x80;
static const u64 GPIO_GPAFEN1 = GPIO_BASE + 0x88;
static const u64 GPIO_GPPUD = GPIO_BASE + 0x8C;
static const u64 GPIO_GPPUDCLK0 = GPIO_BASE + 0x98;
static const u64 GPIO_GPPUDCLK1 = GPIO_BASE + 0x9C;

static const u64 GPIO_MODE_INPUT = 0;
static const u64 GPIO_MODE_OUTPUT = 1;
static const u64 GPIO_MODE_ALT0 = 4;
static const u64 GPIO_MODE_ALT1 = 5;
static const u64 GPIO_MODE_ALT2 = 6;
static const u64 GPIO_MODE_ALT3 = 7;
static const u64 GPIO_MODE_ALT4 = 3;
static const u64 GPIO_MODE_ALT5 = 2;

static inline void gpio_set_mode(u8 pin, u8 mode) {
	assert((pin >= 0) && (pin <= 53));
	assert((mode >= 0) && (mode <= 7));
	const u64 pin_to_reg[6] = {
		GPIO_GPFSEL0, GPIO_GPFSEL1, GPIO_GPFSEL2,
		GPIO_GPFSEL3, GPIO_GPFSEL4, GPIO_GPFSEL5,
	};
	const u64 reg = pin_to_reg[pin / 10];
	const u32 data = mode << (3 * (pin % 10));
	const u32 mask = 0x7 << (3 * (pin % 10));
	const u32 state = mmio_read(reg);
	mmio_write(reg, (state & ~mask) | data);
}

static inline void gpio_set_pin(u8 pin) {
	assert((pin >= 0) && (pin <= 53));
	const u64 pin_to_reg[2] = {GPIO_GPSET0, GPIO_GPSET1};
	const u64 reg = pin_to_reg[pin / 32];
	const u64 data = 1 << (pin % 32);
	mmio_write(reg, data);
}

static inline void gpio_clear_pin(u8 pin) {
	assert((pin >= 0) && (pin <= 53));
	const u64 pin_to_reg[2] = {GPIO_GPCLR0, GPIO_GPCLR1};
	const u64 reg = pin_to_reg[pin / 32];
	const u64 data = 1 << (pin % 32);
	mmio_write(reg, data);
}

static inline void gpio_set_clock(u64 pin_mask) {
	const u32 pin_mask0 = pin_mask & 0xFFFFFFFF;
	const u32 pin_mask1 = pin_mask >> 32;
	mmio_write(GPIO_GPPUD, 0x0);
	mmio_spin(150);
	mmio_write(GPIO_GPPUDCLK0, pin_mask0);
	//mmio_write(GPIO_GPPUDCLK1, pin_mask1);
	mmio_spin(150);
	mmio_write(GPIO_GPPUD, 0x0);
	mmio_write(GPIO_GPPUDCLK0, 0x0);
	//mmio_write(GPIO_GPPUDCLK1, 0x0);
}

#endif

