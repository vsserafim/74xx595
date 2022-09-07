/*
 * USART (Serial Interface)
 *
 * Very simple USART implementation.
 *
 * ATmega328P
 * ATmega8
 */

#ifndef USART_H
#define USART_H

#include <math.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <string.h>

#ifdef BAUD
// This calculates UBRRH_VALUE and UBRRL_VALUE
#include <util/setbaud.h>
#endif

#define BITSET(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define BITCLEAR(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define BITINVERT(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))

void usart_sendString(const char[]);

/**
 * USART enable and configure
 */
static inline void usart_init()
{

#ifdef __AVR_ATmega328P__

    // Enable USART
    BITCLEAR(PRR, PRUSART0);

    // Set BAUD rate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Transmitter Enabled
    BITSET(UCSR0B, TXEN0);

    // Receiver Disabled
    BITCLEAR(UCSR0B, RXEN0);

    // 8-N-1
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

#elif __AVR_ATmega8__
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;

    // Transmitter Enable
    BITSET(UCSRB, TXEN);

    // Receiver disabled
    BITCLEAR(UCSRB, RXEN);

    // 8-N-1
    UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

#endif
}

/**
 * Check if there is data to be read.
 * @return
 */
static inline bool usart_dataready()
{
#ifdef __AVR_ATmega328P__
    return bit_is_set(UCSR0A, RXC0);
#elif __AVR_ATmega8__
    return bit_is_set(UCSRA, RXC);
#else
    return false;
#endif
}

/**
 * Enable/disable rx
 * @param enable
 */
static inline void usart_rx(bool enable)
{
#ifdef __AVR_ATmega328P__
    if (enable)
        BITSET(UCSR0B, RXEN0);
    else
        BITCLEAR(UCSR0B, RXEN0);
#elif __AVR_ATmega8__
    if (enable)
        BITSET(UCSRB, RXEN);
    else
        BITCLEAR(UCSRB, RXEN);
#endif
}

/**
 * Read one byte
 * @return
 */
static inline uint8_t usart_readByte()
{
#ifdef __AVR_ATmega328P__
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
#elif __AVR_ATmega8__
    loop_until_bit_is_set(UCSRA, RXC);
    return UDR;
#else
    return '\0';
#endif
}

/**
 * Send one byte
 * @param byte
 */
static inline void usart_sendByte(uint8_t byte)
{
#ifdef __AVR_ATmega328P__
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = byte;
#elif __AVR_ATmega8__
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = byte;
#endif
}

#endif /* USART_H */
