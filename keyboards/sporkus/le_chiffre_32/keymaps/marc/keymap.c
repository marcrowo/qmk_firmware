// Copyright 2020 tominabox1
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#include "oneshot.h"

enum layers {
    BASE,
    SYM,
    NAV,
    NUM,
};

enum keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
};

// KC_F13 on the center key — bind your voice transcription app to F13 once
// and it will fire from any keyboard that has F13 mapped.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_F13,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,            KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
        MO(NAV), KC_SPC,  KC_LSFT, MO(SYM)
    ),
    [SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_TRNS, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL,  KC_ASTR,
        KC_QUOT, KC_DQUO, KC_LCBR, KC_LPRN, KC_LBRC,          KC_NO,   OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_CIRC, KC_AMPR, KC_RCBR, KC_RPRN, KC_RBRC,          KC_NO,   KC_GRV,  KC_TILD, KC_BSLS, KC_PIPE,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [NAV] = LAYOUT(
        KC_ESC,  KC_VOLD, KC_MUTE, KC_VOLU, KC_NO,   KC_TRNS, QK_BOOT, KC_HOME, KC_UP,   KC_END,  KC_BSPC,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_NO,            KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_ENTER,
        KC_TAB,  KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,            KC_NO,   KC_PGDN, KC_NO,   KC_PGUP, KC_DEL,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [NUM] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_F11,           KC_F12,  OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [SYM]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [NAV]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [NUM]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

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
oneshot_state os_alt_state  = os_up_unqueued;
oneshot_state os_gui_state  = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state,  KC_LALT, OS_ALT,  keycode, record);
    update_oneshot(&os_gui_state,  KC_LGUI, OS_GUI,  keycode, record);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
