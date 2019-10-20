#include QMK_KEYBOARD_H
#include "version.h"
#include "quantum/keymap_extras/keymap_french.h"
#include "quantum/keymap_extras/keymap_bepo.h"

#define L_BEPO_AZERTY 0 // Calque par défaut BEPO. L'OS doit être en AZERTY
#define L_BEPO_DRIVER 1 // Calque alternatif BEPO. L'OS doit être configuré en BEPO
#define L_FUNCTION 2    // Calque pour F1, F2 etc...

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
[L_BEPO_AZERTY] = LAYOUT_ergodox(
  // Main gauche
  KC_EQL,          KC_1,        KC_2,          KC_3,    KC_4,    KC_5,    KC_LGUI,
  KC_TAB,          KC_B,        FR_EACU,       KC_P,    KC_O,    FR_EGRV, KC_NO,
  KC_NO,           FR_A,        KC_U,          KC_I,    KC_E,    FR_COMM,
  KC_LSFT,         KC_E,        FR_A,          KC_Y,    KC_X,    FR_DOT,  KC_NO,
  KC_LCTL,         KC_QUOT,     KC_LALT,       MO(L_FUNCTION),   KC_ENT,

                                                                 MO(L_FUNCTION),  TG(L_FUNCTION),
                                                                                  KC_NO,
                                                       KC_BSPC,  KC_NO,           KC_NO,
  // Main droite
  KC_NO,              KC_6,    KC_7,    KC_8,    KC_9,      KC_0,           KC_MINS,
  TG(L_BEPO_DRIVER),  KC_Y,    KC_U,    KC_I,    KC_O,      KC_P,           KC_BSLS,
                      KC_H,    KC_J,    KC_K,    KC_L,      KC_SCLN,        KC_QUOT,
  KC_NO,              KC_N,    KC_M,    KC_COMM, KC_DOT,    KC_SLSH,        KC_RSFT,
             KC_SPC,  MO(L_FUNCTION), KC_LBRC, KC_RBRC, KC_RCTL,

  TG(L_FUNCTION), MO(L_FUNCTION),
  KC_NO,
  KC_NO, KC_NO, KC_DEL
),

[L_BEPO_DRIVER] = LAYOUT_ergodox(
  // Main gauche
  KC_ESC,       BP_DOUBLE_QUOTE, BP_LEFT_GUILLEMET, BP_RIGHT_GUILLEMET, BP_LEFT_PAREN, BP_RIGHT_PAREN, BP_DOLLAR,
  BP_W,         BP_B,            BP_E_ACUTE,        BP_P,               BP_O,          BP_E_GRAVE,     KC_TAB,
  BP_C_CEDILLA, BP_A,            BP_U,              BP_I,               BP_E,          BP_COMMA,
  KC_LSFT,      BP_A_GRAVE,      BP_Y,              BP_X,               BP_DOT,        BP_K,           KC_DELT,
  KC_LCTL,      BP_E_CIRCUMFLEX, KC_LALT,           KC_TRNS,            KC_TRNS,

                                                                                              KC_TRNS, KC_TRNS,
                                                                                                       KC_TRNS,
                                                                                     KC_TRNS, KC_TRNS, KC_LGUI,

  // Main droite
  BP_PERCENT, BP_AT,              BP_PLUS, BP_MINUS, BP_SLASH, BP_ASTERISK, BP_EQUAL,
  KC_TRNS,    BP_DEAD_CIRCUMFLEX, BP_V,    BP_D,     BP_L,     BP_J,        BP_Z,
              BP_C,               BP_T,    BP_S,     BP_R,     BP_N,        BP_M,
  KC_TRNS,    BP_APOSTROPHE,      BP_Q,    BP_G,     BP_H,     BP_F,        KC_RSFT,
                                  KC_TRNS, KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_RCTL,

  KC_TRNS, KC_TRNS,
  KC_APP,
  KC_RALT, KC_TRNS, KC_TRNS
),

// F1-F12 Layer
[L_FUNCTION] = LAYOUT_ergodox(
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
