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

// Macro indexes
#define STENO 0
#ifdef PROGRAMMER_COLEMAK_ENABLE
    #define GO_SFT 1
    #define SP_SFT 2
    #ifdef AZERTY_OS_ENABLE
        #define CIRC 3
    #endif
#endif

#ifdef SUBPROJECT_infinity
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
#endif

// Keys family
enum key_family
{
    FAMILY_UNKNOWN,
    FAMILY_SPECIAL_CONTROLS,
    FAMILY_CASE_CONTROLS,
    FAMILY_LEFT_USER_SYMBOLS,
    FAMILY_LEFT_HAND,
    FAMILY_THUMBS,
    FAMILY_RIGHT_HAND,
    FAMILY_RIGHT_PINKY,
    FAMILY_RIGHT_USER_SYMBOLS,
    FAMILY_SPACES,
    NB_FAMILY
};

// Bit to identify a steno key
#define STENO_BIT (1L << 31) 

// 4 bits for star and the plus key
#define OFFSET_SPECIAL_CONTROLS 0
#define SC_STAR (0 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)
#define SC_LPLUS (1 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)
#define SC_RPLUS (2 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT)
#define SC_MSPC (3 | (FAMILY_SPECIAL_CONTROLS << 4) | STENO_BIT) // META SPACE = No space key, Backspace when used with SC_STAR, space key otherwise

// 8 bits for the left hand
#define OFFSET_LEFT_HAND 4
#define L_N (0 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_R (1 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_W (2 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_H (3 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_C (4 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_T (5 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_A (6 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)
#define L_S (7 | (FAMILY_LEFT_HAND << 4) | STENO_BIT)

// 5 bits for thumbs
#define OFFSET_THUMBS 12
#define T_E (0 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_O (1 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_A (2 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_U (3 | (FAMILY_THUMBS << 4) | STENO_BIT)
#define T_I (4 | (FAMILY_THUMBS << 4) | STENO_BIT)

// 8 bits for the right hand
#define OFFSET_RIGHT_HAND 17
#define R_R (0 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_N (1 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_L (2 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_G (3 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_C (4 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_H (5 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_T (6 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)
#define R_S (7 | (FAMILY_RIGHT_HAND << 4) | STENO_BIT)

// 3 bits for E and Y and S
#define OFFSET_RIGHT_PINKY 25
#define RP_E  (0 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)
#define RP_Y  (1 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)
#define RP_S  (2 | (FAMILY_RIGHT_PINKY << 4) | STENO_BIT)

// 2 bits for space control keys
#define OFFSET_SPACE_CONTROLS 28
#define S_TAB  (0 | (FAMILY_SPACES << 4) | STENO_BIT)
#define S_ENT  (1 | (FAMILY_SPACES << 4) | STENO_BIT)

// 2 bits for case control keys (upper case, initial case)
#define OFFSET_CASE_CONTROLS 30
#define C_UC  (0 | (FAMILY_CASE_CONTROLS << 4) | STENO_BIT)
#define C_IC  (1 | (FAMILY_CASE_CONTROLS << 4) | STENO_BIT)

// 6 bits for left user symbols
#define OFFSET_LEFT_USER_SYMBOLS 0
#define USRL_0  (0 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRL_1  (1 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRL_2  (2 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRL_3  (3 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRL_4  (4 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRL_5  (5 | (FAMILY_LEFT_USER_SYMBOLS << 4) | STENO_BIT)

// 6 bits for right user symbols
#define OFFSET_RIGHT_USER_SYMBOLS 6
#define USRR_0  (0 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRR_1  (1 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRR_2  (2 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRR_3  (3 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRR_4  (4 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)
#define USRR_5  (5 | (FAMILY_RIGHT_USER_SYMBOLS << 4) | STENO_BIT)

#define CONVERT_TO_NUMBERS_BITS(hand_bits) ((((((hand_bits & 0xE0) >> 1) | (hand_bits & 0xF)) & 0x78) >> 1 | (hand_bits & 0x3)) >> 1)

// Table to convert family id to bit offset
const uint8_t g_family_to_bit_offset[NB_FAMILY] =
{
    0,
    OFFSET_SPECIAL_CONTROLS,
    OFFSET_CASE_CONTROLS,
    OFFSET_LEFT_USER_SYMBOLS,
    OFFSET_LEFT_HAND,
    OFFSET_THUMBS,
    OFFSET_RIGHT_HAND,
    OFFSET_RIGHT_PINKY,
    OFFSET_RIGHT_USER_SYMBOLS,
    OFFSET_SPACE_CONTROLS
};

// A lookup table can stores letters (8 bits) or symbols (16 bits)
enum kind_table
{
    KIND_UNKNOWN,
    KIND_LETTERS,
    KIND_SYMBOLS,
    KIND_PUNCTUATIONS
};

const uint8_t g_family_to_kind_table[NB_FAMILY] =
{
    KIND_UNKNOWN,
    KIND_UNKNOWN,
    KIND_UNKNOWN,
    KIND_SYMBOLS,
    KIND_LETTERS,
    KIND_LETTERS,
    KIND_LETTERS,
    KIND_LETTERS,
    KIND_SYMBOLS,
    KIND_SYMBOLS
};

// Global vars for the steno layer
uint32_t g_bits_keys_pressed_part1 = 0;
uint32_t g_bits_keys_pressed_part2 = 0;

uint32_t* g_family_to_keys_pressed[NB_FAMILY] = 
{
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part2,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part1,
    &g_bits_keys_pressed_part2,
    &g_bits_keys_pressed_part1
};

uint8_t g_family_bits[NB_FAMILY] = {0};
typedef const uint8_t letters_table_t[MAX_LETTERS];
typedef const uint16_t symbols_table_t[MAX_SYMBOLS];
typedef const uint16_t punctuations_table_t[MAX_PUNCTUATIONS];
void* g_all_tables[NB_FAMILY] = 
{
    0,
    0,
    0,
    g_left_user_symbols_table,
    g_left_hand_table,
    g_thumbs_table,
    g_right_hand_table,
    g_right_pinky_table,
    g_right_user_symbols_table,
    g_spaces_ctl_table
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
typedef struct 
{
    uint8_t inserted_chars_count    :8;
    uint8_t left_arrow_count        :3;
} undo_command_t;
bool can_undo(undo_command_t undo) { return (undo.inserted_chars_count > 0) || (undo.left_arrow_count > 0);}
undo_command_t g_undo_stack[MAX_UNDO] = { {0} };
int8_t g_undo_stack_index = 0;

bool g_force_punctuations_on_right_hand = false; // Must be global for the Ergodox Infinity (avoid a lock)
bool g_has_left_plus = false; // Must be global for the Ergodox Infinity (avoid a lock)
bool g_has_right_plus = false; // Must be global for the Ergodox Infinity (avoid a lock)

// Steno keymap
const uint32_t PROGMEM g_steno_keymap[MATRIX_ROWS][MATRIX_COLS] = KEYMAP(
        // Left hand
        0,      0,          0,          0,          0,          0,          0,
        0,      0,          USRL_2,     USRL_3,     USRL_4,     USRL_5,     S_ENT,
        0,      USRL_0,     USRL_1,     L_C,        L_W,        L_N,        
        C_UC,   C_IC,       L_A,        L_T,        L_H,        L_R,        SC_STAR,
        C_UC,   C_IC,       L_S,        0,          0,
                                                                SC_STAR,    SC_STAR,
                                                                            T_O,
                                                    SC_LPLUS,   T_E,        T_A,
        // Right hand
        0,          0,          0,          0,          0,          0,          0,
        SC_MSPC,    USRR_5,     USRR_4,     USRR_3,     USRR_2,     0,          0,
                    R_R,        R_L,        R_C,        USRR_1,     USRR_0,     0, 
        SC_MSPC,    R_N,        R_G,        R_H,        R_T,        RP_E,       RP_S,
                                0,          0,          R_S,        RP_Y,       RP_S,
        SC_STAR,    SC_STAR,
        T_A,
        T_U,        T_I,       SC_RPLUS 
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
        M(GO_SFT),      _Z,         _X,         _C,         _V,         _B,             KC_ESC,
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
                    KC_ENT,             _K,         _M,         _COMM,      _DOT,       _APOS,    M(GO_SFT),  
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
#define ST_ON M(STENO)
[LAYER_STENO] = KEYMAP(
        // left hand
        KC_F1,        KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,        KC_BSPC,
        KC_LALT,      KC_TAB,     ST_ON,      ST_ON,      ST_ON,      ST_ON,        ST_ON,
        KC_LCTL,      ST_ON,      ST_ON,      ST_ON,      ST_ON,      ST_ON,   
        ST_ON,        ST_ON,      ST_ON,      ST_ON,      ST_ON,      ST_ON,        ST_ON,
        ST_ON,        ST_ON,      ST_ON,      KC_TRNS,    KC_TRNS,
                                                                                        ST_ON,    ST_ON,
                                                                                                  ST_ON,
                                                                            ST_ON,      ST_ON,    ST_ON,
        // right hand
                    KC_TRNS,        KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
                    ST_ON,          ST_ON,      ST_ON,      ST_ON,      ST_ON,      KC_DEL,     KC_RALT,
                                    ST_ON,      ST_ON,      ST_ON,      ST_ON,      ST_ON,      KC_RCTL,   
                    ST_ON,          ST_ON,      ST_ON,      ST_ON,      ST_ON,      ST_ON,      ST_ON,   
                                                KC_TRNS,    KC_TRNS,    ST_ON,      ST_ON,      ST_ON,
        ST_ON,    ST_ON,
        ST_ON,
        ST_ON,    ST_ON,    ST_ON 
),

#ifdef PROGRAMMER_COLEMAK_ENABLE
#ifdef AZERTY_OS_ENABLE
// PROGRAMMER COLEMAK SHIFTED LAYER (for AZERTY OS)
[LAYER_SHIFT_COLEMAK] = KEYMAP(
        FR_UGRV,        KC_7,       KC_5,       KC_3,       KC_1,       KC_9,           KC_TRNS,
        M(SP_SFT),      KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        FR_UGRV,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
    // right hand
                    KC_NO,          KC_0,       KC_2,       KC_4,           KC_6,           KC_8,           M(SP_SFT),
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        M(SP_SFT),      FR_COMM,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        M(SP_SFT),   
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    M(SP_SFT),      M(SP_SFT),      M(SP_SFT),      KC_TRNS,
                                                KC_TRNS,    KC_TRNS,        M(SP_SFT),      M(SP_SFT),      KC_TRNS,
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
                                    KC_7,       KC_2,       M(CIRC),    M(CIRC),        M(CIRC),    KC_NO,  
                    KC_NO,          KC_NO,      KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,
                                                KC_NO,      KC_NO,      KC_NO,          KC_NO,      KC_NO,  
        KC_NO,      KC_NO,  
        KC_NO,  
        KC_TRNS,      KC_NO,      KC_NO
),

#else
// PROGRAMMER COLEMAK SHIFTED LAYER (for QWERTY OS)
[LAYER_SHIFT_COLEMAK] = KEYMAP(
        KC_TRNS,        M(SP_SFT),  M(SP_SFT),  M(SP_SFT),  M(SP_SFT),  M(SP_SFT),      KC_5,
        KC_GRV,         KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_5,       KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
    // right hand
                    KC_TRNS,        M(SP_SFT),  M(SP_SFT),  M(SP_SFT),      M(SP_SFT),      M(SP_SFT),      M(SP_SFT),
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,   
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
                                                KC_TRNS,    KC_TRNS,        KC_TRNS,        M(SP_SFT),      KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
),
#endif
#endif

// F1-F12 Layer
[LAYER_FN] = KEYMAP(
       // left hand
        KC_TRNS,        KC_F1,      KC_F2,      KC_F3,      KC_F4,          KC_F5,          KC_F6,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,
        KC_LSFT,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_PGUP,    KC_PGDN,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
       // right hand
                    KC_F7,          KC_F8,      KC_F9,      KC_F10,     KC_F11,         KC_F12,     KC_NO,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_TRNS,
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_TRNS,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_RSFT,
                                                KC_HOME,    KC_END,     KC_TRNS,        KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        RESET,    KC_TRNS,    KC_TRNS
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
    // Send characters for each key family
    const uint8_t original_mods = get_mods();
    del_mods(MOD_LSFT|MOD_RSFT);
    bool initial_case_1 = false;
    bool initial_case_2 = false;
    undo_command_t new_undo_command;
    new_undo_command.inserted_chars_count = 0;
    new_undo_command.left_arrow_count = 0;

    // Get *, + and case controls info
    const uint8_t special_controls_bits = g_family_bits[FAMILY_SPECIAL_CONTROLS];
    const uint8_t thumbs_bits = g_family_bits[FAMILY_THUMBS];
    const uint8_t right_hand_bits = g_family_bits[FAMILY_RIGHT_HAND]; 
    const bool has_star = special_controls_bits & (1 << (SC_STAR & 0xF));
    g_has_left_plus = special_controls_bits & (1 << (SC_LPLUS & 0xF));
    g_has_right_plus = special_controls_bits & (1 << (SC_RPLUS & 0xF));
    const bool has_meta_space = special_controls_bits & (1 << (SC_MSPC & 0xF));
    const uint8_t case_controls_bits = g_family_bits[FAMILY_CASE_CONTROLS];
    if (case_controls_bits)
    {
        initial_case_1 = case_controls_bits == 2;
        initial_case_2 = case_controls_bits == 3;
        add_mods(MOD_LSFT);
    }

    // Evaluate stroke
    g_force_punctuations_on_right_hand = false;
    bool undo_allowed = true;
    bool no_space_code_detected = false;
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
        bool do_numbers = false;
        if (family_id == FAMILY_THUMBS)
        {
            if (g_has_left_plus && g_has_right_plus)
            {
                any_table = 0;
            }
            else if (has_star)
            {
                any_table = g_thumbs_bigrams_table;
            }
        }
        else if (family_id == FAMILY_LEFT_HAND)
        {
            if (!thumbs_bits)
            {
                if (g_has_left_plus)
                {
                    if (!right_hand_bits)
                    {
                        do_numbers = true;
                    }
                    else if (g_has_right_plus)
                    {
                        do_numbers = true;
                    }
                    else if (has_star)
                    {
                        do_numbers = true;
                        g_force_punctuations_on_right_hand = true;
                    }
                }
                else if (has_star)
                {
                    any_table = g_left_punctuations_table;
                    kind = KIND_PUNCTUATIONS;
                }
            }
        }
        else if (family_id == FAMILY_RIGHT_HAND)
        {
            bool do_punctuations = g_force_punctuations_on_right_hand;
            if (!thumbs_bits)
            {
                if (!do_punctuations && g_has_left_plus)
                {
                    if (g_has_right_plus)
                    {
                        do_numbers = true;
                    }
                }
                else if (has_star)
                {
                    if (g_has_right_plus)
                    {
                        do_numbers = true;
                    }
                    else
                    {
                        do_punctuations = true;
                    }
                }
            }

            if (do_punctuations)
            {
                any_table = g_right_punctuations_table;
                kind = KIND_PUNCTUATIONS;
            }
        }

        if (do_numbers)
        {
            family_bits = CONVERT_TO_NUMBERS_BITS(family_bits);
            any_table = g_numbers;
            kind = KIND_SYMBOLS;
        }

        if (any_table)
        {
            switch (kind)
            {
            case KIND_LETTERS:
                {
                    letters_table_t* letters_table = (letters_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_LETTERS; ++code_pos)
                    {
                        const uint8_t byte = pgm_read_byte(&(letters_table[family_bits][code_pos]));
                        if (byte)
                        {
                            register_code(byte);
                            unregister_code(byte);
                            new_undo_command.inserted_chars_count++;

                            // Jackdaw rule: If a 'Q' is detected on the left hand followed by a thumb vowel => add a 'U'
                            if (byte == _Q && thumbs_bits)
                            {
                                register_code(_U);
                                unregister_code(_U);
                                new_undo_command.inserted_chars_count++;
                            }

                            // Jackdaw rule: Double the first letter for the right hand only if + is in the stroke
                            if ((g_has_right_plus || g_has_left_plus) && (family_id == FAMILY_RIGHT_HAND) && !code_pos)
                            {
                                register_code(byte);
                                unregister_code(byte);
                                new_undo_command.inserted_chars_count++;
                            }

                            if ((initial_case_1 && new_undo_command.inserted_chars_count == 1) || (initial_case_2 && new_undo_command.inserted_chars_count == 2))
                            {
                                del_mods(MOD_LSFT);
                            }
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
                            const uint8_t code = (uint8_t)word;
                            if (is_letter(code))
                            {
                                // By doing this the shift mod can be applied on letter code
                                register_code(code);
                            }
                            else
                            {
                                send_mods_and_code(word >> 8, code);
                            }

                            unregister_code(code);

                            if (code == KC_LEFT)
                            {
                                new_undo_command.left_arrow_count++;
                            }
                            else
                            {
                                new_undo_command.inserted_chars_count++;
                            }

                            if ((initial_case_1 && new_undo_command.inserted_chars_count == 1) || (initial_case_2 && new_undo_command.inserted_chars_count == 2))
                            {
                                del_mods(MOD_LSFT);
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
                            if (word == _NOSPC)
                            {
                                no_space_code_detected = true;
                            }
                            else
                            {
                                const uint8_t code = (uint8_t)word;
                                send_mods_and_code(word >> 8, code);
                                unregister_code(code);

                                if (code == KC_LEFT)
                                {
                                    new_undo_command.left_arrow_count++;
                                }
                                else
                                {
                                    new_undo_command.inserted_chars_count++;
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
            }
        }
    }

    // Send automatically a space after a stroke or send explicitely when SC_MSPC is the only pressed key
    const bool send_space = (new_undo_command.inserted_chars_count > 0 && !has_meta_space) || (new_undo_command.inserted_chars_count == 0 && has_meta_space && !has_star);
    if (send_space && !no_space_code_detected)
    {
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        new_undo_command.inserted_chars_count++;
    }

    if (can_undo(new_undo_command))
    {
        // Undo history
        if (g_undo_stack_index == MAX_UNDO)
        {
            g_undo_stack_index = 0;
        }

        g_undo_stack[g_undo_stack_index++] = new_undo_command;
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
        undo_command_t previous_undo_command = g_undo_stack[previous_index];
        if (can_undo(previous_undo_command))
        {
            if (has_meta_space)
            {
                // Metaspace becomes a Backspace
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                g_undo_stack[previous_index].inserted_chars_count--; // Patch chars to delete for the next undo

                // If there is no more data to remove at previous_index we can go to the previous stroke undo data 
                if (g_undo_stack[previous_index].inserted_chars_count == 0)
                {
                    g_undo_stack_index = previous_index;
                }
            }
            else if (undo_allowed)
            {
                // Undo motion commands first (only left arrow at the moment)
                if (previous_undo_command.left_arrow_count)
                {
                    for (uint8_t i = 0; i < previous_undo_command.left_arrow_count; ++i)
                    {
                        register_code(KC_RIGHT);
                        unregister_code(KC_RIGHT);
                    }

                    g_undo_stack[previous_index].left_arrow_count = 0;
                }

                // Undo inserted characters
                if (previous_undo_command.inserted_chars_count)
                {
                    for (uint8_t i = 0; i < previous_undo_command.inserted_chars_count; ++i)
                    {
                        register_code(KC_BSPC);
                        unregister_code(KC_BSPC);
                    }

                    g_undo_stack[previous_index].inserted_chars_count = 0;
                }

                g_undo_stack_index = previous_index;
            }
        }
        else if (has_meta_space)
        {
            // No data to remove in the undo stack, but we allow the Metaspace to become a Backspace
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t macroId, uint8_t opt)
{
    switch (macroId)
    {
    case STENO:
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
                    if (family == FAMILY_CASE_CONTROLS)
                    {
                        register_code(KC_LSFT);
                    }

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

                    if (family == FAMILY_CASE_CONTROLS)
                    {
                        unregister_code(KC_LSFT);
                    }
                }
            }
            break;
        }
#ifdef PROGRAMMER_COLEMAK_ENABLE
    case GO_SFT: // Apply SHIFT and go to LAYER_SHIFT_COLEMAK
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
            break;
        }
    case SP_SFT: // Handle special shift codes for the Programmer Colemak layout
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
            break;
        }
#ifdef AZERTY_OS_ENABLE
    case CIRC:
        {
            if (record->event.pressed)
            {
                const uint16_t keycode = keymap_key_to_keycode(LAYER_BASE , record->event.key);
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                register_code(keycode);
                unregister_code(keycode);
            }
            break;
        }
#endif
#endif
    }
    return MACRO_NONE;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {}

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
