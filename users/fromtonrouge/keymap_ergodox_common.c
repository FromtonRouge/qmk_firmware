#include "colemak.h"
#include "undo.h"
#include "stroke.h"
#include "lookup_tables/shelton_tables.h"
#include "lookup_tables/user_tables.h"
#include "lookup_tables/tables.h"
#include "quantum/keymap_extras/keymap_french.h"
#include "tmk_core/common/action_layer.h"
#include "tmk_core/common/action_util.h"

// Layer indexes
#define L_BASE 0
#define L_STENO 1

#ifdef PROGRAMMER_COLEMAK_ENABLE
    #define L_SHIFT_COLEMAK 2
    #ifdef AZERTY_OS_ENABLE
        #define L_ACCENTS 3
        #define L_FN 4
    #else
        #define L_FN 3
    #endif
#else
    #define L_FN 2
#endif

// Steno keymap
const uint32_t PROGMEM g_steno_keymap[MATRIX_ROWS][MATRIX_COLS] = LAYOUT_ergodox(
        // Left hand
        0,      0,          0,          0,          0,          0,          0,
        0,      0,          0,          L2,         L1,         L0,         S_ENT,
        0,      0,          L3,         L_C,        L_W,        L_N,        
        0,      L4,         L_A,        L_T,        L_H,        L_R,        SC_STAR,
        0,      L5,         L_S,        0,          0,
                                                                SC_STAR,    SC_STAR,
                                                                            T_O,
                                                    SC_LPLUS,   T_E,        T_A,
        // Right hand
        0,          0,          0,          0,          0,          0,          0,
        SC_SEP,     R0,         R1,         R2,         0,          0,          0,
                    R_R,        R_L,        R_C,        R3,         R4,         0, 
        SC_SEP,     R_N,        R_G,        R_H,        R_T,        RP_E,       0,
                                0,          0,          R_S,        RP_Y,       0,
        SC_STAR,    SC_STAR,
        T_Y,
        T_U,        T_I,       SC_RPLUS 
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*

#ifdef PROGRAMMER_COLEMAK_ENABLE
[L_BASE ] = LAYOUT_ergodox(
        // left hand
        _AMP,           _LBRC,      _LCBR,      _RCBR,      _LPRN,      _EQL,           KC_LGUI,
        _DLR,           _Q,         _W,         _F,         _P,         _G,             KC_INS,
        KC_TAB,         _A,         _R,         _S,         _T,         _D,  
        CKC_SFT,        _Z,         _X,         _C,         _V,         _B,             KC_ESC,
        KC_LCTL,        _AMP,       KC_LALT,    MO(L_FN),   KC_ENT,    
                                                                                    MO(L_FN),       TG(L_FN),
                                                                                                        KC_PSCR,
#ifdef AZERTY_OS_ENABLE
                                                                            KC_BSPC,    MO(L_ACCENTS), KC_NO,
#else
                                                                            KC_BSPC,    KC_NO, KC_NO,
#endif
        // right hand
                    TG(L_STENO),    _ASTR,      _RPRN,      _PLUS,      _RBRC,      _EXLM,    _HASH,
                    KC_BSPC,            _J,         _L,         _U,         _Y,         _SCLN,    _SLSH,
                                        _H,         _N,         _E,         _I,         _O,       _MINS,
                    KC_NO,              _K,         _M,         _COMM,      _DOT,       _APOS,    CKC_SFT,  
                                                    KC_SPC,     MO(L_FN),   _BSLS,      _AT,      KC_RCTL,
        TG(L_FN),   MO(L_FN),
        KC_NO,
#ifdef AZERTY_OS_ENABLE
        KC_NO,  MO(L_ACCENTS),   KC_DEL
#else
        KC_NO,  KC_NO,   KC_DEL
#endif
),
#else
// This is the default ErgoDox EZ Qwerty layout
#define SYMB L_FN
[L_BASE] = LAYOUT_ergodox(  // layer 0 : default
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
             TG(L_STENO),   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_MINS,
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
[L_STENO] = LAYOUT_ergodox(
        // left hand
        KC_F1,        KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,        KC_LGUI,
        KC_TAB,       KC_NO,      KC_NO,      CKC_STENO,  CKC_STENO,  CKC_STENO,    CKC_STENO,
        KC_LALT,      KC_ESC,     CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,   
        KC_LSFT,      CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,    CKC_STENO,
        KC_LCTL,      CKC_STENO,  CKC_STENO,  KC_TRNS,    KC_TRNS,
                                                                                    CKC_STENO,    CKC_STENO,
                                                                                                  CKC_STENO,
                                                                        CKC_STENO,  CKC_STENO,    CKC_STENO,
        // right hand
                    KC_TRNS,        KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
                    CKC_STENO,      CKC_STENO,  CKC_STENO,  CKC_STENO,  KC_NO,      KC_DEL,     KC_BSPC,
                                    CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  KC_RALT,   
                    CKC_STENO,      CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  CKC_STENO,  KC_RSFT,     
                                                KC_TRNS,    KC_TRNS,    CKC_STENO,  CKC_STENO,  KC_RCTL,  
        CKC_STENO,    CKC_STENO,
        CKC_STENO,
        CKC_STENO,    CKC_STENO,    CKC_STENO 
),

#ifdef PROGRAMMER_COLEMAK_ENABLE
#ifdef AZERTY_OS_ENABLE
// PROGRAMMER COLEMAK SHIFTED LAYER (for AZERTY OS)
[L_SHIFT_COLEMAK] = LAYOUT_ergodox(
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
[L_ACCENTS] = LAYOUT_ergodox(
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
[L_SHIFT_COLEMAK] = LAYOUT_ergodox(
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
[L_FN] = LAYOUT_ergodox(
       // left hand
        KC_F1,          KC_F2,      KC_F3,      KC_F4,      KC_F5,          KC_F6,          RESET,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_HOME,    KC_PGUP,    KC_PGDN,    KC_END,         KC_TRNS,
        KC_LSFT,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,    KC_TRNS,
                                                                                                    KC_TRNS,
                                                                            KC_TRNS,    KC_TRNS,    KC_TRNS,
        // right hand
                    KC_NO,          KC_F7,      KC_F8,      KC_F9,      KC_F10,         KC_F11,     KC_F12,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_ESC,     KC_TRNS,        KC_TRNS,    KC_TRNS,
                                    KC_TRNS,    KC_LEFT,    KC_DOWN,    KC_UP,          KC_RIGHT,   KC_TRNS,
                    KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_RSFT,
                                                KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
),

};

const uint16_t PROGMEM fn_actions[] = 
{ 
#ifdef SYMB
    [1] = ACTION_L_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
#endif
};

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
    case CKC_SFT: // Apply SHIFT and go to L_SHIFT_COLEMAK
        {
            const uint8_t shift_key = record->event.key.row == 0 ? KC_LSFT : KC_RSFT;
            if (record->event.pressed)
            {
                register_code(shift_key);
                layer_on(L_SHIFT_COLEMAK);
            }
            else
            {
                unregister_code(shift_key);
                layer_off(L_SHIFT_COLEMAK);
            }
            return false;
        }
    case CKC_SFUN: // Handle special shift codes for the Programmer Colemak layout
        {
            const uint16_t keycode = keymap_key_to_keycode(L_BASE , record->event.key);
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
                const uint16_t keycode = keymap_key_to_keycode(L_BASE , record->event.key);
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
    case L_STENO:
        ergodox_right_led_1_on();
        break;
    case L_FN:
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
