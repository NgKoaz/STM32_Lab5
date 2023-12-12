/*
 * timer.c
 *
 *  Created on: Dec 12, 2023
 *      Author: HP
 */

#include "timer.h"

uint32_t counter[NO_COUNTER];
uint8_t flag[NO_COUNTER];

uint8_t getFlagTimer(uint8_t index){
	return flag[index];
}
void setTimer(uint8_t index, uint32_t duration){
	counter[index] = duration / TIMER_CYCLE;
	flag[index] = 0;
}
void timerRun(uint8_t index){
	if (counter[index] <= 0) return;
	counter[index]--;
	if (counter[index] <= 0){
		flag[index] = 1;
	}
}

void allTimerRun(void){
	timerRun(LED_BLINKING);
	timerRun(UART_COM);
}

