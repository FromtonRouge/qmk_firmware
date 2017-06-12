#ifndef STROKE_H
#define STROKE_H

#include "lookup_tables/tables.h"
#include "lookup_tables/table.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    kind_table_t kind;
    uint16_t keycode;
} stroke_element_t;

#ifdef SUBPROJECT_infinity
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
#endif

bool is_letter(uint8_t code);
void send_mods_and_code(uint8_t mods, uint8_t code);
bool can_stroke(void);
void stroke_init_buffer(void);
void stroke_add_element(kind_table_t kind, uint16_t keycode);
void stroke(void);

#define MAX_STROKE_BUFFER 64

extern stroke_element_t g_stroke_buffer[MAX_STROKE_BUFFER];
extern uint8_t g_stroke_buffer_count;
extern uint32_t g_bits_keys_pressed_part1;
extern uint32_t g_bits_keys_pressed_part2;
extern uint16_t g_separator_mode;
extern uint16_t g_case_mode;
extern uint8_t g_family_bits[NB_FAMILY];
extern uint32_t* g_family_to_keys_pressed[NB_FAMILY];

#endif
