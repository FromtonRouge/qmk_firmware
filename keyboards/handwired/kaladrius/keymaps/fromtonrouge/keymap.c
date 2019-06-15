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

#define L_STANDARD 0
#define L_STENO 1
#define L_SHIFT_COLEMAK 2
#define L_ACCENTS 3
#define L_FN 4

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes
{
    CKC_SFT = CKC_KALADRIUS_SAFE_RANGE, // Custom shift
    CKC_SFUN,   // Shift function
    CKC_CIRC,
};

// Steno keymap
const uint32_t PROGMEM g_steno_layout[MATRIX_ROWS][MATRIX_COLS] = LAYOUT(
        0,  0,   0,   0,   0,   0,  0,             0,      0,   0,   0,   0,   0,    0, 
        0,  0,  L3,  L2,  L1,  L0,  S_ENT,         SC_SEP, R0,  R1,  R2,  R3,  R4,   0,
        0,  L4, L_A, L_C, L_W, L_N, SC_STAR,       SC_SEP, R_R, R_L, R_C, R_T, RP_E, 0,
        0,  L5, L_S, L_T, L_H, L_R,                        R_N, R_G, R_H, R_S, RP_Y, 0,
        0,  0,   0,                                                       0,   0,    0,
                                   SC_STAR,        SC_SEP, 
                        SC_LPLUS, T_E, T_O,        T_Y, T_I, SC_RPLUS, 
                                       T_A,        T_U );

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [L_STENO] = LAYOUT(
        KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_LGUI,      KC_TRNS,   KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,    KC_F12, 
        KC_NO,     CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,    CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,    CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,                          CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        KC_LSFT,   KC_LCTL,   KC_LALT,                                                                                              KC_RALT,   KC_RCTL,   KC_RSFT, 
                                                               CKC_STENO,                          CKC_STENO, 
                                         CKC_STENO, CKC_STENO, CKC_STENO,                          CKC_STENO, CKC_STENO, CKC_STENO, 
                                                               CKC_STENO,                          CKC_STENO ),

    [L_STANDARD] = LAYOUT(
        _AMP,    _LBRC,   _LCBR, _RCBR, _LPRN, _EQL, KC_LGUI,                TG(L_STENO), _ASTR, _RPRN, _PLUS,    _RBRC, _EXLM, _HASH, 
        _DLR,    _Q,      _W,    _F,    _P,    _G,   TG(L_FN),               TG(L_FN),    _J,    _L,    _U,       _Y,    _SCLN, _SLSH, 
        KC_TAB,  _A,      _R,    _S,    _T,    _D,   KC_ESC,                 KC_PSCR,       _H,    _N,    _E,       _I,    _O,    _MINS, 
        CKC_SFT, _Z,      _X,    _C,    _V,    _B,                                        _K,    _M,    _COMM,    _DOT,  _APOS, CKC_SFT, 
        KC_LCTL, _AMP, KC_LALT,                                                                                _BSLS, _AT, KC_RCTL,
                                                     TG(L_FN),               TG(L_FN), 
                                   KC_ENT, KC_BSPC,  MO(L_ACCENTS),          KC_DEL, MO(L_FN), KC_SPC,  
                                                     MO(L_ACCENTS),          KC_NO),
    [L_SHIFT_COLEMAK] = LAYOUT(
        FR_UGRV, KC_7,    KC_5,    KC_3,    KC_1,    KC_9,    KC_TRNS,     KC_NO,   KC_0,    KC_2,    KC_4,     KC_6,     KC_8,     CKC_SFUN, 
        CKC_SFUN,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,  CKC_SFUN, FR_COMM, 
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS,  CKC_SFUN, 
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS, KC_TRNS, CKC_SFUN, CKC_SFUN, CKC_SFUN, KC_TRNS, 
        KC_TRNS,  FR_UGRV, KC_TRNS,                                                                              CKC_SFUN,  CKC_SFUN,  KC_TRNS, 
                                                      KC_TRNS,                       KC_TRNS, 
                                    KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS, KC_TRNS, KC_TRNS, 
                                                      KC_TRNS,                       KC_TRNS ),
    [L_ACCENTS] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, KC_NO,    KC_NO,    KC_NO,         KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO, FR_UGRV,  KC_NO,    LSFT(KC_LBRC), KC_NO, 
        KC_NO, KC_0,  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_7,  KC_2,  CKC_CIRC, CKC_CIRC, CKC_CIRC,      KC_NO, 
        KC_NO, KC_NO, KC_NO, KC_9,  KC_NO, KC_NO,                      KC_NO, KC_NO, KC_NO,    KC_NO,    KC_NO,         KC_NO, 
        KC_NO, KC_NO, KC_NO,                                                                   KC_NO,    KC_NO,         KC_NO, 
                                           KC_NO,                      KC_NO, 
                             KC_NO, KC_TRNS, KC_TRNS,                  KC_TRNS, KC_TRNS, KC_NO, 
                                           KC_TRNS,                    KC_TRNS ),
    [L_FN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_TRNS,        KC_NO,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS, KC_TRNS, KC_ESC, KC_TRNS, KC_TRNS,  KC_TRNS, 
        KC_LSFT, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  KC_TRNS, RESET,          KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_RSFT, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS,                                                                                KC_TRNS, KC_TRNS,  KC_TRNS, 
                                                      KC_TRNS,                         KC_TRNS, 
                                    KC_TRNS, KC_TRNS, KC_TRNS,                         KC_TRNS, KC_TRNS, KC_TRNS, 
                                                      KC_TRNS,                         KC_TRNS ),
};

steno_layout_t* get_steno_layout(void) { return g_steno_layout; }

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
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
            const uint16_t keycode = keymap_key_to_keycode(L_STANDARD , record->event.key);
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
                const uint16_t keycode = keymap_key_to_keycode(L_STANDARD , record->event.key);
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

void matrix_scan_user(void)
{
    uint8_t layer = biton32(layer_state);

    switch (layer)
    {
    case L_STENO:
        set_leds(true, false, false);
        break;
    case L_FN:
        set_leds(false, true, false);
        break;
    case L_ACCENTS:
        set_leds(false, false, true);
        break;
    case L_SHIFT_COLEMAK:
        set_leds(false, true, true);
        break;
    default:
        set_leds(false, false, false);
        break;
    }

    if (!can_stroke())
    {
        set_leds(true, false, true);
    }
}
