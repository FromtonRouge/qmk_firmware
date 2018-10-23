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

// Set 0 if debouncing isn't needed
#ifndef DEBOUNCING_DELAY
#define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
//static uint16_t debouncing_time;
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

#define MCP_I2C_ADDR        0x20 // 0x20 Because the ADDR pin is connected to the ground (@see mcp23018 datasheet)
#define MCP_I2C_ADDR_WRITE  ( (I2C_ADDR<<1) | I2C_WRITE )
#define MCP_I2C_ADDR_READ   ( (I2C_ADDR<<1) | I2C_READ  )
#define MCP_IODIRA          0x00            // i/o direction register
#define MCP_IODIRB          0x01
#define MCP_GPPUA           0x0C            // GPIO pull-up resistor register
#define MCP_GPPUB           0x0D
#define MCP_GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define MCP_GPIOB           0x13
#define MCP_OLATA           0x14            // output latch register
#define MCP_OLATB           0x15

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
uint8_t mcp23018_status = 1;

__attribute__ ((weak)) void matrix_init_user(void) {}
__attribute__ ((weak)) void matrix_scan_user(void) {}
__attribute__ ((weak)) void matrix_init_kb(void) { matrix_init_user(); }
__attribute__ ((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }
uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }

void matrix_init(void)
{
    // Same as quantum/matrix.c
    // Initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
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

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; ++i)
    {
        count += matrix_bitpop(i);
    }
    return count;
}
