/* Copyright 2021 rjboone
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
#include QMK_KEYBOARD_H

#include "oneshot.h"

enum layers{
  BASE,
  SYM,
  NAV,
  NUM,
  GAME
};

enum keycodes {
  // Custom oneshot mod implementation with no timers.
  OS_SHFT = SAFE_RANGE,
  OS_CTRL,
  OS_ALT,
  OS_GUI,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
   KC_1, KC_2, KC_3, KC_4, KC_5,
   KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,
   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN,
   KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
   KC_NO, KC_NO, MO(NAV), KC_SPC, KC_LSFT, MO(SYM), TG(GAME), KC_SLSH
  ),

  [SYM] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL,  KC_ASTR,
    KC_QUOT, KC_DQUO, KC_LCBR, KC_LPRN, KC_LBRC, KC_NO,   OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
    KC_CIRC, KC_AMPR, KC_RCBR, KC_RPRN, KC_RBRC, KC_NO,   KC_GRV,  KC_TILD, KC_BSLS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PIPE
  ),

  [NAV] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_ESC,  KC_VOLD, KC_MUTE, KC_VOLU, KC_NO, RESET,   KC_HOME,  KC_UP,   KC_END,  KC_BSPC,
    OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_NO, KC_CAPS, KC_LEFT,  KC_DOWN, KC_RGHT, KC_ENTER,
    KC_TAB,  KC_MPRV, KC_MPLY, KC_MNXT, KC_NO, KC_NO,   KC_PGDN,  KC_NO,   KC_PGUP,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL
  ),

  [NUM] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,
    OS_SHFT, OS_CTRL, OS_ALT, OS_GUI, KC_F11, KC_F12, OS_GUI, OS_ALT, OS_CTRL, OS_SHFT,
    KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_F10
  ),

  [GAME] = LAYOUT(
    KC_1, KC_2, KC_3, KC_4, KC_5,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_6, KC_UP, KC_NO, KC_P,
    KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_LEFT, KC_DOWN, KC_RIGHT, KC_NO,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_NO, KC_NO, KC_NO,
    KC_LCTL, KC_NO, KC_LALT, KC_SPC, KC_TILD, KC_NO, TG(GAME), KC_NO
  ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case MO(SYM):
    case MO(NAV):
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case MO(SYM):
    case MO(NAV):
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, SYM, NAV, NUM);
}
