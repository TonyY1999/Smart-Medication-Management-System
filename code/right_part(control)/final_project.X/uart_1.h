#ifndef UART1_H
#define UART1_H

#include <stdint.h>

// Function declarations
void UART1_init(int prescale);

void UART1_send( unsigned char data);

unsigned char UART1_receive();

void UART1_send_packet(uint8_t* data, uint8_t length);

void UART1_receive_packet(uint8_t* buffer, uint8_t length);

#endif 