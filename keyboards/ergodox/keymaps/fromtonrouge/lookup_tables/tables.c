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
    OFFSET_LEFT_HAND,
    OFFSET_THUMBS,
    OFFSET_RIGHT_HAND,
    OFFSET_RIGHT_PINKY,
    OFFSET_RIGHT_CONTROLS,
    OFFSET_SPACE_CONTROLS
};

table_t g_tables[NB_FAMILY] =
{
    {
        0,
        0,
        KIND_UNKNOWN,
    },
    {
        (void*)g_left_controls_table,
        MAX_ONE_KEYCODE,
        KIND_ONE_KEYCODE,
    },
    {
        (void*)g_left_hand_table,
        MAX_LETTERS,
        KIND_LETTERS,
    },
    {
        (void*)g_thumbs_table,
        MAX_VOWELS,
        KIND_LETTERS,
    },
    {
        (void*)g_right_hand_table,
        MAX_LETTERS,
        KIND_LETTERS,
    },
    {
        (void*)g_right_pinky_table,
        MAX_LETTERS,
        KIND_LETTERS,
    },
    {
        (void*)g_right_controls_table,
        MAX_SYMBOLS,
        KIND_SYMBOLS,
    },
    {
        (void*)g_spaces_ctl_table,
        MAX_SYMBOLS,
        KIND_SYMBOLS,
    },
};
