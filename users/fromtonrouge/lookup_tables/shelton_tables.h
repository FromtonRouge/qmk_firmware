#ifndef SHELTON_TABLES_H
#define SHELTON_TABLES_H

#include "table.h"
#include "keycodes.h"
#include "tmk_core/common/progmem.h"
#include <stdint.h>

extern const uint8_t PROGMEM g_left_hand_table[256][MAX_LETTERS];
extern const uint8_t PROGMEM g_thumbs_table[64][MAX_VOWELS];
extern const uint8_t PROGMEM g_thumbs_bigrams_table[64][MAX_VOWELS];
extern const uint8_t PROGMEM g_right_hand_table[256][MAX_LETTERS];
extern const uint8_t PROGMEM g_right_pinky_table[4][2];

#endif
