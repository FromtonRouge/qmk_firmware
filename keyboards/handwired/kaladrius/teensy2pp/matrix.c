/* Copyright 2018 FromtonRouge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include QMK_KEYBOARD_H
#include "kaladrius.h"
#include "i2cmaster/i2cmaster.h"

// Set 0 if debouncing isn't needed
#ifndef DEBOUNCING_DELAY
#define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
static uint16_t debouncing_time;
static bool debouncing = false;
#endif

#if (MATRIX_COLS <= 8)
#    define print_matrix_header()  print("\nr/c 01234567\n")
#    define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop(matrix[i])
#    define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop16(matrix[i])
#    define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop32(matrix[i])
#    define ROW_SHIFTER  ((uint32_t)1)
#endif

#define I2C_ADDR        0x20 // 0x20 Because the ADDR pin is connected to the ground (@see mcp23018 datasheet)
#define I2C_ADDR_WRITE  ( (I2C_ADDR<<1) | I2C_WRITE )
#define I2C_ADDR_READ   ( (I2C_ADDR<<1) | I2C_READ  )
#define IODIRA          0x00            // i/o direction register
#define IODIRB          0x01
#define GPPUA           0x0C            // GPIO pull-up resistor register
#define GPPUB           0x0D
#define GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define GPIOB           0x13
#define OLATA           0x14            // output latch register
#define OLATB           0x15

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
uint8_t mcp23018_status = 1;
uint8_t g_led_right = 0;

void init_cols(void)
{
    // Init on teensy2pp
    // The original init_cols() function in quantum/matrix.c 
    // configure every column pins as input with pull up resistor
    // Columns:    C0 C1 C2 C3 C4 C5 C6
    // Note from teensy documentation:
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    const uint8_t bits_columns = (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6);
    DDRC &= ~bits_columns; // Input
    PORTC |= bits_columns; // Pullup enabled

    // For mcp23018 the initialization is done in init_mcp23018()
}

void unselect_rows(void)
{
    // Unselect on teensy2pp
    // The original unselect_rows() function in quantum/matrix.c
    // configure every row pins as normal input without pullup resistor
    // Rows:    F0 F1 F2 F3 F4 
    // Note from teensy documentation:
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    const uint8_t bits_rows = (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4);
    DDRF &= ~bits_rows;
    PORTF &= ~bits_rows;

    // Unselect on mcp23018
    if (!mcp23018_status)
    {
        mcp23018_status = i2c_start(I2C_ADDR_WRITE);        if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(GPIOB);                 if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(0b00011111);                  if (mcp23018_status) goto out;
out:
        i2c_stop();
    }
}

void select_row(uint8_t row)
{
    // Select on teensy2pp
    // The original select_row() function in quantum/matrix.c
    // configure the row pin as an output and write 0 (low) to the pin
    // Rows:    F0 F1 F2 F3 F4 
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Set Output (when DDRx=1): 0=Low Output, 1=High Output
    const uint8_t bit_row = (1<<row);
    DDRF  |= bit_row;    // Output
    PORTF &= ~bit_row;   // Write 0

    // Select on mcp23018 and auto-unselect other rows by writing 0xFF first
    if (!mcp23018_status)
    {
        mcp23018_status = i2c_start(I2C_ADDR_WRITE);        if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(GPIOB);                 if (mcp23018_status) goto out;
        uint8_t byte = 0b00011111 & ~(1<<row);
        byte |= (g_led_right << 5);
        mcp23018_status = i2c_write(byte);      if (mcp23018_status) goto out;
out:
        i2c_stop();
    }
}

void unselect_row(uint8_t row)
{
    // Unselect on teensy2pp
    // The original unselect_row() function in quantum/matrix.c
    // configure the row pin as an input with pull up resistor
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    const uint8_t bit_row = (1<<row);
    DDRF &= ~bit_row;
    PORTF |= bit_row;

    // Useless for mcp23018 because select_row() for mcp23018 auto-unselect other rows
}

bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row)
{
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selection to stabilize
    select_row(current_row);
    wait_us(30);

    // Read columns for the teensy part
    bool pin_state = true;
    const uint8_t MATRIX_COLS_HALF = MATRIX_COLS/2;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS_HALF; ++col_index)
    {
        // Select the col pin to read (active low)
        pin_state = (PINC & (1<<col_index)) == (1<<col_index);

        // Populate the matrix row with the state of the col pin
        current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
    }

    // Read columns for the mcp23018 part
    if (!mcp23018_status)
    {
        for (uint8_t col_index = MATRIX_COLS_HALF; col_index < MATRIX_COLS; ++col_index)
        {
            // Select the col pin to read (active low)
            uint8_t data = 0;
            mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
            mcp23018_status = i2c_write(GPIOA);             if (mcp23018_status) goto out;
            mcp23018_status = i2c_start(I2C_ADDR_READ);     if (mcp23018_status) goto out;
            data = i2c_readNak();

            pin_state = (data & (1<<(col_index-MATRIX_COLS_HALF))) == (1<<(col_index-MATRIX_COLS_HALF));
out:
            i2c_stop();

            // Populate the matrix row with the state of the col pin
            current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
        }
    }

    // Unselect row
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

uint8_t init_mcp23018(void)
{
    mcp23018_status = 1;
    static bool i2c_initialized = false;
    if (!i2c_initialized)
    {
        i2c_init();
        _delay_ms(1000);
        i2c_initialized = true;
    }

    // Set pins directions on A and B
    // Columns:     A0 A1 A2 A3 A4 A5 A6
    // Rows:        B0 B1 B2 B3 B4
    // From mcp23018 datasheet:
    //      1 = Pin is configured as an input.
    //      0 = Pin is configured as an output.
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(IODIRA);            if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b01111111);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00000000);        if (mcp23018_status) goto out;
    i2c_stop();

    // Set pull up resistors on ports A and B
    // From mcp23018 datasheet:
    //      1 = Pull-up enabled.
    //      0 = Pull-up disabled.
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPPUA);             if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b01111111);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00000000);        if (mcp23018_status) goto out;
out:
    i2c_stop();

    return mcp23018_status;
}

__attribute__ ((weak)) void matrix_init_user(void) {}
__attribute__ ((weak)) void matrix_scan_user(void) {}
__attribute__ ((weak)) void matrix_init_kb(void) { matrix_init_user(); }
__attribute__ ((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }
uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }

void set_leds(bool red, bool green, bool blue)
{
    DDRB |= (1<<4 | 1<<5 | 1<<6);    // Output

    g_led_right = 0;
    if (red)
    {
        PORTB |= (1<<4);
        g_led_right |= (1<<0);
    }
    else
    {
        PORTB &= ~(1<<4);
    }

    if (green)
    {
        PORTB |= (1<<5);
        g_led_right |= (1<<1);
    }
    else
    {
        PORTB &= ~(1<<5);
    }

    if (blue)
    {
        PORTB |= (1<<6);
        g_led_right |= (1<<2);
    }
    else
    {
        PORTB &= ~(1<<6);
    }
}

void blink_leds(void)
{
    const uint8_t delay = 30;
    set_leds(true, false, false);
    _delay_ms(delay);
    set_leds(false, true, false);
    _delay_ms(delay);
    set_leds(false, false, true);
    _delay_ms(delay);
    set_leds(true, true, false);
    _delay_ms(delay);
    set_leds(false, true, true);
    _delay_ms(delay);
    set_leds(true, false, true);
    _delay_ms(delay);
    set_leds(false, false, false);
}

void matrix_init(void)
{
    blink_leds();
    
    // Same as quantum/matrix.c
    unselect_rows();
    init_cols();
    init_mcp23018();
    
    // Same as quantum/matrix.c
    // Initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

void check_mcp23018_connection(void)
{
    if (mcp23018_status)
    { 
        static uint8_t mcp23018_reset_loop = 0;
        if (++mcp23018_reset_loop == 0)
        {
            print("trying to reset mcp23018\n");
            mcp23018_status = init_mcp23018();
            if (mcp23018_status)
            {
                print("left side not responding\n");
            }
            else
            {
                print("left side attached\n");
            }
        }
    }
}

uint8_t matrix_scan(void)
{
    check_mcp23018_connection();

    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; ++current_row)
    {
#if (DEBOUNCING_DELAY > 0)
        const bool matrix_changed = read_cols_on_row(matrix_debouncing, current_row);
        if (matrix_changed)
        {
            debouncing = true;
            debouncing_time = timer_read();
        }
#else
        read_cols_on_row(matrix, current_row);
#endif
    }

#if (DEBOUNCING_DELAY > 0)
    if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY))
    {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++)
        {
            matrix[i] = matrix_debouncing[i];
        }
        debouncing = false;
    }
#endif

    matrix_scan_quantum();
    return 1;
}

bool matrix_is_modified(void)
{
#if (DEBOUNCING_DELAY > 0)
    if (debouncing) return false;
#endif
    return true;
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row)
    {
        phex(row); print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; ++i)
    {
        count += matrix_bitpop(i);
    }
    return count;
}
