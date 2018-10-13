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

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static uint8_t mcp23018_reset_loop;

__attribute__ ((weak)) void matrix_init_user(void) {}
__attribute__ ((weak)) void matrix_scan_user(void) {}
__attribute__ ((weak)) void matrix_init_kb(void)
{
    matrix_init_user();
}

__attribute__ ((weak)) void matrix_scan_kb(void)
{
    matrix_scan_user();
}

uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    DDRD |= (1<<6);
    matrix_init_quantum();
}

void matrix_power_up(void)
{
}

uint8_t matrix_scan(void)
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

    uint8_t data = 0;
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPIOA);             if (mcp23018_status) goto out;
    mcp23018_status = i2c_start(I2C_ADDR_READ);     if (mcp23018_status) goto out;
    data = ~i2c_readNak();
out:
    i2c_stop();

    xprintf("GPIOA = 0b%08b\n", data);

    if (data & 1)
    {
        PORTD |= (1<<6);
    }
    else
    {
        PORTD &= ~(1<<6);
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
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++)
    {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++)
    {
        count += bitpop16(matrix[i]);
    }
    return count;
}
