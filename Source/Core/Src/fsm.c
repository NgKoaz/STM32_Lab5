/*
 * fsm.c
 *
 *  Created on: 12 Dec 2023
 *      Author: HP
 */

#include "fsm.h"

ParserState parserState;
UART_COM_State uartState;

uint8_t ok_flag = 0;
uint8_t rst_flag = 0;

uint8_t buffer[BUFFER_SIZE];
uint16_t produce_idx = BUFFER_SIZE - 1;
uint16_t consume_idx = 0;

char msg[32];

void init_parser_fsm(){
	parserState = WAIT_CMD;
	uartState = SLEEP;
}

uint16_t nextIndex(uint16_t index){
	index++;
	if (index >= BUFFER_SIZE)
		return 0;
	return index;
}

void send_value(){
	uint16_t ADC_Value = HAL_ADC_GetValue(&hadc1);
	sprintf(msg, "\r\n!ADC=%d#\r\n", ADC_Value);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 200);
}

void resend_value(){
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 200);
}

void parser_fsm(){
	uint16_t next_produce_idx = nextIndex(produce_idx);
	if (next_produce_idx == consume_idx) return;
	produce_idx = next_produce_idx;
	uint8_t input = buffer[produce_idx];
	switch (parserState){
	case WAIT_CMD:
		if (input == '!'){
			parserState = LISTEN;
		}
		break;
	case LISTEN:
		switch (input){
		case '!':
			break;
		case 'O':
			parserState = GOT_O;
			break;
		case 'R':
			parserState = GOT_R;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case GOT_O:
		switch (input){
		case 'K':
			parserState = GOT_K;
			break;
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case GOT_K:
		switch (input){
		case '#':
			parserState = EXE_OK;
			ok_flag = 1;
			break;
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case EXE_OK:
		switch (input){
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case GOT_R:
		switch (input){
		case 'S':
			parserState = GOT_S;
			break;
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case GOT_S:
		switch (input){
		case 'T':
			parserState = GOT_T;
			break;
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case GOT_T:
		switch (input){
		case '#':
			parserState = EXE_RST;
			rst_flag = 1;
			break;
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	case EXE_RST:
		switch (input){
		case '!':
			parserState = LISTEN;
			break;
		default:
			parserState = WAIT_CMD;
		}
		break;
	default:
		parserState = WAIT_CMD;
	}
}

void uart_fsm(){
	switch(uartState){
	case SLEEP:
		ok_flag = 0;
		if (rst_flag){
			uartState = WAIT_RESPONSE;
			send_value();
			setTimer(UART_COM, 3000);
		}
		break;
	case WAIT_RESPONSE:
		rst_flag = 0;
		if (ok_flag){
			uartState = SLEEP;
		}
		if (getFlagTimer(UART_COM)){
			setTimer(UART_COM, 3000);
			resend_value();
		}
		break;
	default:
		uartState = SLEEP;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2){
		if (produce_idx != consume_idx){
			buffer[consume_idx] = temp;
			consume_idx = nextIndex(consume_idx);
		}
		HAL_UART_Transmit(&huart2, &temp, sizeof(temp), 50);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}
