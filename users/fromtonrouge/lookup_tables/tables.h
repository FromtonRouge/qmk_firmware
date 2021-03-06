#ifndef TABLES_H
#define TABLES_H

#include "table.h"
#include <stdint.h>
#include <stdbool.h>

// Keys family
enum key_family
{
    FAMILY_SPECIAL_CONTROLS,
    FAMILY_LEFT_CONTROLS,
    FAMILY_LEFT_HAND,
    FAMILY_THUMBS,
    FAMILY_RIGHT_HAND,
    FAMILY_RIGHT_PINKY,
    FAMILY_RIGHT_CONTROLS,
    FAMILY_SPACES,
    NB_FAMILY
};

extern const uint8_t g_family_to_bit_offset[NB_FAMILY];
extern table_t g_tables[NB_FAMILY];

#endif
