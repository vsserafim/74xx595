/*
 * mcusetup.h
 *
 * Vinícius da Silveira Serafim <vinicius@serafim.eti.br>
 */

#ifndef MCU_SETUP_H
#define MCU_SETUP_H

#ifdef __AVR_ATmega328P__
#include "mcu_setup/atmega328p.h"

#elif __AVR_ATmega8__
#include "mcu_setup/atmega8.h"

#elif __AVR_ATtiny85__
#include "mcu_setup/attiny85.h"

#elif __AVR_ATtiny13A__
#include "mcu_setup/attiny13a.h"

#endif

#endif // MCU_SETUP_H

// eof