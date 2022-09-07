/*
 * misc.h
 *
 * Vinícius da Silveira Serafim <vinicius@serafim.eti.br>
 */

#ifndef MISC_H
#define MISC_H

#include <avr/io.h>

// Bit manipulation macros
#define BITSET(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define BITCLEAR(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define BITINVERT(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))
// for more bit operations check:
// http://www.nongnu.org/avr-libc/user-manual/group__avr__sfr.html

#endif /* MISC_H */
