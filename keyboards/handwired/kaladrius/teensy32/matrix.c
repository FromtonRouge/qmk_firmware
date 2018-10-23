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
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include QMK_KEYBOARD_H

static uint16_t debouncing_time;
static bool debouncing = false;

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

#define MCP_I2C_ADDR        0x20 // 0x20 Because the ADDR pin is connected to the ground (@see mcp23018 datasheet)
#define MCP_I2C_ADDR_WRITE  (MCP_I2C_ADDR<<1)
#define MCP_I2C_ADDR_READ   ((MCP_I2C_ADDR<<1)|1)
#define MCP_IODIRA          0x00            // i/o direction register
#define MCP_IODIRB          0x01
#define MCP_GPPUA           0x0C            // GPIO pull-up resistor register
#define MCP_GPPUB           0x0D
#define MCP_GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define MCP_GPIOB           0x13
#define MCP_OLATA           0x14            // output latch register
#define MCP_OLATB           0x15

static matrix_row_t matrix[MATRIX_ROWS];
static uint8_t matrix_debouncing[MATRIX_COLS];

__attribute__ ((weak)) void matrix_init_user(void) {}
__attribute__ ((weak)) void matrix_scan_user(void) {}
__attribute__ ((weak)) void matrix_init_kb(void) { matrix_init_user(); }
__attribute__ ((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }
uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }

static const I2CConfig i2c_config =
{
    400000 // clock speed (Hz);
};

void matrix_init(void)
{
    // Columns (strobe)
    palSetPadMode(TEENSY_PIN0_IOPORT, TEENSY_PIN0, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN1_IOPORT, TEENSY_PIN1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN2_IOPORT, TEENSY_PIN2, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN3_IOPORT, TEENSY_PIN3, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN4_IOPORT, TEENSY_PIN4, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN5_IOPORT, TEENSY_PIN5, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN6_IOPORT, TEENSY_PIN6, PAL_MODE_OUTPUT_PUSHPULL);

    // Rows (sense)
    palSetPadMode(TEENSY_PIN14_IOPORT, TEENSY_PIN14, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN15_IOPORT, TEENSY_PIN15, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN16_IOPORT, TEENSY_PIN16, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN17_IOPORT, TEENSY_PIN17, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN20_IOPORT, TEENSY_PIN20, PAL_MODE_INPUT_PULLDOWN);

    // Start I2C
    palSetPadMode(TEENSY_PIN18_IOPORT, TEENSY_PIN18, PAL_MODE_OUTPUT_OPENDRAIN); // SDA
    palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_OPENDRAIN); // SCL 
    i2cStart(&I2CD1, &i2c_config);
    // IT DOESN'T WORK ! CAN'T FIND A WAY TO USE I2C WITH TEENSY 3.2 :(
    //uint8_t tx[2] = {MCP_IODIRA, 0b01111111};
	//i2cMasterTransmit(&I2CD1, MCP_I2C_ADDR, tx, 2, 0, 0);

    // Initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
    }

    for (uint8_t i=0; i < MATRIX_COLS; ++i)
    {
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
    xprintf("matrix_scan %d\n", timer_read());

    const uint8_t MATRIX_COLS_HALF = MATRIX_COLS/2;
    for (int col = 0; col < MATRIX_COLS_HALF; ++col)
    {
        uint8_t data = 0;

        // Strobe col
        switch (col)
        {
        case 0: palSetPad(TEENSY_PIN0_IOPORT, TEENSY_PIN0); break;
        case 1: palSetPad(TEENSY_PIN1_IOPORT, TEENSY_PIN1); break;
        case 2: palSetPad(TEENSY_PIN2_IOPORT, TEENSY_PIN2); break;
        case 3: palSetPad(TEENSY_PIN3_IOPORT, TEENSY_PIN3); break;
        case 4: palSetPad(TEENSY_PIN4_IOPORT, TEENSY_PIN4); break;
        case 5: palSetPad(TEENSY_PIN5_IOPORT, TEENSY_PIN5); break;
        case 6: palSetPad(TEENSY_PIN6_IOPORT, TEENSY_PIN6); break;
        }
        
        // Need wait to settle pin state
        wait_us(20);

        // Read row data
        data = (
                (palReadPad(TEENSY_PIN14_IOPORT, TEENSY_PIN14) << 0) |
                (palReadPad(TEENSY_PIN15_IOPORT, TEENSY_PIN15) << 1) |
                (palReadPad(TEENSY_PIN16_IOPORT, TEENSY_PIN16) << 2) |
                (palReadPad(TEENSY_PIN17_IOPORT, TEENSY_PIN17) << 3) |
                (palReadPad(TEENSY_PIN20_IOPORT, TEENSY_PIN20) << 4)
               );

        // Unstrobe col
        switch (col)
        {
        case 0: palClearPad(TEENSY_PIN0_IOPORT, TEENSY_PIN0); break;
        case 1: palClearPad(TEENSY_PIN1_IOPORT, TEENSY_PIN1); break;
        case 2: palClearPad(TEENSY_PIN2_IOPORT, TEENSY_PIN2); break;
        case 3: palClearPad(TEENSY_PIN3_IOPORT, TEENSY_PIN3); break;
        case 4: palClearPad(TEENSY_PIN4_IOPORT, TEENSY_PIN4); break;
        case 5: palClearPad(TEENSY_PIN5_IOPORT, TEENSY_PIN5); break;
        case 6: palClearPad(TEENSY_PIN6_IOPORT, TEENSY_PIN6); break;
        }

        if (matrix_debouncing[col] != data)
        {
            matrix_debouncing[col] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)
    {
        for (int row = 0; row < MATRIX_ROWS; row++)
        {
            matrix[row] = 0;
            for (int col = 0; col < MATRIX_COLS; col++)
            {
                matrix[row] |= ((matrix_debouncing[col] & (1 << row) ? 1 : 0) << col);
            }
        }
        debouncing = false;
    }

    matrix_scan_quantum();
    return 1;
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
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