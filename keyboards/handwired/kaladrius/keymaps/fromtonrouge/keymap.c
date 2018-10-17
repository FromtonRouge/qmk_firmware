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
#include QMK_KEYBOARD_H
#include "jackdaw/keycodes.h"
#include "jackdaw/stroke.h"
#include "quantum/keymap_extras/keymap_french.h"
#include "tmk_core/common/keycode.h"
#include <stdint.h>

// For the Programmer Colemak layout
#define SPECIAL_SHIFT_TABLE_SIZE 18

// This table was computed externally
const uint16_t g_special_shift_table[SPECIAL_SHIFT_TABLE_SIZE] =
{
    FR_COLN,    // [0] FR_SCLN
    0,          // [1]
    0,          // [2]
    0,          // [3]
    FR_GRV,     // [4] FR_HASH
    0,          // [5]
    0,          // [6]
    0,          // [7]
    FR_GRTR,    // [8] FR_DOT
    FR_PIPE,    // [9] FR_BSLS
    0,          // [10]
    FR_CIRC,    // [11] FR_AT
    FR_TILD,    // [12] FR_DLR
    0,          // [13]
    0,          // [14]
    FR_QUOT,    // [15] FR_APOS
    FR_LESS,    // [16] FR_COMM
    FR_UNDS     // [17] FR_MINS
};

#define L_BASE 0
#define L_STENO 1
#define L_SHIFT_COLEMAK 2
#define L_ACCENTS 3
#define L_FN 4

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes
{
    QMKBEST = CKC_KALADRIUS_SAFE_RANGE,
    QMKURL,
    CKC_SFT,    // Custom shift
    CKC_SFUN,   // Shift function
    CKC_CIRC,
};

// Steno keymap
const uint32_t PROGMEM g_steno_keymap[MATRIX_ROWS][MATRIX_COLS] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                      KC_NO, KC_NO,                                                 KC_NO, KC_NO, 
                                                KC_NO,              KC_NO, 
                                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, 
                                                KC_NO,              KC_NO );

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [L_BASE] = LAYOUT(
        _DLR,       _LBRC,  _LCBR,      _RCBR,      _LPRN,  _EQL,   KC_LGUI,                TG(L_STENO),    _ASTR,  _RPRN,  _PLUS,      _RBRC,  _EXLM,  _SLSH, 
        KC_TAB,     _Q,     _W,         _F,         _P,     _G,     KC_INS,                 KC_NO,          _J,     _L,     _U,         _Y,     _SCLN,  _MINS, 
        CKC_SFT,    _A,     _R,         _S,         _T,     _D,     KC_ESC,                 _HASH,          _H,     _N,     _E,         _I,     _O,     CKC_SFT, 
        KC_LCTL,    _Z,     _X,         _C,         _V,     _B,     _AMP,                   _BSLS,          _K,     _M,     _COMM,      _DOT,   _APOS,  KC_RCTL, 
                            KC_LALT,    MO(L_FN),                                                                           MO(L_FN),   _AT, 
                                                                    TG(L_FN),               TG(L_FN), 
                                                    KC_ENT, KC_BSPC,MO(L_ACCENTS),          MO(L_ACCENTS),  KC_DEL, KC_SPC, 
                                                                    KC_BSPC,                KC_DEL ),
    [L_STENO] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                      KC_NO, KC_NO,                                                 KC_NO, KC_NO, 
                                                KC_NO,              KC_NO, 
                                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, 
                                                KC_NO,              KC_NO ),
    [L_SHIFT_COLEMAK] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                      KC_NO, KC_NO,                                                 KC_NO, KC_NO, 
                                                KC_NO,              KC_NO, 
                                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, 
                                                KC_NO,              KC_NO ),
    [L_ACCENTS] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                      KC_NO, KC_NO,                                                 KC_NO, KC_NO, 
                                                KC_NO,              KC_NO, 
                                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, 
                                                KC_NO,              KC_NO ),
    [L_FN] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                      KC_NO, KC_NO,                                                 KC_NO, KC_NO, 
                                                KC_NO,              KC_NO, 
                                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, 
                                                KC_NO,              KC_NO ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
    case QMKBEST:
        {
            if (record->event.pressed)
            {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            }
            else
            {
                // when keycode QMKBEST is released
            }
            break;
        }
    case QMKURL:
        {
            if (record->event.pressed)
            {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
            }
            else
            {
                // when keycode QMKURL is released
            }
            break;
        }
    case CKC_STENO:
        {
            const uint32_t dword = pgm_read_dword(&(g_steno_keymap[record->event.key.row][record->event.key.col]));
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
            return false;
        }
    case CKC_SFT: // Apply SHIFT and go to L_SHIFT_COLEMAK
        {
            const uint8_t shift_key = record->event.key.row == 0 ? KC_LSFT : KC_RSFT;
            if (record->event.pressed)
            {
                register_code(shift_key);
                layer_on(L_SHIFT_COLEMAK);
            }
            else
            {
                unregister_code(shift_key);
                layer_off(L_SHIFT_COLEMAK);
            }
            return false;
        }
    case CKC_SFUN: // Handle special shift codes for the Programmer Colemak layout
        {
            const uint16_t keycode = keymap_key_to_keycode(L_BASE , record->event.key);
            const uint16_t special_shift_code = g_special_shift_table[keycode % SPECIAL_SHIFT_TABLE_SIZE];
            const uint8_t code = (uint8_t)special_shift_code;
            if (record->event.pressed)
            {
                send_mods_and_code(special_shift_code >> 8, code);
            }
            else
            {
                unregister_code(code);
            }
            return false;
        }
    case CKC_CIRC:
        {
            if (record->event.pressed)
            {
                const uint16_t keycode = keymap_key_to_keycode(L_BASE , record->event.key);
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                register_code(keycode);
                unregister_code(keycode);
            }
            return false;
        }
    default:
        {
            break;
        }
    }
    return true;
}

void matrix_init_user(void)
{
}

void matrix_scan_user(void)
{
}

void led_set_user(uint8_t usb_led)
{
}
