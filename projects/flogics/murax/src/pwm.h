#ifndef PWM_H_
#define PWM_H_

typedef struct {
	volatile uint32_t WIDTH;
} Pwm_Reg;

#define PWM ((Pwm_Reg*) 0xf0011000)

#endif /* PWM_H_ */
