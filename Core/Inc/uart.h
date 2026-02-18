//UART Baremetal Header File

#pragma once

void UART_Init(void);
uint8_t receive_UART(void);
void transmit_UART(uint8_t txData);