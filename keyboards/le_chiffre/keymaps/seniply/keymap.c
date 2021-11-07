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

enum layers {
  BASE,
  EXTEND,
  FUNCTION,
  SYMBOL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
    KC_Q, KC_W, KC_F,  KC_P,    KC_B,   KC_MPLY, KC_J,   KC_L,      KC_U,    KC_Y,   KC_QUOT,
    KC_A, KC_R, KC_S,  KC_T,    KC_G,            KC_M,   KC_N,      KC_E,    KC_I,   KC_O,
    KC_Z, KC_X, KC_C,  KC_D,    KC_V,            KC_K,   KC_H,      KC_COMM, KC_DOT, KC_SLSH,
                MO(EXTEND),     KC_LSFT,         KC_SPC, MO(SYMBOL)
  ),
  [EXTEND] = LAYOUT(
    KC_ESC,        KC_MUTE,       KC_VOLD,       KC_VOLU,       KC_NO,         KC_TRNS, KC_PGUP, KC_HOME, KC_UP,         KC_END,        KC_CAPS,
    OSM(MOD_LALT), OSM(MOD_LGUI), OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_RALT),          KC_PGDN, KC_LEFT, KC_DOWN,       KC_RGHT,       KC_DEL,
    KC_TAB,        KC_MRWD,       KC_MPLY,       KC_MFFD,       KC_NO,                  KC_ENT,  KC_BSPC, OSM(MOD_RSFT), OSM(MOD_RCTL), KC_NO,
                                  KC_TRNS,       KC_TRNS,                               KC_TRNS, KC_TRNS
  ),
  [FUNCTION] = LAYOUT(
    KC_TILD,       KC_GRV,        KC_BSLS,       KC_PIPE,       KC_NO,         KC_TRNS, KC_F12,  KC_F7,  KC_F8, KC_F9, KC_NO,
    OSM(MOD_LALT), OSM(MOD_LGUI), OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_RALT),          KC_F11,  KC_F4,  KC_F5, KC_F6, KC_NO,
    RESET,         KC_NO,         KC_NO,         KC_NO,         KC_NO,                  KC_F10,  KC_F1,  KC_F2, KC_F3, KC_NO,
                                  KC_TRNS,       KC_TRNS,                               KC_TRNS, KC_TRNS
  ),
  [SYMBOL] = LAYOUT(
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_TRNS, KC_EQL,  KC_7,   KC_8, KC_9, KC_PLUS,
    KC_SCLN, KC_COLN, KC_LCBR, KC_LPRN, KC_LBRC,          KC_8,    KC_4,   KC_5, KC_6, KC_MINS,
    KC_CIRC, KC_AMPR, KC_RCBR, KC_RPRN, KC_RBRC,          KC_0,    KC_1,   KC_2, KC_3, KC_SLSH,
                      KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, EXTEND, SYMBOL, FUNCTION);
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
    case EXTEND:
      oled_write_ln_P(PSTR("EXTEND"), false);
      break;
    case FUNCTION:
      oled_write_ln_P(PSTR("FUNCTION"), false);
      break;
    case SYMBOL:
      oled_write_ln_P(PSTR("SYMBOL"), false);
      break;
    default:
      oled_write_ln_P(PSTR("UNKNOWN"), false);
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
