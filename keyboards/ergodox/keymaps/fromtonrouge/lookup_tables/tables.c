#include "tables.h"
#include "table.h"
#include "keycodes.h"
#include "shelton_tables.h"
#include "user_tables.h"

// Table to convert family id to bit offset
const uint8_t g_family_to_bit_offset[NB_FAMILY] =
{
    OFFSET_SPECIAL_CONTROLS,
    OFFSET_LEFT_CONTROLS,
    OFFSET_LEFT_PINKY,
    OFFSET_LEFT_HAND,
    OFFSET_THUMBS,
    OFFSET_RIGHT_HAND,
    OFFSET_RIGHT_PINKY,
    OFFSET_RIGHT_CONTROLS,
    OFFSET_SPACE_CONTROLS
};

const uint8_t g_family_to_kind_table[NB_FAMILY] =
{
    KIND_UNKNOWN,
    KIND_ONE_KEYCODE,   // Left controls
    KIND_ONE_KEYCODE,   // Left pinky
    KIND_LETTERS,   // Left hand
    KIND_LETTERS,   // Thumbs
    KIND_LETTERS,   // Right hand
    KIND_LETTERS,   // Right pinky
    KIND_SYMBOLS,   // Right controls
    KIND_SYMBOLS
};

void* g_all_tables[NB_FAMILY] = 
{
    0,
    (void*)g_left_controls_table,
    (void*)g_left_pinky_table,
    (void*)g_left_hand_table,
    (void*)g_thumbs_table,
    (void*)g_right_hand_table,
    (void*)g_right_pinky_table,
    (void*)g_right_controls_table,
    (void*)g_spaces_ctl_table
};
