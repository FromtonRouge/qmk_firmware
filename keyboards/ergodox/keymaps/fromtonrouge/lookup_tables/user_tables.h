#ifndef USER_TABLES_H
#define USER_TABLES_H

#include <stdint.h>
#include "table.h"
#include "keycodes.h"
#include "tmk_core/common/progmem.h"

extern const uint16_t PROGMEM g_left_controls_table[64][MAX_ONE_KEYCODE];
extern const uint16_t PROGMEM g_right_controls_table[32][MAX_SYMBOLS];
extern const uint16_t PROGMEM g_left_punctuations_table[256][MAX_PUNCTUATIONS];
extern const uint16_t PROGMEM g_right_punctuations_table[256][MAX_PUNCTUATIONS];
extern const uint16_t PROGMEM g_spaces_ctl_table[4][MAX_SYMBOLS];

#endif
