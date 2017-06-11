#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "keymap_extras/keymap_french.h"
#include "lookup_tables/shelton_tables.h"
#include "lookup_tables/user_tables.h"

// Layer indexes
#define LAYER_BASE 0
#define LAYER_STENO 1

#ifdef PROGRAMMER_COLEMAK_ENABLE
    #define LAYER_SHIFT_COLEMAK 2
    #ifdef AZERTY_OS_ENABLE
        #define LAYER_ACCENTS 3
        #define LAYER_FN 4
    #else
        #define LAYER_FN 3
    #endif
#else
    #define LAYER_FN 2
#endif

#ifdef SUBPROJECT_infinity
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
#endif

// Keys family
enum key_family
{
    FAMILY_SPECIAL_CONTROLS,
    FAMILY_LEFT_CONTROLS,
    FAMILY_LEFT_PINKY,
    FAMILY_LEFT_HAND,
    FAMILY_THUMBS,
    FAMILY_RIGHT_HAND,
    FAMILY_RIGHT_PINKY,
    FAMILY_RIGHT_CONTROLS,
    FAMILY_SPACES,
    NB_FAMILY
};

// Bit to identify a steno key
#define STENO_BIT (1L << 31) 

// 3 bits for star and the plus key
#define OFFSET_SPECIAL_CONTROLS 0
#define SC_STAR (0 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)
#define SC_PLUS (1 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)
#define SC_SEP  (2 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)

// 8 bits for the left hand
#define OFFSET_LEFT_HAND 3
#define L_N (0 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_R (1 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_W (2 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_H (3 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_C (4 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_T (5 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_A (6 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_S (7 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)

// 5 bits for thumbs
#define OFFSET_THUMBS 11
#define T_E (0 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_O (1 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_A (2 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_U (3 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_I (4 | (FAMILY_THUMBS << 4) | STENO_BIT)

// 8 bits for the right hand
#define OFFSET_RIGHT_HAND 16
#define R_R (0 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_N (1 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_L (2 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_G (3 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_C (4 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_H (5 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_T (6 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_S (7 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)

// 3 bits for E and Y and S
#define OFFSET_RIGHT_PINKY 24
#define RP_E  (0 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)
#define RP_Y  (1 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)
#define RP_S  (2 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)

// 2 bits for space control keys
#define OFFSET_SPACE_CONTROLS 27
#define S_TAB  (0 | (FAMILY_SPACES << 4) | STENO_BIT)
#define S_ENT  (1 | (FAMILY_SPACES << 4) | STENO_BIT)

// 6 bits for right controls
#define OFFSET_RIGHT_CONTROLS 0
#define R0  (0 | (FAMILY_RIGHT_CONTROLS << 4) | STENO_BIT)
#define R1  (1 | (FAMILY_RIGHT_CONTROLS << 4) | STENO_BIT)
#define R2  (2 | (FAMILY_RIGHT_CONTROLS << 4) | STENO_BIT)
#define R3  (3 | (FAMILY_RIGHT_CONTROLS << 4) | STENO_BIT)
#define R4  (4 | (FAMILY_RIGHT_CONTROLS << 4) | STENO_BIT)

// 3 bits for left pinky
#define OFFSET_LEFT_PINKY 5
#define LP_O  (0 | (FAMILY_LEFT_PINKY << 4) | STENO_BIT)
#define LP_U  (1 | (FAMILY_LEFT_PINKY << 4) | STENO_BIT)
#define LP_I  (2 | (FAMILY_LEFT_PINKY << 4) | STENO_BIT)

// 6 bits for left controls
#define OFFSET_LEFT_CONTROLS 8
#define L0  (0 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)
#define L1  (1 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)
#define L2  (2 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)
#define L3  (3 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)
#define L4  (4 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)
#define L5  (5 | (FAMILY_LEFT_CONTROLS << 4) | STENO_BIT)

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

// A lookup table can stores letters (8 bits) or symbols (16 bits)
typedef enum
{
    KIND_UNKNOWN,
    KIND_ONE_KEYCODE,
    KIND_LETTERS,
    KIND_SYMBOLS,
    KIND_PUNCTUATIONS
} kind_table_t;

typedef struct
{
    kind_table_t kind;
    uint16_t keycode;
} stroke_element_t;

#define MAX_STROKE_BUFFER 64
stroke_element_t g_stroke_buffer[MAX_STROKE_BUFFER] = {{0}};
uint8_t g_stroke_buffer_count = 0;

void stroke_init_buffer(void)
{
    g_stroke_buffer_count = 0;
    for (uint8_t i = 0; i < MAX_STROKE_BUFFER; ++i)
    {
        g_stroke_buffer[i].kind = KIND_UNKNOWN;
        g_stroke_buffer[i].keycode = 0;
    }
}

void stroke_add_element(kind_table_t kind, uint16_t keycode)
{
    g_stroke_buffer[g_stroke_buffer_count].kind = kind;
    g_stroke_buffer[g_stroke_buffer_count].keycode = keycode;
    g_stroke_buffer_count++;
}

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

// Global vars for the steno layer
uint32_t g_bits_keys_pressed_part1 = 0;
uint32_t g_bits_keys_pressed_part2 = 0;

uint32_t* g_family_to_keys_pressed[NB_FAMILY] = 
{
    &g_bits_keys_pressed_part1, // Special controls
    &g_bits_keys_pressed_part2, // Left controls
    &g_bits_keys_pressed_part2, // Left pinky
    &g_bits_keys_pressed_part1, // Left hand
    &g_bits_keys_pressed_part1, // Thumbs
    &g_bits_keys_pressed_part1, // Right hand
    &g_bits_keys_pressed_part1, // Right pinky
    &g_bits_keys_pressed_part2, // Right controls
    &g_bits_keys_pressed_part1
};

uint8_t g_family_bits[NB_FAMILY] = {0};
typedef const uint16_t one_keycode_table_t[MAX_ONE_KEYCODE];
typedef const uint8_t letters_table_t[MAX_LETTERS];
typedef const uint16_t symbols_table_t[MAX_SYMBOLS];
typedef const uint16_t punctuations_table_t[MAX_PUNCTUATIONS];
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

#define MAX_UNDO 100

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
typedef struct 
{
    uint8_t change_index            :3;
    change_t changes[MAX_CHANGES];
} undo_command_t;

void undo_command_add_change(undo_command_t* undo_command, change_kind_t kind)
{
    if (undo_command->change_index >= MAX_CHANGES || kind == NO_CHANGE)
    {
        return;
    }

    change_t* current_change = &undo_command->changes[undo_command->change_index];
    if (current_change->kind == NO_CHANGE || current_change->kind == kind)
    {
        current_change->kind = kind;
        current_change->count++;
    }
    else
    {
        undo_command->change_index++;
        undo_command_add_change(undo_command, kind);
    }
}

change_kind_t undo_command_add_change_from_code(undo_command_t* undo_command, uint8_t code)
{
    change_kind_t kind = CHARACTER;
    switch (code)
    {
    case KC_END:
        {
            kind = NO_CHANGE;
            break;
        }
    case KC_ENT:
        {
            kind = ENTER;
            break;
        }
    case KC_LEFT:
        {
            kind = MOVE_LEFT;
            break;
        }
    case KC_UP:
        {
            kind = MOVE_UP;
            break;
        }
    }
    undo_command_add_change(undo_command, kind);
    return kind;
}

uint8_t undo_command_get_changes_count(undo_command_t* undo_command, change_kind_t kind)
{
    uint8_t result = 0;
    for (int i = 0; i < MAX_CHANGES; ++i)
    {
        change_kind_t change_kind = undo_command->changes[i].kind;
        if (change_kind == kind)
        {
            result += undo_command->changes[i].count;
        }
        else if (change_kind == NO_CHANGE)
        {
            break;
        }
    }
    return result;
}

bool can_undo(undo_command_t* undo_command) { return undo_command->changes[0].kind != NO_CHANGE;}
undo_command_t g_undo_stack[MAX_UNDO] = { {0} };
int8_t g_undo_stack_index = 0;
undo_command_t g_new_undo_command; // Must be global for ErgoDox Infinity to avoid a lock between the two halves

uint16_t g_separator_mode = CKC_SEPMODE_SPC;
uint16_t g_case_mode = CKC_CASE_NORMAL;

// Steno keymap
const uint32_t PROGMEM g_steno_keymap[MATRIX_ROWS][MATRIX_COLS] = KEYMAP(
        // Left hand
        0,      0,          0,          0,          0,          0,          0,
        0,      0,          0,          L2,         L1,         L0,         S_ENT,
        0,      LP_U,       LP_O,       L_C,        L_W,        L_N,        
        L5,     LP_I,       L_A,        L_T,        L_H,        L_R,        SC_STAR,
        L4,     L3,         L_S,        0,          0,
                                                                SC_STAR,    SC_STAR,
                                                                            T_O,
                                                    SC_PLUS,    T_E,        T_A,
        // Right hand
        0,          0,          0,          0,          0,          0,          0,
        SC_SEP,     R0,         R1,         R2,         0,          0,          0,
                    R_R,        R_L,        R_C,        R3,         R4,         0, 
        SC_SEP,     R_N,        R_G,        R_H,        R_T,        RP_E,       RP_S,
                                0,          0,          R_S,        RP_Y,       RP_S,
        SC_STAR,    SC_STAR,
        T_A,
        T_U,        T_I,       SC_PLUS 
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*

#ifdef PROGRAMMER_COLEMAK_ENABLE
// PROGRAMMER COLEMAK
[LAYER_BASE ] = KEYMAP(
        // left hand
        _AMP,           _LBRC,      _LCBR,      _RCBR,      _LPRN,      _EQL,           KC_LGUI,
        _DLR,           _Q,         _W,         _F,         _P,         _G,             KC_INS,
        KC_TAB,         _A,         _R,         _S,         _T,         _D,  
        CKC_SFT,         _Z,         _X,         _C,         _V,         _B,             KC_ESC,
        KC_LCTL,        _AMP,       KC_LALT,    KC_UP,      KC_DOWN,    
                                                                                    MO(LAYER_FN),       TG(LAYER_FN),
                                                                                                        KC_PSCR,
#ifdef AZERTY_OS_ENABLE
                                                                            KC_ENT, KC_BSPC,            MO(LAYER_ACCENTS),
#else
                                                                            KC_ENT, KC_BSPC,            KC_NO,
#endif
        // right hand
                    TG(LAYER_STENO),    _ASTR,      _RPRN,      _PLUS,      _RBRC,      _EXLM,    _HASH,
                    KC_BSPC,            _J,         _L,         _U,         _Y,         _SCLN,    _SLSH,
                                        _H,         _N,         _E,         _I,         _O,       _MINS,
                    KC_ENT,             _K,         _M,         _COMM,      _DOT,       _APOS,    CKC_SFT,  
                                                    KC_LEFT,    KC_RIGHT,   _BSLS,      _AT,      KC_RCTL,
        TG(LAYER_FN),   MO(LAYER_FN),
        KC_NO,
#ifdef AZERTY_OS_ENABLE
        MO(LAYER_ACCENTS),  KC_DEL,     KC_SPC
#else
        KC_NO,              KC_DEL,     KC_SPC
#endif
),
#else
// This is the default ErgoDox EZ Qwerty layout
#define SYMB LAYER_FN
[LAYER_BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_EQL,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_NO,
        KC_DELT,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   TG(SYMB),
        KC_BSPC,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSFT,        CTL_T(KC_Z),  KC_X,   KC_C,   KC_V,   KC_B,   ALL_T(KC_NO),
        LT(SYMB,KC_GRV),KC_QUOT,      LALT(KC_LSFT),  KC_LEFT,KC_RGHT,
                                              ALT_T(KC_APP),  KC_LGUI,
                                                              KC_HOME,
                                               KC_SPC,KC_BSPC,KC_END,
        // right hand
             TG(LAYER_STENO),   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_MINS,
             TG(SYMB),          KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,             KC_BSLS,
                                KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,          GUI_T(KC_QUOT),
             MEH_T(KC_NO),      KC_N,   KC_M,   KC_COMM,KC_DOT, CTL_T(KC_SLSH),   KC_RSFT,
                                        KC_UP,  KC_DOWN,KC_LBRC,KC_RBRC,          KC_FN1,
             KC_LALT,        CTL_T(KC_ESC),
             KC_PGUP,
             KC_PGDN,KC_TAB, KC_ENT
    ),
#endif

// STENO LAYER
[LAYER_STENO] = KEYMAP(
        // left hand
        KC_F1,        KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,        KC_LGUI,
        KC_LALT,      KC_TAB,     CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,    CKC_STENO,
        KC_LCTL,      CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,   
        CKC_STENO,    CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,    CKC_STENO,
        CKC_STENO,    CKC_STENO,  CKC_STENO,      KC_TRNS,    KC_TRNS,
                                                                                    CKC_STENO,    CKC_STENO,
                                                                                                  CKC_STENO,
                                                                        CKC_STENO,  CKC_STENO,    CKC_STENO,
        // right hand
                    KC_TRNS,        KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
                    CKC_STENO,      CKC_STENO,  CKC_STENO,  CKC_STENO,  KC_BSPC,    KC_DEL,     KC_RALT,
                                    CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  KC_RCTL,   
                    CKC_STENO,      CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,   
                                                KC_TRNS,    KC_TRNS,    CKC_STENO,  CKC_STENO,  CKC_STENO,
        CKC_STENO,    CKC_STENO,
        CKC_STENO,
        CKC_STENO,    CKC_STENO,    CKC_STENO 
),

#ifdef PROGRAMMER_COLEMAK_ENABLE
#ifdef AZERTY_OS_ENABLE
// PROGRAMMER COLEMAK SHIFTED LAYER (for AZERTY OS)
[LAYER_SHIFT_COLEMAK] = KEYMAP(
        FR_UGRV,        KC_7,       KC_5,       KC_3,       KC_1,       KC_9,           KC_TRNS,
        CKC_SFUN,       KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        FR_UGRV,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
    // right hand
                    KC_NO,          KC_0,       KC_2,       KC_4,           KC_6,           KC_8,           CKC_SFUN,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        CKC_SFUN,       FR_COMM,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        CKC_SFUN,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    CKC_SFUN,       CKC_SFUN,       CKC_SFUN,         KC_TRNS,
                                                KC_TRNS,    KC_TRNS,        CKC_SFUN,       CKC_SFUN,         KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
),

// Accents Layer (for AZERTY OS)
[LAYER_ACCENTS] = KEYMAP(
       // left hand
        KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,          KC_NO,          KC_NO,
        KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,          KC_NO,          KC_NO,  
        KC_NO,          KC_0,       KC_NO,      KC_NO,      KC_NO,          KC_NO,  
        KC_NO,          KC_NO,      KC_NO,      KC_9,       KC_NO,          KC_NO,          KC_NO,  
        KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,
                                                                                        KC_NO,      KC_NO,  
                                                                                                    KC_NO,  
                                                                            KC_NO,      KC_NO,      KC_TRNS,  
       // right hand
                    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,
                    KC_NO,          KC_NO,      KC_NO,      FR_UGRV,    KC_NO,          LSFT(KC_LBRC),    KC_NO,  
                                    KC_7,       KC_2,       CKC_CIRC,   CKC_CIRC,       CKC_CIRC,   KC_NO,  
                    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,
                                                KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,  
        KC_NO,      KC_NO,  
        KC_NO,  
        KC_TRNS,      KC_NO,      KC_NO
),

#else
// PROGRAMMER COLEMAK SHIFTED LAYER (for QWERTY OS)
[LAYER_SHIFT_COLEMAK] = KEYMAP(
        KC_TRNS,        CKC_SFUN,   CKC_SFUN,   CKC_SFUN,   CKC_SFUN,   CKC_SFUN,       KC_5,
        KC_GRV,         KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_5,       KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
    // right hand
                    KC_TRNS,        CKC_SFUN,   CKC_SFUN,   CKC_SFUN,       CKC_SFUN,       CKC_SFUN,       CKC_SFUN,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,   
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
                                                KC_TRNS,    KC_TRNS,        KC_TRNS,        CKC_SFUN,       KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
),
#endif
#endif

// F1-F12 Layer
[LAYER_FN] = KEYMAP(
       // left hand
        KC_F1,          KC_F2,      KC_F3,      KC_F4,      KC_F5,          KC_F6,          RESET,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_LSFT,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_PGUP,    KC_PGDN,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
       // right hand
                    KC_NO,          KC_F7,      KC_F8,      KC_F9,      KC_F10,         KC_F11,     KC_F12,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_TRNS,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_TRNS,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_RSFT,
                                                KC_HOME,    KC_END,     KC_TRNS,        KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
),

};

const uint16_t PROGMEM fn_actions[] = 
{ 
#ifdef SYMB
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
#endif
};

bool can_stroke(void) { return (g_bits_keys_pressed_part1 == 0) && (g_bits_keys_pressed_part2 == 0); }

void send_mods_and_code(uint8_t mods, uint8_t code)
{
    const uint8_t original_mods = get_mods();
    set_mods(mods);
    register_code(code);
    set_mods(original_mods);
}

bool is_letter(uint8_t code)
{
#ifdef AZERTY_OS_ENABLE
    return (code != FR_COMM) && (((code >= KC_A) && (code <= KC_Z)) || (code == FR_M));
#else
    return (code >= KC_A) && (code <= KC_Z);
#endif
}

void stroke(void)
{
    // Init stroke buffer
    stroke_init_buffer();
    
    // Send characters for each key family
    const uint8_t original_mods = get_mods();
    del_mods(MOD_LSFT|MOD_RSFT);

    bool initial_case = false;
    g_new_undo_command.change_index = 0;
    for (int i = 0; i < MAX_CHANGES; ++i)
    {
        g_new_undo_command.changes[i].kind = NO_CHANGE;
        g_new_undo_command.changes[i].count = 0;
    }

    // Get *, + and case controls info
    const uint8_t special_controls_bits = g_family_bits[FAMILY_SPECIAL_CONTROLS];
    const bool has_star = special_controls_bits & (1 << (SC_STAR & 0xF));
    const bool has_plus = special_controls_bits & (1 << (SC_PLUS & 0xF));
    const bool has_separator = special_controls_bits & (1 << (SC_SEP & 0xF));
    const uint8_t thumbs_bits = g_family_bits[FAMILY_THUMBS];
    const uint8_t left_pinky_bits = g_family_bits[FAMILY_LEFT_PINKY];

    // Reset L3 if LP_I is pressed
    if (left_pinky_bits & STENO_KEY_BIT(LP_I))
    {
        g_family_bits[FAMILY_LEFT_CONTROLS] &= ~STENO_KEY_BIT(L3);
    }

    // Build stroke (but we don't send it yet)
    uint16_t left_pinky_keycode = 0;
    bool undo_allowed = true;
    for (int family_id = 0; family_id < NB_FAMILY; ++family_id)
    {
        uint8_t family_bits = g_family_bits[family_id];
        if (family_bits == 0)
        {
            continue;
        }

        // Get the lookup table
        undo_allowed = family_id == FAMILY_SPECIAL_CONTROLS;
        void* any_table = g_all_tables[family_id];
        uint8_t kind = g_family_to_kind_table[family_id];
        if (family_id == FAMILY_THUMBS)
        {
            if (has_star)
            {
                any_table = (void*)g_thumbs_bigrams_table;
            }
        }
        else if (family_id == FAMILY_LEFT_HAND)
        {
            if (!thumbs_bits && has_star)
            {
                any_table = (void*)g_left_punctuations_table;
                kind = KIND_PUNCTUATIONS;
            }

            // Check left pinky value
            if (left_pinky_keycode == _O || left_pinky_keycode == _U)
            {
                // L_A become L_S, so get the bit state of L_A and put it on L_S
                family_bits &= ~STENO_KEY_BIT(L_S); // Clear L_S
                family_bits |= (family_bits & STENO_KEY_BIT(L_A)) << 1; // Transfert L_A to L_S
                family_bits |= STENO_KEY_BIT(L_A); // LP_O or LP_U become L_A
            }
        }
        else if (family_id == FAMILY_RIGHT_HAND)
        {
            if (!thumbs_bits && has_star)
            {
                any_table = (void*)g_right_punctuations_table;
                kind = KIND_PUNCTUATIONS;
            }
        }

        if (any_table)
        {
            switch (kind)
            {
            case KIND_ONE_KEYCODE:
                {
                    one_keycode_table_t* one_keycode_table = (one_keycode_table_t*)any_table;
                    const uint16_t word = pgm_read_word(&(one_keycode_table[family_bits][0]));
                    if (word)
                    {
                        if (family_id == FAMILY_LEFT_CONTROLS)
                        {
                            if ((word >= CKC_CASE_NORMAL) && (word <= CKC_CASE_UPPER_LOCKED))
                            {
                                g_case_mode = word;
                            }
                        }
                        else if (family_id == FAMILY_LEFT_PINKY)
                        {
                            left_pinky_keycode = word;
                            if (word == _I)
                            {
                                g_family_bits[FAMILY_LEFT_HAND] |= STENO_KEY_BIT(L_A);
                            }
                            else if (word == _O || word == _U)
                            {
                                g_family_bits[FAMILY_LEFT_HAND] |= STENO_KEY_BIT(L_S);
                            }
                        }
                    }
                    break;
                }
            case KIND_LETTERS:
                {
                    letters_table_t* letters_table = (letters_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_LETTERS; ++code_pos)
                    {
                        uint8_t byte = pgm_read_byte(&(letters_table[family_bits][code_pos]));
                        if (byte)
                        {
                            // Left pinky vowels
                            if (family_id == FAMILY_LEFT_HAND)
                            {
                                if (byte == _A)
                                {
                                    if (left_pinky_keycode != 0 && left_pinky_keycode < CKC_STENO)
                                    {
                                        byte = (uint8_t)left_pinky_keycode;
                                    }
                                }
                            }

                            stroke_add_element(kind, byte);

                            // Jackdaw rule: If a 'Q' is detected on the left hand followed by a thumb vowel => add a 'U'
                            if (byte == _Q && thumbs_bits)
                            {
                                stroke_add_element(kind, _U);
                            }

                            // Jackdaw rule: Double the first letter for the right hand only if + is in the stroke
                            if (has_plus && (family_id == FAMILY_RIGHT_HAND) && !code_pos)
                            {
                                stroke_add_element(kind, byte);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            case KIND_SYMBOLS:
                {
                    symbols_table_t* symbols_table = (symbols_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_SYMBOLS; ++code_pos)
                    {
                        const uint16_t word = pgm_read_word(&(symbols_table[family_bits][code_pos]));
                        if (word)
                        {
                            switch (word)
                            {
                            case CKC_CASE_NORMAL:
                            case CKC_CASE_INNER_ONCE:
                            case CKC_CASE_INNER_LOCKED:
                            case CKC_CASE_UPPER_ONCE:
                            case CKC_CASE_UPPER_LOCKED:
                                {
                                    g_case_mode = word;
                                    break;
                                }
                            case CKC_SEPMODE_SPC:
                            case CKC_SEPMODE_CAMEL:
                            case CKC_SEPMODE_UNDS:
                            case CKC_SEPMODE_MINS:
                            case CKC_SEPMODE_SPC_UPPER:
                            case CKC_SEPMODE_UNDS_UPPER:
                            case CKC_SEPMODE_MINS_UPPER:
                                {
                                    g_separator_mode = word;
                                    break;
                                }
                            default:
                                {
                                    stroke_add_element(kind, word);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            case KIND_PUNCTUATIONS:
                {
                    punctuations_table_t* punctuations_table = (punctuations_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_PUNCTUATIONS; ++code_pos)
                    {
                        const uint16_t word = pgm_read_word(&(punctuations_table[family_bits][code_pos]));
                        if (word)
                        {
                            stroke_add_element(kind, word);
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }

    // Check if we can send the separator (space, underscore...) before the stroke
    if (g_stroke_buffer_count || (!g_stroke_buffer_count && has_separator && !has_star))
    {
        uint16_t keycode_separator = 0;
        switch (g_separator_mode)
        {
        case CKC_SEPMODE_SPC:
            {
                keycode_separator = KC_SPC;
                break;
            }
        case CKC_SEPMODE_UNDS:
            {
                keycode_separator = _UNDS;
                break;
            }
        case CKC_SEPMODE_MINS:
            {
                keycode_separator = _MINS;
                break;
            }
        case CKC_SEPMODE_CAMEL:
            {
                initial_case = !has_separator;
                break;
            }
        case CKC_SEPMODE_SPC_UPPER:
            {
                initial_case = false;
                add_mods(MOD_LSFT);
                keycode_separator = KC_SPC;
                break;
            }
        case CKC_SEPMODE_UNDS_UPPER:
            {
                initial_case = false;
                add_mods(MOD_LSFT);
                keycode_separator = _UNDS;
                break;
            }
        case CKC_SEPMODE_MINS_UPPER:
            {
                initial_case = false;
                add_mods(MOD_LSFT);
                keycode_separator = _MINS;
                break;
            }
        default:
            {
                break;
            }
        }

        bool can_send_separator = true;
        if (g_stroke_buffer_count)
        {
            // Check the first keycode in the stroke
            stroke_element_t* stroke_element = &g_stroke_buffer[0];
            switch (stroke_element->keycode)
            {
            case _COMM:
            case _DOT:
            case _SCLN:
            case KC_ESC:
            case KC_ENT:
            case KC_END:
            case KC_HOME:
                {
                    can_send_separator = false;
                    break;
                }
            default:
                {
                    break;
                }
            }
        }

        if (can_send_separator && keycode_separator && (!has_separator || (!g_stroke_buffer_count && has_separator && !has_star)))
        {
            send_mods_and_code(keycode_separator >> 8, keycode_separator);
            unregister_code(keycode_separator);
            undo_command_add_change(&g_new_undo_command, CHARACTER);
        }
    }

    if (initial_case || (g_case_mode > CKC_CASE_NORMAL))
    {
        add_mods(MOD_LSFT);
    }

    // Send stroke buffer
    uint8_t inserted_characters = 0;
    for (uint8_t i = 0; i < g_stroke_buffer_count; ++i)
    {
        stroke_element_t* stroke_element = &g_stroke_buffer[i];
        switch (stroke_element->kind)
        {
        case KIND_LETTERS:
            {
                register_code(stroke_element->keycode);
                unregister_code(stroke_element->keycode);
                undo_command_add_change(&g_new_undo_command, CHARACTER);
                inserted_characters++;
                break;
            }
        case KIND_SYMBOLS:
            {
                const uint8_t code = (uint8_t)stroke_element->keycode;
                if (is_letter(code))
                {
                    // By doing this the shift mod can be applied on letter code
                    register_code(code);
                }
                else
                {
                    send_mods_and_code(stroke_element->keycode >> 8, code);
                }
                unregister_code(code);
                if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                {
                    inserted_characters++;
                }
                break;
            }
        case KIND_PUNCTUATIONS:
            {
                // TODO: Use another table for specific key sequences
                uint16_t specific_sequence[5] = {0};
                uint16_t word = stroke_element->keycode;
                switch (word)
                {
                case CKC_DELWORD:
                    {
                        specific_sequence[0] = LCTL(LSFT(KC_LEFT));
                        specific_sequence[1] = KC_BSPC;
                        break;
                    }
                case CKC_DLEFT:
                    {
                        specific_sequence[0] = KC_LEFT;
                        specific_sequence[1] = KC_LEFT;
                        break;
                    }
                case CKC_ENTABOVE:
                    {
                        specific_sequence[0] = KC_UP;
                        specific_sequence[1] = KC_END;
                        specific_sequence[2] = KC_ENT;
                        break;
                    }
                default:
                    {
                        const uint8_t code = (uint8_t)word;
                        send_mods_and_code(word >> 8, code);
                        unregister_code(code);
                        if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                        {
                            inserted_characters++;
                        }
                        break;
                    }
                }

                // Specific key sequence if any
                for (int i = 0; i < 5; ++i)
                {
                    const uint16_t word = specific_sequence[i];
                    if (word == 0)
                    {
                        break;
                    }
                    const uint8_t code = (uint8_t)word;
                    send_mods_and_code(word >> 8, code);
                    unregister_code(code);
                    if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                    {
                        inserted_characters++;
                    }
                }
                break;
            }
        default:
            {
                break;
            }
        }

        // Camel case
        if (inserted_characters == 1)
        {
            switch (g_case_mode)
            {
            case CKC_CASE_INNER_ONCE:
                {
                    g_case_mode = CKC_CASE_NORMAL;
                    initial_case = true;
                    break;
                }
            case CKC_CASE_INNER_LOCKED:
                {
                    initial_case = true;
                    break;
                }
            default:
                {
                    break;
                }
            }

            if (initial_case)
            {
                del_mods(MOD_LSFT);
            }
        }
    }

    if (inserted_characters && (g_case_mode == CKC_CASE_UPPER_ONCE))
    {
        g_case_mode = CKC_CASE_NORMAL;
        del_mods(MOD_LSFT);
    }

    if (can_undo(&g_new_undo_command))
    {
        // Undo history
        if (g_undo_stack_index == MAX_UNDO)
        {
            g_undo_stack_index = 0;
        }

        g_undo_stack[g_undo_stack_index++] = g_new_undo_command;
    }
    else if (has_star)
    {
        // Compute the previous index
        int8_t previous_index = g_undo_stack_index - 1;
        if (previous_index < 0)
        {
            previous_index = MAX_UNDO - 1;
        }

        // Check if we have data to undo at previous_index
        undo_command_t* previous_undo_command = &g_undo_stack[previous_index];
        if (can_undo(previous_undo_command))
        {
            if (has_separator)
            {
                // SC_SEP becomes a Backspace
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                
                // Patch chars to delete for the next undo
                for (int i = previous_undo_command->change_index; i >= 0; --i)
                {
                    if (previous_undo_command->changes[i].kind == CHARACTER)
                    {
                        previous_undo_command->changes[i].count--;
                        break;
                    }
                }

                // If there is no more data to remove at previous_index we can go to the previous stroke undo data 
                const uint8_t inserted_characters = undo_command_get_changes_count(&g_undo_stack[previous_index], CHARACTER);
                if (inserted_characters == 0)
                {
                    g_undo_stack_index = previous_index;
                }
            }
            else if (undo_allowed)
            {
                // Parse changes in reverse order and undo them
                for (int i = previous_undo_command->change_index; i >= 0; --i)
                {
                    change_t* change = &previous_undo_command->changes[i];
                    for (int j = 0; j < change->count; ++j)
                    {
                        switch (change->kind)
                        {
                        case CHARACTER:
                            {
                                register_code(KC_BSPC);
                                unregister_code(KC_BSPC);
                                break;
                            }
                        case MOVE_LEFT:
                            {
                                register_code(KC_RIGHT);
                                unregister_code(KC_RIGHT);
                                break;
                            }
                        case ENTER:
                            {
                                // We use SHIFT to select text. We have to do this for text editors that autoindent text.
                                // Vim users needs to add : 'behave mswin' in their .vimrc file
                                register_code(KC_LSFT);
                                register_code(KC_UP);
                                unregister_code(KC_UP);
                                register_code(KC_END);
                                unregister_code(KC_END);
                                unregister_code(KC_LSFT);

                                register_code(KC_DEL);
                                unregister_code(KC_DEL);
                                break;
                            }
                        case MOVE_UP:
                            {
                                register_code(KC_DOWN);
                                unregister_code(KC_DOWN);
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }
                }

                g_undo_stack_index = previous_index;
            }
        }
        else if (has_separator)
        {
            // No data to remove in the undo stack, but we allow the SC_SEP to become a Backspace
            register_code(KC_BSPC);
            unregister_code(KC_BSPC);
        }
    }

    // Restore original mods
    set_mods(original_mods);

    // Clear bits
    for (int i = 0; i < NB_FAMILY; ++i)
    {
        g_family_bits[i] = 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode)
    {
    case CKC_STENO:
        {
            const uint32_t dword = pgm_read_dword(&(g_steno_keymap[record->event.key.row][record->event.key.col]));
            if (dword & STENO_BIT)
            {
                const uint32_t bit_key = 1L << (dword & 0x0F);
                const uint8_t family = (dword >> 4) & 0x0F;
                const uint8_t family_offset = g_family_to_bit_offset[family];
                uint32_t* keys_pressed_p = g_family_to_keys_pressed[family];
                if (record->event.pressed)
                {
                    (*keys_pressed_p) |= (bit_key << family_offset);
                    g_family_bits[family] |= bit_key;
                }
                else
                {
                    (*keys_pressed_p) &= ~(bit_key << family_offset);

                    // Stroke if all steno keys are released
                    if (can_stroke())
                    {
                        stroke();
                    }
                }
            }
            return false;
        }
#ifdef PROGRAMMER_COLEMAK_ENABLE
    case CKC_SFT: // Apply SHIFT and go to LAYER_SHIFT_COLEMAK
        {
            const uint8_t shift_key = record->event.key.row == 0 ? KC_LSFT : KC_RSFT;
            if (record->event.pressed)
            {
                register_code(shift_key);
                layer_on(LAYER_SHIFT_COLEMAK);
            }
            else
            {
                unregister_code(shift_key);
                layer_off(LAYER_SHIFT_COLEMAK);
            }
            return false;
        }
    case CKC_SFUN: // Handle special shift codes for the Programmer Colemak layout
        {
            const uint16_t keycode = keymap_key_to_keycode(LAYER_BASE , record->event.key);
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
#ifdef AZERTY_OS_ENABLE
    case CKC_CIRC:
        {
            if (record->event.pressed)
            {
                const uint16_t keycode = keymap_key_to_keycode(LAYER_BASE , record->event.key);
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                register_code(keycode);
                unregister_code(keycode);
            }
            return false;
        }
#endif
#endif
    default:
        {
            break;
        }
    }
    return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void)
{
    uint8_t layer = biton32(layer_state);

    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    switch (layer)
    {
    case LAYER_STENO:
        ergodox_right_led_1_on();
        break;
    case LAYER_FN:
        ergodox_right_led_2_on();
        break;
    default:
        break;
    }

    if (!can_stroke())
    {
        ergodox_right_led_3_on();
    }
}
