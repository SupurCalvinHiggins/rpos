#include "gpio.h"
#include "uart.h"
#include "mmio.h"


void handle_exception() {
	uart1_puts((u8*)"Exception!\r\n");
}


void main() {
	// Bootstrap complete.
	gpio_clear_pin(5);

	// Init UART1.
	uart1_init();
	uart1_puts((u8*)"UART1 init completed.\r\n");

	// Generate a test exception.
	uart1_puts((u8*)"Throwing an exception...\r\n");
	asm volatile("svc 0");

	// Ensure control flow is returned.
	uart1_puts((u8*)"Recovered from exception.\r\n");

	// Fallback.
	while (1);
}
