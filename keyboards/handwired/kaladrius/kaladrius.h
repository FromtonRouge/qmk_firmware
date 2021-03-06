/* Copyright 2018 FromtonRouge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

typedef const uint32_t steno_layout_t[MATRIX_COLS];

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
        K00, K01, K02, K03, K04, K05, K06,       K09, K0A, K0B, K0C, K0D, K0E, K0F, \
        K10, K11, K12, K13, K14, K15, K16,       K19, K1A, K1B, K1C, K1D, K1E, K1F, \
        K20, K21, K22, K23, K24, K25, K26,       K29, K2A, K2B, K2C, K2D, K2E, K2F, \
        K30, K31, K32, K33, K34, K35,                 K3A, K3B, K3C, K3D, K3E, K3F, \
        K40, K41, K42, K43,                                     K4C, K4D, K4E, K4F, \
                                      K36,        K39, \
                            K44, K45, K46,        K49, K4A, K4B, \
                                      K47,        K48 \
) \
{ \
    { K00, K01, K02, K03, K04, K05, K06, KC_NO, KC_NO, K09, K0A, K0B, K0C, K0D, K0E, K0F }, \
    { K10, K11, K12, K13, K14, K15, K16, KC_NO, KC_NO, K19, K1A, K1B, K1C, K1D, K1E, K1F }, \
    { K20, K21, K22, K23, K24, K25, K26, KC_NO, KC_NO, K29, K2A, K2B, K2C, K2D, K2E, K2F }, \
    { K30, K31, K32, K33, K34, K35, K36, KC_NO, KC_NO, K39, K3A, K3B, K3C, K3D, K3E, K3F }, \
    { K40, K41, K42, K43, K44, K45, K46, K47,   K48,   K49, K4A, K4B, K4C, K4D, K4E, K4F }, \
}

extern void set_leds(bool red, bool green, bool blue);
extern void blink_leds(void);
