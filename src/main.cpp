/*
 *  main.cpp
 *
 * Vinícius da Silveira Serafim <vinicius@serafim.eti.br>
 */

#include "main.h"

// To use EEPROM see:
// http://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

static void print_commands();

static void pulse_srclk()
{
    HC595_PORT |= _BV(SRCLK_PIN);
    _delay_us(1);
    HC595_PORT &= ~_BV(SRCLK_PIN);
}

static void pulse_rclk()
{
    HC595_PORT |= _BV(RCLK_PIN);
    _delay_us(1);
    HC595_PORT &= ~_BV(RCLK_PIN);
}

static void print_commands()
{
    usart_sendString("s -> toggle SER value\n");
    usart_sendString("o -> toggle OE value\n");
    usart_sendString("r -> pulse RCLK\n");
    usart_sendString("k -> pulse SRCLK\n");
    usart_sendString("c -> toggle SRCLR\n");
    usart_sendString("1 -> send byte 1 + SRCLK\n");
    usart_sendString("0 -> send byte 0 + SRCLK\n");
    usart_sendString("t -> show status\n");
}

/**
 * Main
 */
int main(void)
{
    /* mcu_init must be called first */
    mcu_init();

    /* here goes the setup not covered by mcu_init */

#ifdef BAUD
    usart_init();
    usart_rx(true);
    usart_sendString("avr-pio-template\n");
#endif

    print_commands();

    // disable OE
    HC595_PORT |= _BV(OE_PIN);

    /* main loop */
    while (1)
        main_loop();

    // as long as you have an infinite loop above, this will never happen
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_mode();
}

/**
 * Main loop
 */
static inline void main_loop()
{
    // read a byte from serial and do switch
    uint8_t byte = usart_readByte();

    switch (byte)
    {
    case 's': // toggle SER input
        HC595_PORT ^= _BV(SER_PIN);
        if (HC595_PORT & _BV(SER_PIN))
        {
            LED_ON;
            usart_sendString("SER high\n");
        }
        else
        {
            LED_OFF;
            usart_sendString("SER low\n");
        }
        break;
    case 'o': // toggle output enable
        HC595_PORT ^= _BV(OE_PIN);
        if (HC595_PORT & _BV(OE_PIN))
            usart_sendString("OE off\n");
        else
            usart_sendString("OE on\n");
        break;
    case 'r': // generate pulse on RCLK
        pulse_rclk();
        usart_sendString("RCLK\n");
        break;
    case 'k': // generate pulse on SRCLK
        pulse_srclk();
        usart_sendString("SRCLK\n");
        break;
    case 'c': // toggle SRCLR
        HC595_PORT ^= _BV(SRCLR_PIN);
        if (HC595_PORT & _BV(SRCLR_PIN))
            usart_sendString("SRCLR off\n");
        else
            usart_sendString("SRCLR on\n");
        break;
    case '1':
        HC595_PORT |= _BV(SER_PIN);
        pulse_srclk();
        usart_sendString("1");
        break;
    case '0':
        HC595_PORT &= ~_BV(SER_PIN);
        pulse_srclk();
        usart_sendString("0");
        break;
    case '\r':
    case '\n':
        usart_sendString("\n");
        break;
    case 't':
        usart_sendString("---\nOE: ");
        if (HC595_PORT & _BV(OE_PIN))
            usart_sendString("off\n");
        else
            usart_sendString("on\n");
        usart_sendString("SER: ");
        if (HC595_PORT & _BV(SER_PIN))
            usart_sendString("high\n");
        else
            usart_sendString("low\n");
        usart_sendString("SRCLR: ");
        if (HC595_PORT & _BV(SRCLR_PIN))
            usart_sendString("off\n");
        else
            usart_sendString("on\n");
        break;
    default:
        usart_sendString("Unknown command: ");
        usart_sendByte(byte);
        usart_sendString("\n");
    case 'h':
        print_commands();
        break;
    }
}

// eof
