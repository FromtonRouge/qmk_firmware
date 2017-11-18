#ifndef USER_TABLES_H
#define USER_TABLES_H

#include "table.h"
#include "keycodes.h"
#include "tmk_core/common/progmem.h"
#include <stdint.h>

extern const uint16_t PROGMEM g_left_controls_table[64][MAX_ONE_KEYCODE];
extern const uint16_t PROGMEM g_right_controls_table[32][MAX_SYMBOLS];
extern const uint16_t PROGMEM g_left_punctuation_table[256][MAX_PUNCTUATION];
extern const uint16_t PROGMEM g_right_punctuation_table[256][MAX_PUNCTUATION];
extern const uint16_t PROGMEM g_spaces_ctl_table[2][MAX_SYMBOLS];

#endif
