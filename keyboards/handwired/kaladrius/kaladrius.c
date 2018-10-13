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
#include "kaladrius.h"

uint8_t mcp23018_status = 0x20;
bool i2c_initialized = false;
uint8_t init_mcp23018(void)
{
    mcp23018_status = 0x20;

    if (!i2c_initialized)
    {
        print("test0");
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

void matrix_init_kb(void)
{
	matrix_init_user();
}

void matrix_scan_kb(void)
{
	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
	return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led)
{
	led_set_user(usb_led);
}
