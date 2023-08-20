#include "gpio.h"
#include "uart.h"
#include "mmio.h"


void main() {
	uart1_init();
	gpio_clear_pin(5);
	uart1_puts((u8*)"Hello world!\r\n");
	uart1_putu(0x1234567890ABCDEF);
	while (1);
}
