#ifndef H_MMIO
#define H_MMIO

#include "int.h"
#include "assert.h"

static const u64 MMIO_BASE = 0x3F000000;
static const u64 MMIO_SIZE = 0x01000000;

static inline void mmio_write(u64 reg, u32 data) {
	assert((reg >= MMIO_BASE) && (reg < MMIO_BASE + MMIO_SIZE));
	*(volatile u32*)reg = data;
}

static inline u32 mmio_read(u64 reg) {
	assert((reg >= MMIO_BASE) && (reg < MMIO_BASE + MMIO_SIZE));
	return *(volatile u32*)reg;
}

static inline void mmio_spin(u64 cycles) {
	while (cycles-- > 0) {
		asm volatile("nop");
	}
}

#endif
