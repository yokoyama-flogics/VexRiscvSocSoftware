#include <stdint.h>

#define CORE_HZ 16000000
#include <murax.h>
#include "pwm.h"
#include "spislave.h"


void wait_tx_ready(int len)
{
	volatile uint32_t save = GPIO_A->OUTPUT;

	while ((UART->STATUS >> 16) < len) {
		// To show CPU is still running
		GPIO_A->OUTPUT ^= 0x80; // Toggle led 7
	}

	GPIO_A->OUTPUT = save;	// Recover original value
}

void print_hex(unsigned int v, int digits)
{
	char c;

	for (int i = digits - 1; i >= 0; i --) {
		c = (v & (0xf << (i * 4))) >> (i * 4);
		if (c < 10)
			c += '0';
		else
			c = c - 10 + 'a';
		UART->DATA = c;
	}
}

void crlf(void)
{
	UART->DATA = 0x0d;
	UART->DATA = 0x0a;
}

void main() {
	volatile uint32_t a = 1, b = 2, c = 3;
	uint32_t result = 0;

	interruptCtrl_init(TIMER_INTERRUPT);
	prescaler_init(TIMER_PRESCALER);
	timer_init(TIMER_A);

	TIMER_PRESCALER->LIMIT = 16000 - 1;	// 1 ms rate

	TIMER_A->LIMIT = 1000 - 1;	// 1 second rate
	TIMER_A->CLEARS_TICKS = 0x00010002;

	TIMER_INTERRUPT->PENDINGS = 0xF;
	TIMER_INTERRUPT->MASKS = 0x1;

	GPIO_A->OUTPUT_ENABLE = 0x000000FF;
	GPIO_A->OUTPUT = 0x00000000;

	UART->STATUS = 2;	// Enable RX interrupts
	UART->DATA = 'A';

	PWM->WIDTH = 0;		// Initialize PWM

	while (1) {
		result += a;
		result += b + c;

		for (uint32_t idx = 0; idx < 50000; idx++)
			asm volatile("");

		// Counter on LED[5:0]
		GPIO_A->OUTPUT =
			(GPIO_A->OUTPUT & ~0x3F) | ((GPIO_A->OUTPUT + 1) & 0x3F);

		// Check UART Rx break
		if (UART->ERROR & (1 << 8))
			GPIO_A->OUTPUT ^= 0x80; // Toggle led 7

		// Test of SimpleAxi4Master
		wait_tx_ready(8 + 2 + 1);
		crlf();
		print_hex(*((volatile int*) 0x90000f00), 8);
	}
}

void irqCallback(){
	if (TIMER_INTERRUPT->PENDINGS & 1) {	// Timer A interrupt
		GPIO_A->OUTPUT ^= 0x80; // Toggle led 7
		TIMER_INTERRUPT->PENDINGS = 1;
	}

	while (UART->STATUS & (1 << 9)) { // UART RX interrupt
		volatile int rx = (UART->DATA) & 0xFF;
		UART->DATA = rx;
		if (rx >= '0' && rx <= '9')
			PWM->WIDTH = (rx - '0') * (255 / 9); // Assign PWM width
	}

	while (SPISLAVE->STATUS & 1) {	// SPI SLAVE interrupt
		volatile int rx = (SPISLAVE->RX) & 0xffff;

		wait_tx_ready(4 + 2 + 1);
		crlf();
		print_hex(rx, 4);
	}
}
