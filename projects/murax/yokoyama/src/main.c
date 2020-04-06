#include <stdint.h>

#define CORE_HZ 16000000
#include <murax.h>
#include "pwm.h"


void main() {
	volatile uint32_t a = 1, b = 2, c = 3;
	uint32_t result = 0;

	interruptCtrl_init(TIMER_INTERRUPT);
	prescaler_init(TIMER_PRESCALER);
	timer_init(TIMER_A);

	TIMER_PRESCALER->LIMIT = 16000-1; //1 ms rate

	TIMER_A->LIMIT = 1000-1;  //1 second rate
	TIMER_A->CLEARS_TICKS = 0x00010002;

	TIMER_INTERRUPT->PENDINGS = 0xF;
	TIMER_INTERRUPT->MASKS = 0x1;

	GPIO_A->OUTPUT_ENABLE = 0x000000FF;
	GPIO_A->OUTPUT = 0x00000000;

	UART->STATUS = 2; //Enable RX interrupts
	UART->DATA = 'A';

	PWM->WIDTH = 0; // Initialize PWM

	while(1){
		result += a;
		result += b + c;
		for(uint32_t idx = 0;idx < 50000;idx++) asm volatile("");
		GPIO_A->OUTPUT = (GPIO_A->OUTPUT & ~0x3F) | ((GPIO_A->OUTPUT + 1) & 0x3F);  //Counter on LED[5:0]
	}
}

void irqCallback(){
	if(TIMER_INTERRUPT->PENDINGS & 1){  //Timer A interrupt
		GPIO_A->OUTPUT ^= 0x80; //Toogle led 7
		TIMER_INTERRUPT->PENDINGS = 1;
        // *((int *)0xf0011000) += 1;
	}
	while(UART->STATUS & (1 << 9)){ //UART RX interrupt
        volatile int rx = (UART->DATA) & 0xFF;
		UART->DATA = rx;
		if (rx >= '0' && rx <= '9')
			PWM->WIDTH = (rx - '0') * (255 / 9); // Assign PWM width
	}
}