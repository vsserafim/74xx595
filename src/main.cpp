/*
 *  main.cpp
 *
 * Vinícius da Silveira Serafim <vinicius@serafim.eti.br>
 */

#include "main.h"

// To use EEPROM see:
// http://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

// Global variables
bool auto_rclk = false;

static void pulse_srclk()
{
    // set SRLCK to high
    HC595_PORT |= _BV(SRCLK_PIN);
    _delay_us(1);
    // set SRLCK to low
    HC595_PORT &= ~_BV(SRCLK_PIN);
}

static void pulse_rclk()
{
    // set RCLK to high
    HC595_PORT |= _BV(RCLK_PIN);
    _delay_us(1);
    // set RCLK to low
    HC595_PORT &= ~_BV(RCLK_PIN);
}

// print valid commands
static void print_commands()
{
    usart_sendString("s -> toggle SER value\n");
    usart_sendString("o -> toggle OE value\n");
    usart_sendString("r -> pulse RCLK\n");
    usart_sendString("k -> pulse SRCLK\n");
    usart_sendString("c -> toggle SRCLR\n");
    usart_sendString("1 -> set SER high + SRCLK\n");
    usart_sendString("0 -> set SER low + SRCLK\n");
    usart_sendString("t -> show status\n");
    usart_sendString("R -> enable/disable auto RCLK\n");
    usart_sendString("H -> halt MCU\n");
    usart_sendString("h -> print this help\n");
}

/**
 * Main
 */
int main(void)
{
    /* mcu_init must be called first */
    mcu_init();

    /* here goes the setup not covered by mcu_init */
    usart_init();
    usart_rx(true);

    // disable OE
    HC595_PORT |= _BV(OE_PIN);

    usart_sendString("avr-pio-template\n");
    print_commands();

    while (1)
        main_loop();

    // this will happen if you press H
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
    // Toggle SER input high/low
    case 's':
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

    // Toggle Output Enable (OE) on/off
    case 'o':
        HC595_PORT ^= _BV(OE_PIN);
        if (HC595_PORT & _BV(OE_PIN))
            usart_sendString("OE off\n");
        else
            usart_sendString("OE on\n");
        break;

    // Generate pulse on RCLK
    case 'r':
        pulse_rclk();
        usart_sendString(" RCLK ");
        break;

    // Generate pulse on SRCLK
    case 'k':
        pulse_srclk();
        usart_sendString(" SRCLK ");
        break;

    // Toggle SRCLR on/off
    case 'c':
        HC595_PORT ^= _BV(SRCLR_PIN);
        if (HC595_PORT & _BV(SRCLR_PIN))
            usart_sendString("\nSRCLR off\n");
        else
            usart_sendString("\nSRCLR on\n");
        break;

    // Set SER high + pulse SRCLK
    case '1':
        HC595_PORT |= _BV(SER_PIN);
        pulse_srclk();

        // if auto RCLK is enabled, pulse RCLK
        if (auto_rclk)
            pulse_rclk();

        // echo
        usart_sendString("1");
        break;

    // Set SER low + pulse SRCLK
    case '0':
        HC595_PORT &= ~_BV(SER_PIN);
        pulse_srclk();

        // if auto RCLK is enabled, pulse RCLK
        if (auto_rclk)
            pulse_rclk();

        // echo
        usart_sendString("0");
        break;

    // Show status
    case 't':
        usart_sendString("\n---\n");

        // Output Enable (OE) status
        usart_sendString("OE: ");
        if (HC595_PORT & _BV(OE_PIN))
            usart_sendString("off\n");
        else
            usart_sendString("on\n");

        // Serial Data (SER) status
        usart_sendString("SER: ");
        if (HC595_PORT & _BV(SER_PIN))
            usart_sendString("high\n");
        else
            usart_sendString("low\n");

        // Serial Clear (SRCLR) status
        usart_sendString("SRCLR: ");
        if (HC595_PORT & _BV(SRCLR_PIN))
            usart_sendString("off\n");
        else
            usart_sendString("on\n");

        // Auto RCLK status
        if (auto_rclk)
            usart_sendString("Auto RCLK: enabled\n");
        else
            usart_sendString("Auto RCLK: disabled\n");

        usart_sendString("---\n");
        break;

    // Enable/disable auto RCLK
    case 'R':
        auto_rclk = !auto_rclk;
        if (auto_rclk)
            usart_sendString("\nAuto RCLK enabled\n");
        else
            usart_sendString("\nAuto RCLK disabled\n");
        break;

    // show help
    case 'h':
        print_commands();
        break;

    // line feed echo
    case '\r':
    case '\n':
        usart_sendString("\n");
        break;

    // halt MCU
    case 'H':
        usart_sendString("MCU halted\n");
        _delay_ms(100);
        // return to main
        return;

    // show error and tip for help
    default:
        usart_sendString("Unknown command: ");
        usart_sendByte(byte);
        usart_sendString("\nPress h for help\n");
    }
}

// eof
