#pragma once

#include QMK_KEYBOARD_H

typedef enum {
    os_up_unqueued,
    os_up_queued,
    os_down_unused,
    os_down_used,
} oneshot_state;

// Custom oneshot mod implementation with no timers. If a mod is used while
// held it will be unregistered on keyup as normal, otherwise it will be queued
// and only released after the next non-mod keyup.
void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// Defines keys to ignore when determining whether a oneshot mod has been used.
// Setting this to modifiers and layer change keys allows stacking multiple
// oneshot modifiers and carrying them between layers.
bool is_oneshot_ignored_key(uint16_t keycode);
