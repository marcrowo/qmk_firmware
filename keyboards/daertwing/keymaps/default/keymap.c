// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    //  c0       c1       c2       c3       c4                 c5       c6       c7       c8       c9
    [0] = LAYOUT(
        KC_Q,    KC_L,    KC_D,    KC_Z,    KC_K,                KC_P,    KC_W,    KC_O,    KC_U,    KC_COMM,
         KC_R,    KC_N,    KC_T,    KC_H,    KC_F,              KC_Y,    KC_C,    KC_E,    KC_I,    KC_A,
        KC_X,    KC_J,    KC_M,    KC_B,    KC_V,                KC_QUOT, KC_G,    KC_SCLN, KC_SLSH, KC_DOT,
                                    KC_TAB,  KC_S,   KC_MUTE,   KC_SPC,  KC_QUOT
    )
};
