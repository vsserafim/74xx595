/*
 * ATmega8
 *
 * Vinícius da Silveira Serafim <vinicius@serafim.eti.br>
 */

/* Note on F_CPU versus USART maximum baud rate
 *
 * F_CPU @ 460kHz baud rates @ 300,600,1200,2400,4800,9600,14400 (error 0.2%)
 *
 * Bellow 460kHz USART gets too much error to work safely.
 *
 * http://wormfood.net/avrbaudcalc.php
 */

#ifndef ATMEGA8P_H
#define ATMEGA8P_H

#include "misc.h"
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/power.h>
#include <avr/wdt.h>

static inline void mcu_init();

/*
 *  PIN NAMES AND CONFIGURATION
 */
#define LED_PIN PB0
#define LED_PORT PORTB

/*
 *  OUTPUT PORTS
 */
#define PORTB_OUTPUTS (_BV(LED_PIN))
#define PORTC_OUTPUTS (0x0)
#define PORTD_OUTPUTS (0x0)

/*
 * AIN PINS
 */
#define USED_AIN_PINS (0x0)

/*
 * WATCHDOG
 */
//#define WATCHDOG_ENABLED

/*
 * CPU FREQUENCY & CLOCK DIVIDER
 *
 * *** WARNING!!!! ***
 * Always check your MCU Clock Fuses before using/altering the CLOCK PRESCALER.
 *
 * Never put your F_CPU bellow 2000UL (2kHz)! (This is the limit for avrdude)
 *
 * If you do this, you will have to do a High-Voltage Programming to recover
 * access to your MCU.
 *
 * ATENTION: F_CPU defined in platformio.ini (board_build.f_cpu)
 */
/* No clock prescale on ATmega8 */

/**
 * Initializes MCU.
 */
static inline void mcu_init()
{

#ifndef WATCHDOG_ENABLED
    MCUSR = 0;
    wdt_disable();
#endif

    /* No clock prescale on ATmega8 */

    /* DATA DIRECTION REGISTER
     * Set output ports */
    DDRB = PORTB_OUTPUTS;
    DDRC = PORTC_OUTPUTS;
    DDRD = PORTD_OUTPUTS;

    /* All remaining ports stays as INPUTs (default) and now
     * turn on PULL-UPs (see AVR4013). */
    PORTB = ~PORTB_OUTPUTS;
    PORTC = ~PORTC_OUTPUTS;
    PORTD = ~(PORTD_OUTPUTS | USED_AIN_PINS);

    /* --- CHANGE ONLY THINGS BELLOW THIS LINE --- */

    /* No DIDR registers on Atmega8 */

    /* SPI CONTROL REGISTER (SPCR)
     * Disable Serial Peripheral Interface */
    SPCR &= ~_BV(SPE);

    /* ADC CONTROL AND STATUS REGISTER (ADCSRA)
     * Disable ADC Analog-to-Digital Converter */
    ADCSRA &= ~_BV(ADEN);

    /* ANALOG COMPARATOR CONTROL AND STATUS REGISTER (ACSR)
     * Power off Analog Comparator */
    ACSR |= _BV(ACD);
}

#endif /* ATMEGA8P_H */
