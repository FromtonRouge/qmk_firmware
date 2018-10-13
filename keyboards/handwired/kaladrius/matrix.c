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

static matrix_row_t matrix[MATRIX_ROWS];

static uint8_t mcp23018_reset_loop;
uint8_t mcp23018_status = 0x20;
bool i2c_initialized = false;

void init_cols(void)
{
    // The original int_cols() function in quantum/matrix.c 
    // configure every column pins as input with pull up resistor
    // Columns (inputs):    C0 C1 C2
    // Note from teensy documentation:
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    DDRC &= ~(1<<0 | 1<<1 | 1<<2);
    PORTC |= (1<<0 | 1<<1 | 1<<2);
}

void unselect_rows(void)
{
    // The original unselect_rows() function in quantum/matrix.c
    // configure every row pins as normal input without pullup resistor
    // Note from teensy documentation:
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    DDRC &= ~(1<<6 | 1<<7);
    PORTC &= ~(1<<6 | 1<<7);
}

void select_row(uint8_t row)
{
    // The original select_row() function in quantum/matrix.c
    // configure the row pin as an output and write 0 (low) to the pin
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Set Output (when DDRx=1): 0=Low Output, 1=High Output
    switch (row)
    {
    case 0:
        {
            DDRC  |= (1<<6);
            PORTC &= ~(1<<6);
            break;
        }
    case 1:
        {
            DDRC  |= (1<<7);
            PORTC &= ~(1<<7);
            break;
        }
    default:
        {
            break;
        }
    }
}

void unselect_row(uint8_t row)
{
    // The original unselect_row() function in quantum/matrix.c
    // configure the row pin as an input with pull up resistor
    //  DDRx:   0=Input, 1=Output
    //  PORTx:  Config Input (when DDRx=0): 0=Normal, 1=Pullup Resistor
    switch (row)
    {
    case 0:
        {
            DDRC &= ~(1<<6);
            PORTC |= (1<<6);
            break;
        }
    case 1:
        {
            DDRC &= ~(1<<7);
            PORTC |= (1<<7);
            break;
        }
    default:
        {
            break;
        }
    }
}

bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row)
{
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selecton to stabilize
    select_row(current_row);
    wait_us(30);

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; ++col_index)
    {
        // Select the col pin to read (active low)
        bool pin_state = false;

        switch (col_index)
        {
        case 0:
            {
                pin_state = (PINC & (1<<0)) == (1<<0);
                break;
            }
        case 1:
            {
                pin_state = (PINC & (1<<1)) == (1<<1);
                break;
            }
        case 2:
            {
                pin_state = (PINC & (1<<2)) == (1<<2);
                break;
            }
        default:
            {
                break;
            }
        }

        // Populate the matrix row with the state of the col pin
        current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

uint8_t init_mcp23018(void)
{
    mcp23018_status = 0x20;

    if (!i2c_initialized)
    {
        i2c_init();
        _delay_ms(1000);
        i2c_initialized = true;
    }

    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(IODIRA);            if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b11111111);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00000000);        if (mcp23018_status) goto out;
    i2c_stop();

    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPPUA);             if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b11111111);        if (mcp23018_status) goto out;
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

void matrix_init(void)
{
    // Same as quantum/matrix.c
    unselect_rows();
    init_cols();
    
    init_mcp23018();
    
    // Same as quantum/matrix.c
    // Initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
    }

    matrix_init_quantum();
}

void check_mcp23018_connection(void)
{
    if (mcp23018_status)
    { 
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
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row)
    {
        read_cols_on_row(matrix, row);
    }

    matrix_scan_quantum();
    return 1;
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
