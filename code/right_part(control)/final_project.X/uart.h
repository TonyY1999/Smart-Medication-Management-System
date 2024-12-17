#ifndef UART_H
#define UART_H

// Function declarations
void UART_init(int prescale);

void UART_send( unsigned char data);

void UART_putstring(char* StringPtr);

#endif 