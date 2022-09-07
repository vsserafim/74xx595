/*
 * ATmega328P
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
#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include "misc.h"
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/power.h>
#include <avr/wdt.h>

/*
 *  PIN NAMES AND CONFIGURATION
 */
#ifdef __ARDUINO_UNO__
#define LED_PIN PB5
#define SER_PIN PD3
#define OE_PIN PD4
#define RCLK_PIN PD5
#define SRCLK_PIN PD6
#define SRCLR_PIN PD7
#define HC595_PORT PORTD
#else
#define LED_PIN PB0
#endif
#define LED_PORT PORTB

/*
 *  OUTPUT PORTS
 */
#define PORTB_OUTPUTS (_BV(LED_PIN))
#define PORTC_OUTPUTS (0x0)
#define PORTD_OUTPUTS (_BV(SER_PIN) | _BV(OE_PIN) | _BV(RCLK_PIN) | _BV(SRCLK_PIN) | _BV(SRCLR_PIN))

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
#ifdef __ARDUINO_UNO__
#define CLOCK_DIV clock_div_1 // clock prescaler factor
#else
#define CLOCK_DIV clock_div_8 // clock prescaler factor
#endif

/**
 * Initializes MCU.
 */
static inline void mcu_init()
{

#ifndef WATCHDOG_ENABLED
    MCUSR = 0;
    wdt_disable();
#endif

    // Set clock prescaler
    clock_prescale_set(CLOCK_DIV);

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

    /* DIGITAL INPUT DISABLE REGISTER (DIDR) 0
     * Disable ADC pins */
    DIDR0 |= _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) |
             _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);

    /* DIGITAL INPUT DISABLE REGISTER (DIDR) 1
     * Disable AIN (Analog Comparator) */
    DIDR1 |= _BV(AIN1D) | _BV(AIN0D);

    /* ANALOG COMPARATOR CONTROL AND STATUS REGISTER (ACSR)
     * Power off Analog Comparator */
    ACSR |= _BV(ACD);

    /* ADC CONTROL AND STATUS REGISTER (ADCSRA)
     * Disable ADC Analog-to-Digital Converter */
    ADCSRA &= ~_BV(ADEN);

    /* SPI CONTROL REGISTER (SPCR)
     * Disable Serial Peripheral Interface */
    SPCR &= ~_BV(SPE);

    /* POWER REDUCTION REGISTER (PRR)
     * On AVR devices all peripherals are enabled from power on reset.
     * Disables all peripherals to save power. */
    PRR = _BV(PRSPI) | _BV(PRTIM2) | _BV(PRTIM0) | _BV(PRTIM1) |
          _BV(PRTWI) | _BV(PRUSART0) | _BV(PRADC);
}

#endif /* ATMEGA328P_H */
