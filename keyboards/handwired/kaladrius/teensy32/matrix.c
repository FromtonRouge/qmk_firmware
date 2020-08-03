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
#include "kaladrius.h"

#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

// Set 0 if debouncing isn't needed
#ifndef DEBOUNCING_DELAY
#define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
static uint16_t debouncing_time;
static bool debouncing = false;
#endif

#define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
#define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))

static matrix_row_t matrix[MATRIX_ROWS];
static uint8_t matrix_debouncing[MATRIX_COLS];

#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif

__attribute__ ((weak)) void matrix_init_user(void) {}
__attribute__ ((weak)) void matrix_scan_user(void) {}
__attribute__ ((weak)) void matrix_init_kb(void) { matrix_init_user(); }
__attribute__ ((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }
uint8_t matrix_rows(void) { return MATRIX_ROWS; }
uint8_t matrix_cols(void) { return MATRIX_COLS; }

void set_leds(bool red, bool green, bool blue)
{
    if (red)
    {
        palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);
    }
    else
    {
        palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);
    }

    if (green)
    {
        palSetPad(TEENSY_PIN14_IOPORT, TEENSY_PIN14);
    }
    else
    {
        palClearPad(TEENSY_PIN14_IOPORT, TEENSY_PIN14);
    }

    if (blue)
    {
        palSetPad(TEENSY_PIN15_IOPORT, TEENSY_PIN15);
    }
    else
    {
        palClearPad(TEENSY_PIN15_IOPORT, TEENSY_PIN15);
    }
}

void blink_leds(void)
{
    const uint8_t delay = 200;
    set_leds(true, false, false);
    chThdSleepMilliseconds(delay);
    set_leds(false, true, false);
    chThdSleepMilliseconds(delay);
    set_leds(false, false, true);
    chThdSleepMilliseconds(delay);
    set_leds(true, true, false);
    chThdSleepMilliseconds(delay);
    set_leds(false, true, true);
    chThdSleepMilliseconds(delay);
    set_leds(true, false, true);
    chThdSleepMilliseconds(delay);
    set_leds(false, false, false);
}

void matrix_init(void)
{
    blink_leds();

    // Left Columns (strobe)
    palSetPadMode(TEENSY_PIN0_IOPORT, TEENSY_PIN0, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN1_IOPORT, TEENSY_PIN1, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN2_IOPORT, TEENSY_PIN2, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN3_IOPORT, TEENSY_PIN3, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN4_IOPORT, TEENSY_PIN4, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN5_IOPORT, TEENSY_PIN5, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN6_IOPORT, TEENSY_PIN6, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN7_IOPORT, TEENSY_PIN7, PAL_MODE_OUTPUT_PUSHPULL);

    // Right Columns (strobe)
    palSetPadMode(TEENSY_PIN16_IOPORT, TEENSY_PIN16, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN17_IOPORT, TEENSY_PIN17, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN18_IOPORT, TEENSY_PIN18, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN20_IOPORT, TEENSY_PIN20, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN21_IOPORT, TEENSY_PIN21, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN22_IOPORT, TEENSY_PIN22, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN23_IOPORT, TEENSY_PIN23, PAL_MODE_OUTPUT_PUSHPULL);

    // RGB leds : Red = 13 Green = 14 Blue = 15
    palSetPadMode(TEENSY_PIN13_IOPORT, TEENSY_PIN13, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN14_IOPORT, TEENSY_PIN14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(TEENSY_PIN15_IOPORT, TEENSY_PIN15, PAL_MODE_OUTPUT_PUSHPULL);

    // Rows (sense)
    palSetPadMode(TEENSY_PIN8_IOPORT, TEENSY_PIN8, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN9_IOPORT, TEENSY_PIN9, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN10_IOPORT, TEENSY_PIN10, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN11_IOPORT, TEENSY_PIN11, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(TEENSY_PIN12_IOPORT, TEENSY_PIN12, PAL_MODE_INPUT_PULLDOWN);

    // Initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; ++i)
    {
        matrix[i] = 0;
    }

    for (uint8_t i=0; i < MATRIX_COLS; ++i)
    {
        matrix_debouncing[i] = 0;
    }

#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_timer = timer_read32();
    matrix_scan_count = 0;
#endif

    matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_scan_count++;

    uint32_t timer_now = timer_read32();
    if (TIMER_DIFF_32(timer_now, matrix_timer) > 1000)
    {
        print("matrix scan frequency: ");
        pdec(matrix_scan_count);
        print("\n");

        matrix_timer = timer_now;
        matrix_scan_count = 0;
    }
#endif

    for (uint8_t col = 0; col < MATRIX_COLS; ++col)
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
        case 7: palSetPad(TEENSY_PIN7_IOPORT, TEENSY_PIN7); break;
        case 8: palSetPad(TEENSY_PIN16_IOPORT, TEENSY_PIN16); break;
        case 9: palSetPad(TEENSY_PIN17_IOPORT, TEENSY_PIN17); break;
        case 10: palSetPad(TEENSY_PIN18_IOPORT, TEENSY_PIN18); break;
        case 11: palSetPad(TEENSY_PIN19_IOPORT, TEENSY_PIN19); break;
        case 12: palSetPad(TEENSY_PIN20_IOPORT, TEENSY_PIN20); break;
        case 13: palSetPad(TEENSY_PIN21_IOPORT, TEENSY_PIN21); break;
        case 14: palSetPad(TEENSY_PIN22_IOPORT, TEENSY_PIN22); break;
        case 15: palSetPad(TEENSY_PIN23_IOPORT, TEENSY_PIN23); break;
        }
        
        // Need wait to settle pin state
        // IMPORTANT NOTE: To use wait_us() be sure to define properly CH_CFG_ST_FREQUENCY with a high value in chconf.h
        wait_us(10);

        // Read row data
        data = (
                (palReadPad(TEENSY_PIN8_IOPORT, TEENSY_PIN8) << 0) |
                (palReadPad(TEENSY_PIN9_IOPORT, TEENSY_PIN9) << 1) |
                (palReadPad(TEENSY_PIN10_IOPORT, TEENSY_PIN10) << 2) |
                (palReadPad(TEENSY_PIN11_IOPORT, TEENSY_PIN11) << 3) |
                (palReadPad(TEENSY_PIN12_IOPORT, TEENSY_PIN12) << 4)
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
        case 7: palClearPad(TEENSY_PIN7_IOPORT, TEENSY_PIN7); break;
        case 8: palClearPad(TEENSY_PIN16_IOPORT, TEENSY_PIN16); break;
        case 9: palClearPad(TEENSY_PIN17_IOPORT, TEENSY_PIN17); break;
        case 10: palClearPad(TEENSY_PIN18_IOPORT, TEENSY_PIN18); break;
        case 11: palClearPad(TEENSY_PIN19_IOPORT, TEENSY_PIN19); break;
        case 12: palClearPad(TEENSY_PIN20_IOPORT, TEENSY_PIN20); break;
        case 13: palClearPad(TEENSY_PIN21_IOPORT, TEENSY_PIN21); break;
        case 14: palClearPad(TEENSY_PIN22_IOPORT, TEENSY_PIN22); break;
        case 15: palClearPad(TEENSY_PIN23_IOPORT, TEENSY_PIN23); break;
        }

#if (DEBOUNCING_DELAY > 0)
        if (matrix_debouncing[col] != data)
        {
            matrix_debouncing[col] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
#endif
    }

#if (DEBOUNCING_DELAY > 0)
    if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY))
    {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++)
        {
            matrix[row] = 0;
            for (uint8_t col = 0; col < MATRIX_COLS; col++)
            {
                matrix[row] |= ((matrix_debouncing[col] & (1 << row) ? 1 : 0) << col);
            }
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
