#include QMK_KEYBOARD_H
#include "version.h"

#define BASE 0 // default layer

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
[BASE] = LAYOUT_ergodox(
  // left hand
  KC_EQL,          KC_1,        KC_2,          KC_3,    KC_4,    KC_5,    KC_LEFT,
  KC_DEL,          KC_Q,        KC_W,          KC_E,    KC_R,    KC_T,    KC_NO,
  KC_BSPC,         KC_A,        KC_S,          KC_D,    KC_F,    KC_G,
  KC_LSFT,         CTL_T(KC_Z), KC_X,          KC_C,    KC_V,    KC_B,    ALL_T(KC_NO),
  KC_GRV,          KC_QUOT,     LALT(KC_LSFT), KC_LEFT, KC_RGHT,
                                                           ALT_T(KC_APP), KC_LGUI,
                                                                          KC_HOME,
                                                         KC_SPC, KC_BSPC, KC_END,
  // right hand
  KC_RGHT,      KC_6,    KC_7,    KC_8,    KC_9,              KC_0,           KC_MINS,
  KC_NO,     KC_Y,    KC_U,    KC_I,    KC_O,              KC_P,           KC_BSLS,
  KC_H,         KC_J,    KC_K,    KC_L,    KC_SCLN, GUI_T(KC_QUOT),
  MEH_T(KC_NO), KC_N,    KC_M,    KC_COMM, KC_DOT,            CTL_T(KC_SLSH), KC_RSFT,
  KC_UP,        KC_DOWN, KC_LBRC, KC_RBRC, KC_NO,
  KC_LALT, CTL_T(KC_ESC),
  KC_PGUP,
  KC_PGDN, KC_TAB, KC_ENT
),
};

void matrix_init_user(void)
{
}

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state)
{
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    uint8_t layer = biton32(state);
    switch (layer)
    {
    case BASE:
        {
            break;
        }
    default:
        {
            break;
        }
    }
    return state;
}
