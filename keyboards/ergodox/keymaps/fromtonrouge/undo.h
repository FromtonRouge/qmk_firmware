#ifndef UNDO_H
#define UNDO_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_UNDO 75 // Can't use 100 anymore, not enough program memory

typedef enum
{
    NO_CHANGE,
    CHARACTER,
    MOVE_LEFT,
    MOVE_UP,
    ENTER
} change_kind_t;

typedef struct
{
    change_kind_t kind  :4;
    uint8_t count       :6;
} change_t;

#define MAX_CHANGES 6

// TODO: reduce the size of undo_command_t
typedef struct 
{
    uint8_t change_index            :3;
    uint16_t previous_case_mode;
    uint16_t next_case_mode;
    change_t changes[MAX_CHANGES];
} undo_command_t;

bool can_undo(undo_command_t* undo_command);
void undo_command_add_change(undo_command_t* undo_command, change_kind_t kind);
change_kind_t undo_command_add_change_from_code(undo_command_t* undo_command, uint8_t code);
uint8_t undo_command_get_changes_count(undo_command_t* undo_command, change_kind_t kind);

extern undo_command_t g_undo_stack[MAX_UNDO];
extern int8_t g_undo_stack_index;
extern undo_command_t g_new_undo_command; // Must be global for ErgoDox Infinity to avoid a lock between the two halves

#endif
