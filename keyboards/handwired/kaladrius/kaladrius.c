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
#include "jackdaw/keycodes.h"
#include "jackdaw/stroke.h"

void matrix_init_kb(void)
{
    matrix_init_user();
}

void matrix_scan_kb(void)
{
    matrix_scan_user();
}

extern steno_layout_t* get_steno_layout(void);
bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
    case CKC_STENO:
        {
			steno_layout_t* steno_keymap_p = get_steno_layout();
			if (steno_keymap_p)
			{
				const uint32_t dword = pgm_read_dword(&(steno_keymap_p[record->event.key.row][record->event.key.col]));
				if (dword & STENO_BIT)
				{
					const uint32_t bit_key = 1L << (dword & 0x0F);
					const uint8_t family = (dword >> 4) & 0x0F;
					const uint8_t family_offset = g_family_to_bit_offset[family];
					uint32_t* keys_pressed_p = g_family_to_keys_pressed[family];
					if (record->event.pressed)
					{
						(*keys_pressed_p) |= (bit_key << family_offset);
						g_family_bits[family] |= bit_key;
					}
					else
					{
						(*keys_pressed_p) &= ~(bit_key << family_offset);

						// Stroke if all steno keys are released
						if (can_stroke())
						{
							stroke();
						}
					}
				}
			}
            return false;
        }
    default:
        {
            break;
        }
    }
    return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led)
{
    led_set_user(usb_led);
}
