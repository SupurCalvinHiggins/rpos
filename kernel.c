#include "gpio.h"
#include "uart.h"
#include "mmio.h"


void main() {
	uart1_init();
	gpio_clear_pin(5);
	while (1) {
		uart1_putc('a');
		if (mmio_read(UART1_AUX_MU_STAT_REG) & (1 << 3)) gpio_clear_pin(5);
		else gpio_set_pin(5);
	}
}
