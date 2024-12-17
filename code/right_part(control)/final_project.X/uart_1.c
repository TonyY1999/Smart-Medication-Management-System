#include "uart_1.h"
#include <avr/io.h>

void UART1_init(int BAUD_PRESCALER)
{
    /*Set baud rate */
    UBRR1H = (unsigned char)(BAUD_PRESCALER>>8);
    UBRR1L = (unsigned char)BAUD_PRESCALER;
    //Enable receiver and transmitter
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    /* Set frame format: 1 stop bits, 8 data bits */
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8 data bits
    UCSR1C &= ~(1<<USBS1); // 1 stop bit
}

void UART1_send(unsigned char data)
{
    // Wait for empty transmit buffer
    while(!(UCSR1A & (1<<UDRE1)));
    // Put data into buffer and send data
    UDR1 = data;
}

unsigned char UART1_receive()
{
    // Wait for receiving data
    while (!(UCSR1A & (1 << RXC1))) {
//        UART_putstring("Waiting for RXC1...\r\n");
    }
    // Return received data
    return UDR1;
}

void UART1_send_packet(uint8_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        UART1_send(data[i]);
    }
}

void UART1_receive_packet(uint8_t *buffer, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        buffer[i] = UART1_receive();
    }
}