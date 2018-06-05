#include QMK_KEYBOARD_H

#include "colemak.h"
#include "undo.h"
#include "stroke.h"
#include "lookup_tables/shelton_tables.h"
#include "lookup_tables/user_tables.h"
#include "lookup_tables/tables.h"
#include "quantum/keymap_extras/keymap_french.h"
#include "tmk_core/common/action_layer.h"
#include "tmk_core/common/action_util.h"

#define L_BASE 0
#define L_SHIFT_COLEMAK 1
#define L_ACCENTS 2
#define L_FN1 3
#define L_FN2 4

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [L_BASE] = LAYOUT(
            _AMP,    _LBRC,  _LCBR, _RCBR,  _LPRN,  _EQL,   KC_LGUI,                                KC_NO,              _ASTR, _RPRN,   _PLUS, _RBRC,  _EXLM,   _HASH,
            _DLR,    _Q,     _W,    _F,     _P,     _G,     KC_INS,                                 KC_ESC,             _J,     _L,     _U,     _Y,     _SCLN,  _SLSH,
            KC_TAB,  _A,    _R,     _S,     _T,     _D,     MO(L_FN2),                           MO(L_FN1),       _H,     _N,     _E,     _I,     _O,     _MINS,
            CKC_SFT, _Z,    _X,     _C,     _V,     _B,     TG(L_FN2),                           TG(L_FN1),       _K,     _M,     _COMM,  _DOT,   _APOS,   CKC_SFT,
            KC_LCTL, _AMP,  KC_LALT, MO(L_FN2), KC_ENT, KC_BSPC,MO(L_ACCENTS),               MO(L_ACCENTS),  KC_DEL, KC_SPC, MO(L_FN1),  _BSLS,  _AT,  KC_RCTL
            ),

    [L_SHIFT_COLEMAK] = LAYOUT(
            FR_UGRV,    KC_7,       KC_5,       KC_3,       KC_1,       KC_9,       KC_TRNS,            KC_TRNS,     KC_0,      KC_2,       KC_4,       KC_6,       KC_8,       CKC_SFUN,
            CKC_SFUN,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,            KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    CKC_SFUN,   FR_COMM,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,            KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    CKC_SFUN,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,            KC_TRNS,     KC_TRNS,   KC_TRNS,    CKC_SFUN,   CKC_SFUN,   CKC_SFUN,   KC_TRNS,
            KC_TRNS,    FR_UGRV,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,            KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    CKC_SFUN,   CKC_SFUN,   KC_TRNS
            ),

    [L_ACCENTS] = LAYOUT(
            KC_NO,    KC_NO,   KC_NO,   KC_NO,     KC_NO,    KC_NO,   KC_NO,                       KC_NO,     KC_NO,     KC_NO,    KC_NO,       KC_NO,     KC_NO,          KC_NO,
            KC_NO,    KC_NO,   KC_NO,   KC_NO,     KC_NO,    KC_NO,   KC_NO,                       KC_NO,     KC_NO,     KC_NO,    FR_UGRV,     KC_NO,     LSFT(KC_LBRC),  KC_NO,
            KC_NO,    KC_0,    KC_NO,   KC_NO,     KC_NO,    KC_NO,   KC_NO,                       KC_NO,     KC_7,      KC_2,     CKC_CIRC,    CKC_CIRC,  CKC_CIRC,       KC_NO,
            KC_NO,    KC_NO,   KC_NO,   KC_9,      KC_NO,    KC_NO,   KC_NO,                       KC_NO,     KC_NO,     KC_NO,    KC_NO,       KC_NO,     KC_NO,          KC_NO,
            KC_NO,    KC_NO,   KC_NO,   KC_NO,     KC_NO,    KC_NO,   KC_TRNS,                     KC_TRNS,   KC_NO,     KC_NO,    KC_NO,       KC_NO,     KC_NO,          KC_NO
            ),

    [L_FN1] = LAYOUT(
            KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,    KC_NO,            KC_NO,       KC_F7,     KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_PSCR,          KC_PSCR,     KC_TRNS,   KC_TRNS,    KC_ESC,     KC_TRNS,    KC_TRNS,    FR_COMM,
            KC_TRNS,    KC_HOME,    KC_PGUP,    KC_PGDN,    KC_END,     KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS,   KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,   KC_TRNS,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
            KC_TRNS,    FR_UGRV,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS
            ),

    [L_FN2] = LAYOUT(
            KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,    KC_NO,            KC_NO,       KC_F7,     KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_PSCR,          KC_NLCK,     KC_TRNS,   KC_P7,      KC_P8,      KC_P9,      KC_TRNS,    FR_COMM,
            KC_TRNS,    KC_HOME,    KC_PGUP,    KC_PGDN,    KC_END,     KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS,   KC_P4,      KC_P5,      KC_P6,      KC_TRNS,    KC_TRNS,
            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_P0,     KC_P1,      KC_P2,      KC_P3,      KC_TRNS,    KC_TRNS,
            KC_TRNS,    FR_UGRV,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS,          KC_TRNS,     KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS
            ),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode)
    {
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
            const uint16_t keycode = keymap_key_to_keycode(L_BASE, record->event.key);
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
                const uint16_t keycode = keymap_key_to_keycode(L_BASE, record->event.key);
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                register_code(keycode);
                unregister_code(keycode);
            }
            return false;
        }
#endif
    default:
        {
            break;
        }
    }
    return true;
}

void persistent_default_layer_set(uint16_t default_layer)
{
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

