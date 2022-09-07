/*
 * USART (Serial Interface)
 *
 * Very simple USART implementation.
 *
 * ATmega328P
 * ATmega8
 */

#include "usart.h"

/*
 * Usage tip:
 *
 * 1) create a buffer the size you need
 * char buffer[BUFFER_SIZE];
 *
 * 2) format your data and put it into your buffer
 * snprintf(buffer, BUFFER_SIZE, "printf format\n", var1, var2,...);
 *
 * 3) send your buffer out
 * usart_sendString(buffer);
 */

/**
 * Send a null terminated string
 * @param str
 */
void usart_sendString(const char s[])
{
    uint8_t i = 0;

    while (s[i] != '\0')
    {
        usart_sendByte(s[i]);
        i++;
    }
}