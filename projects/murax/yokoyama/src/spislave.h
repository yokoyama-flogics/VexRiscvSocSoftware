#ifndef SPISLAVE_H_
#define SPISLAVE_H_

typedef struct {
	volatile uint32_t RX;
	volatile uint32_t STATUS;
} SpiSlave_Reg;

#define SPISLAVE ((SpiSlave_Reg*) 0xf0012000)

#endif /* SPISLAVE_H_ */
