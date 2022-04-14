/* Copyright 2020 tominabox1
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

enum layers {
  BASE,
  SYM,
  NAV,
  NUM,
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
    KC_Q, KC_W, KC_E,  KC_R,  KC_T, KC_NO, KC_Y, KC_U, KC_I,    KC_O,   KC_P,
    KC_A, KC_S, KC_D,  KC_F,  KC_G,        KC_H, KC_J, KC_K,    KC_L,   KC_SCLN,
    KC_Z, KC_X, KC_C,  KC_V,  KC_B,        KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
    // THUMB ROW
    MO(NAV), KC_SPC, KC_LSFT, MO(SYM)
  ),
  [SYM] = LAYOUT(
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_TRNS, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL,  KC_ASTR,
    KC_QUOT, KC_DQUO, KC_LCBR, KC_LPRN, KC_LBRC,          KC_NO,   OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
    KC_CIRC, KC_AMPR, KC_RCBR, KC_RPRN, KC_RBRC,          KC_NO,   KC_GRV,  KC_TILD, KC_BSLS, KC_PIPE,
    // THUMB ROW
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [NAV] = LAYOUT(
    KC_ESC,  KC_VOLD, KC_MUTE, KC_VOLU, KC_NO, KC_TRNS, RESET,  KC_HOME,  KC_UP,   KC_END,  KC_BSPC,
    OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_NO,          KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_ENTER,
    KC_TAB,  KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,          KC_NO,   KC_PGDN, KC_NO,   KC_PGUP, KC_DEL,
    // THUMB ROW
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [NUM] = LAYOUT(
    KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_TRNS, KC_6,   KC_7,   KC_8,   KC_9,    KC_0,
    OS_SHFT, OS_CTRL, OS_ALT, OS_GUI, KC_F11,          KC_F12, OS_GUI, OS_ALT, OS_CTRL, OS_SHFT,
    KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,           KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,
    // THUMB ROW
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
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

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
  }
  return true;
}

#ifdef OLED_ENABLE  // Special thanks to Sickbabies for this great OLED widget!
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_90;  // rotates for proper orientation
}

void render_lechiffre_logo(void) {
  static const char PROGMEM lechiffre_logo[] = {// 'lechiffre_logo', 32x20px
    0x00, 0x3e, 0x20, 0x20, 0x00, 0x18, 0x2c, 0xa8, 0x80, 0x00, 0x1c, 0x22, 0x22, 0x00, 0x3e, 0x08, 0x30, 0x00, 0x34, 0x00, 0x3c, 0x0a, 0x00, 0xbc, 0x8a, 0x00, 0x38, 0x08, 0x00, 0x18, 0x2c, 0x28, 0x00, 0xb6, 0xb6, 0x00, 0xdb, 0xdb, 0x00, 0x6d, 0x6d, 0x00, 0xdb, 0xdb, 0x00, 0xdb, 0xdb, 0x00, 0x00, 0xdb, 0xdb, 0x00, 0xdb, 0xdb, 0x00, 0x6d, 0x6d, 0x00, 0xdb, 0xdb, 0x00, 0xb6, 0xb6, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00};

  oled_write_raw_P(lechiffre_logo, sizeof(lechiffre_logo));
}

static void render_layer_status(void) {
  oled_write_P(PSTR("-----"), false);
  switch (get_highest_layer(layer_state)) {
    case BASE:
      oled_write_ln_P(PSTR("BASE"), false);
      break;
    case SYM:
      oled_write_ln_P(PSTR("SYM"), false);
      break;
    case NAV:
      oled_write_ln_P(PSTR("NAV"), false);
      break;
    case NUM:
      oled_write_ln_P(PSTR("NUM"), false);
      break;
    default:
      oled_write_ln_P(PSTR("?"), false);
  }
}

void render_keylock_status(led_t led_state) {
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("C"), led_state.caps_lock);
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("N"), led_state.num_lock);
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("S"), led_state.scroll_lock);
}

void render_mod_status(uint8_t modifiers) {
  oled_write_P(PSTR("-----"), false);
  oled_write_ln_P(PSTR("SHFT"), (modifiers & MOD_MASK_SHIFT));
  oled_write_ln_P(PSTR("CTRL"), (modifiers & MOD_MASK_CTRL));
  oled_write_ln_P(PSTR("ALT"), (modifiers & MOD_MASK_ALT));
  oled_write_ln_P(PSTR("GUI"), (modifiers & MOD_MASK_GUI));
}

void oled_task_user(void) {
  render_lechiffre_logo();
  oled_set_cursor(0, 3);
  render_layer_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
  render_mod_status(get_mods() | get_oneshot_mods());
  render_keylock_status(host_keyboard_led_state());
}

#endif
