# Tips

## MCU Setup

Setup your device ports and peripherals editing the specific mcu_setup header file.

**IMPORTANT: Peripherals are disabled by default and all ports are setted to work as inputs with pull-ups.**

The header files can be found in the following directory:

```text
include/mcu_setup/
```

## Serial BAUD rate

- BAUD rate limit is related to MCU Freq (F_CPU)

- Configure BAUD rate in the platformio.ini only for devices with serial ports

```ini
build_flags = -D BAUD=19200
monitor_speed = 19200
```

- Reference: datasheet page 172 20.3.1 Internal Clock Generation

- <http://wormfood.net/avrbaudcalc.php>

## Fuses

- Never put your F_CPU bellow 2000UL (2kHz)! (This is the limit for avrdude)
- Never disable SPIEN fuse!
- Never enable RSTDISBL fuse!

If you do any of those things, you will have to do a High-Voltage Programming
to recover access to your MCU.

Use this site for fuses calculation: <https://www.engbedded.com/fusecalc/>

Adjust fuses values in the platformio.ini file.

## Datasheets, Application notes and other useful documents

AVR documents and much much more can be found in the following link: <https://www.microchip.com/doclisting/TechDoc.aspx>.

## Author

Vinícius Serafim <vinicius@serafim.eti.br>
