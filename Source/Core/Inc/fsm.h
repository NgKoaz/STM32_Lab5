/*
 * fsm.h
 *
 *  Created on: 12 Dec 2023
 *      Author: HP
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "timer.h"

#define BUFFER_SIZE 	128

typedef enum {
	WAIT_CMD,
	LISTEN,
	GOT_O,
	GOT_K,
	EXE_OK,
	GOT_R,
	GOT_T,
	GOT_S,
	EXE_RST
} ParserState;

typedef enum {
	SLEEP,
	WAIT_RESPONSE
} UART_COM_State;

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

extern uint8_t temp;

void init_parser_fsm(void);
void parser_fsm(void);
void uart_fsm(void);

#endif /* INC_FSM_H_ */
