#ifndef COLEMAK_H
#define COLEMAK_H

#include "quantum/keymap_extras/keymap_french.h"
#include "tmk_core/common/keycode.h"
#include <stdint.h>

// For the Programmer Colemak layout
#ifdef AZERTY_OS_ENABLE
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
#else // QWERTY
#define SPECIAL_SHIFT_TABLE_SIZE 20

// This table was computed externally
const uint16_t g_special_shift_table[SPECIAL_SHIFT_TABLE_SIZE] =
{
    KC_3,       // [0] KC_RCBR
    0,          // [1]
    KC_8,       // [2] KC_EXLM
    LSFT(KC_6), // [3] KC_AT
    0,          // [4]
    0,          // [5]
    KC_9,       // [6] KC_EQL
    KC_7,       // [7] KC_LBRC
    KC_6,       // [8] KC_RBRC
    KC_0,       // [9] KC_ASTR
    KC_1,       // [10] KC_LPRN
    KC_2,       // [11] KC_RPRN
    0,          // [12]
    0,          // [13]
    0,          // [14]
    0,          // [15]
    0,          // [16]
    0,          // [17]
    KC_4,       // [18] KC_PLUS
    KC_5        // [19] KC_LCBR
};
#endif

#endif
