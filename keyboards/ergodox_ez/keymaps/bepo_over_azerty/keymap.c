#include QMK_KEYBOARD_H
#include "version.h"
#include "quantum/keymap_extras/keymap_french.h"
#include "quantum/keymap_extras/keymap_bepo.h"

enum bepo_custom_keycodes
{
    CKC_LEFT_GUILLEMET = SAFE_RANGE,
    CKC_RIGHT_GUILLEMET,
    CKC_E_CIRC,
};

#define L_BEPO_AZERTY 0 // Calque par défaut BEPO à utiliser sur un PC en AZERTY
#define L_BEPO_DRIVER 1 // Calque BEPO si les drivers BEPO sont installés
#define L_FUNCTION 2    // Calque pour F1, F2 etc...

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
[L_BEPO_AZERTY] = LAYOUT_ergodox(
  // Main gauche
  KC_ESC,  FR_QUOT,    CKC_LEFT_GUILLEMET, CKC_RIGHT_GUILLEMET, FR_LPRN, FR_RPRN, FR_DLR,
  FR_W,    KC_B,       FR_EACU,            KC_P,                KC_O,    FR_EGRV, KC_NO,
  FR_CCED, FR_A,       KC_U,               KC_I,                KC_E,    FR_COMM,
  KC_LSFT, FR_AGRV,    KC_Y,               KC_X,                FR_DOT,  KC_K,    KC_NO,
  KC_LCTL, CKC_E_CIRC, KC_LALT,            MO(L_FUNCTION),      KC_ENT,

                                                                 MO(L_FUNCTION),  TG(L_FUNCTION),
                                                                                  KC_NO,
                                                       KC_BSPC,  KC_NO,           KC_LGUI,
  // Main droite
  FR_PERC,            FR_AT,   FR_PLUS, FR_MINS,        FR_SLSH,   FR_ASTR, FR_EQL,
  TG(L_BEPO_DRIVER),  FR_CIRC, KC_V,    KC_D,           KC_L,      KC_J,    FR_Z,
                      KC_C,    KC_T,    KC_S,           KC_R,      KC_N,    FR_M,
  KC_NO,              FR_APOS, FR_Q,    KC_G,           KC_H,      KC_F,    KC_RSFT,
                               KC_SPC,  MO(L_FUNCTION), KC_RALT,   KC_APP,  KC_RCTL,

  TG(L_FUNCTION), MO(L_FUNCTION),
  KC_NO,
  KC_NO, KC_NO, KC_DEL
),

[L_BEPO_DRIVER] = LAYOUT_ergodox(
  // Main gauche
  KC_ESC,       BP_DOUBLE_QUOTE, BP_LEFT_GUILLEMET, BP_RIGHT_GUILLEMET, BP_LEFT_PAREN, BP_RIGHT_PAREN, BP_DOLLAR,
  BP_W,         BP_B,            BP_E_ACUTE,        BP_P,               BP_O,          BP_E_GRAVE,     KC_TAB,
  BP_C_CEDILLA, BP_A,            BP_U,              BP_I,               BP_E,          BP_COMMA,
  KC_TRNS,      BP_A_GRAVE,      BP_Y,              BP_X,               BP_DOT,        BP_K,           KC_DELT,
  KC_TRNS,      BP_E_CIRCUMFLEX, KC_TRNS,           KC_TRNS,            KC_TRNS,

                                                                                              KC_TRNS, KC_TRNS,
                                                                                                       KC_TRNS,
                                                                                     KC_TRNS, KC_TRNS, KC_TRNS,

  // Main droite
  BP_PERCENT, BP_AT,              BP_PLUS, BP_MINUS, BP_SLASH, BP_ASTERISK, BP_EQUAL,
  KC_TRNS,    BP_DEAD_CIRCUMFLEX, BP_V,    BP_D,     BP_L,     BP_J,        BP_Z,
              BP_C,               BP_T,    BP_S,     BP_R,     BP_N,        BP_M,
  KC_TRNS,    BP_APOSTROPHE,      BP_Q,    BP_G,     BP_H,     BP_F,        KC_TRNS,
                                  KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_TRNS,

  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS
),

// F1-F12 Layer
[L_FUNCTION] = LAYOUT_ergodox(
  // Main gauche
  KC_F1,          KC_F2,      KC_F3,      KC_F4,      KC_F5,          KC_F6,          RESET,
  KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
  KC_TRNS,        KC_HOME,    KC_PGUP,    KC_PGDN,    KC_END,         KC_TRNS,
  KC_LSFT,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,
  KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                                  KC_TRNS,    KC_TRNS,
                                                                                              KC_TRNS,
                                                                      KC_TRNS,    KC_TRNS,    KC_TRNS,
  // Main droite
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

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    if (record->event.pressed)
    {
        const uint8_t layer = biton32(layer_state);
        if (layer == L_BEPO_AZERTY)
        {
            const uint8_t mods = get_mods();
            const bool has_shift = mods & (1 << 1) || mods & (1 << 5);  // Left Shift or Right Shift
            const bool has_alt_gr = mods & (1 << 6);
            switch (keycode)
            {
            case FR_QUOT:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_1);
                        unregister_code(KC_1);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case CKC_LEFT_GUILLEMET:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_2);
                        unregister_code(KC_2);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(KC_NONUS_BSLASH);
                        unregister_code(KC_NONUS_BSLASH);
                        set_mods(mods);
                        return false;
                    }
                    else
                    {
                        register_code(FR_QUOT);
                        unregister_code(FR_QUOT);
                        return false;
                    }
                    break;
                }
            case CKC_RIGHT_GUILLEMET:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_3);
                        unregister_code(KC_3);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        add_mods(MOD_LSFT);
                        register_code(KC_NONUS_BSLASH);
                        unregister_code(KC_NONUS_BSLASH);
                        set_mods(mods);
                        return false;
                    }
                    else
                    {
                        register_code(FR_QUOT);
                        unregister_code(FR_QUOT);
                        return false;
                    }
                    break;
                }
            case FR_LPRN:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_4);
                        unregister_code(KC_4);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_LBRC);
                        unregister_code16(FR_LBRC);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_RPRN:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_5);
                        unregister_code(KC_5);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_RBRC);
                        unregister_code16(FR_RBRC);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_DLR:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code16(FR_HASH);
                        unregister_code16(FR_HASH);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_MINS);
                        unregister_code(FR_MINS);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_AT:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_6);
                        unregister_code(KC_6);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_CCIRC);
                        unregister_code16(FR_CCIRC);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_PLUS:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_7);
                        unregister_code(KC_7);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_MINS:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_8);
                        unregister_code(KC_8);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_SLSH:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_9);
                        unregister_code(KC_9);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_ASTR:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_0);
                        unregister_code(KC_0);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_EQL:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(FR_RPRN);
                        unregister_code(FR_RPRN);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_PERC:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_QUOT);
                        unregister_code16(FR_QUOT);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code16(FR_GRV);
                        unregister_code16(FR_GRV);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case KC_B:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_UNDS);
                        unregister_code(FR_UNDS);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(FR_MINS);
                        unregister_code(FR_MINS);
                        return false;
                    }
                    break;
                }
            case FR_EACU:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code16(KC_E);
                        unregister_code16(KC_E);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case KC_P:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_AMP);
                        unregister_code(FR_AMP);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_O:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        add_mods(MOD_LSFT);
                        register_code(KC_O);
                        unregister_code(KC_O);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(KC_O);
                        unregister_code(KC_O);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_EGRV:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_GRV);
                        unregister_code16(FR_GRV);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code16(KC_E);
                        unregister_code16(KC_E);
                        return false;
                    }
                    break;
                }
            case FR_CIRC:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code(FR_EXLM);
                        unregister_code(FR_EXLM);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case KC_L:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_PND);
                        unregister_code16(FR_PND);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_CCED:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(KC_C);
                        unregister_code(KC_C);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case FR_A:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        add_mods(MOD_LSFT);
                        register_code(FR_A);
                        unregister_code(FR_A);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_A);
                        unregister_code(FR_A);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_U:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        add_mods(MOD_LSFT);
                        register_code(KC_U);
                        unregister_code(KC_U);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_UGRV);
                        unregister_code(FR_UGRV);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_I:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_UMLT);
                        unregister_code16(FR_UMLT);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_E:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_BULT);
                        unregister_code16(FR_BULT);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case FR_COMM:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code(FR_SCLN);
                        unregister_code(FR_SCLN);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_APOS);
                        unregister_code(FR_APOS);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_N:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(FR_EACU);
                        unregister_code(FR_EACU);
                        return false;
                    }
                    break;
                }
            case CKC_E_CIRC:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_CCIRC);
                        unregister_code16(FR_CCIRC);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code(FR_CIRC);
                        unregister_code(FR_CIRC);
                        add_mods(MOD_LSFT);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_SLSH);
                        unregister_code16(FR_SLSH);
                        set_mods(mods);
                        return false;
                    }
                    else
                    {
                        register_code(FR_CIRC);
                        unregister_code(FR_CIRC);
                        register_code(KC_E);
                        unregister_code(KC_E);
                        return false;
                    }
                    break;
                }
            case FR_AGRV:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(FR_A);
                        unregister_code(FR_A);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(KC_8);
                        unregister_code(KC_8);
                        return false;
                    }
                    break;
                }
            case KC_Y:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_APOS);
                        unregister_code(FR_APOS);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(FR_APOS);
                        unregister_code(FR_APOS);
                        return false;
                    }
                    break;
                }
            case KC_X:
                {
                    if (has_shift && has_alt_gr)
                    {
                        clear_mods();
                        register_code(FR_APOS);
                        unregister_code(FR_APOS);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(FR_EQL);
                        unregister_code(FR_EQL);
                        return false;
                    }
                    break;
                }
            case FR_DOT:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        clear_mods();
                        register_code(FR_COLN);
                        unregister_code(FR_COLN);
                        set_mods(mods);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_DOT);
                        unregister_code16(FR_DOT);
                        register_code16(FR_DOT);
                        unregister_code16(FR_DOT);
                        register_code16(FR_DOT);
                        unregister_code16(FR_DOT);
                        set_mods(mods);
                        return false;
                    }
                    break;
                }
            case KC_K:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        register_code(FR_EACU);
                        unregister_code(FR_EACU);
                        return false;
                    }
                    break;
                }
            case FR_APOS:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_shift)
                    {
                        register_code(FR_COMM);
                        unregister_code(FR_COMM);
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        return false;
                    }
                    break;
                }
            case KC_G:
                {
                    if (has_shift && has_alt_gr)
                    {
                        return false;
                    }
                    else if (has_alt_gr)
                    {
                        clear_mods();
                        register_code16(FR_MU);
                        unregister_code16(FR_MU);
                        set_mods(mods);
                        return false;
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
    return true;
}

// Fonction appelée lors d'un changement de calque
layer_state_t layer_state_set_user(layer_state_t state)
{
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = biton32(state);
    switch (layer)
    {
    case L_BEPO_AZERTY:
        {
            break;
        }
    case L_BEPO_DRIVER:
        {
            ergodox_right_led_2_on();
            break;
        }
    case L_FUNCTION:
        {
            ergodox_right_led_1_on();
            break;
        }
    default:
        {
            break;
        }
    }
    return state;
}
