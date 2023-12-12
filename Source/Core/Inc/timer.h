/*
 * timer.h
 *
 *  Created on: Dec 12, 2023
 *      Author: HP
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_CYCLE 	10

#define NO_COUNTER 		2
#define LED_BLINKING 	0
#define UART_COM		1


uint8_t getFlagTimer(uint8_t index);
void setTimer(uint8_t index, uint32_t duration);
void allTimerRun(void);

#endif /* INC_TIMER_H_ */
