// Copyright 2021-2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @mainpage Anarion Dunedain's QMK keymap
 *
 * This is my Quantum Mechanical Keyboard (QMK) keymap. It is based on Pascal
 * Getreuer's keyma
 *
 * This repo uses the Apache License 2.0 except where otherwise indicated. See
 * LICENSE.txt for details.
 *
 * For further documentation of this keymap's features, see
 * <https://getreuer.info/posts/keyboards>
 */

#include "config_anarion.h"

// Needed for navigation keys on NAV layer
typedef struct {
  uint16_t timer;
  bool hold_triggered;
  uint8_t tap_count;
  uint16_t last_tap_time;
  bool is_pressed;  // Track if key is currently held
} nav_key_state_t;

static nav_key_state_t nav_left_state = {0};
static nav_key_state_t nav_right_state = {0};
static nav_key_state_t nav_up_state = {0};
static nav_key_state_t nav_down_state = {0};

#define NAV_TAPPING_TERM 200
#define NAV_QUICK_TAP_TERM 200

enum layers {
  BASE,
  NAV,
  NUM,
  MOUSE,
  TAP,
};

enum custom_keycodes {
  ARROW = SAFE_RANGE,
  CLOSEAPP,
  RGBNEXT,
  RGBHUP,
  RGBHRND,
  RGBDEF1,
  RGBDEF2,
  NAV_LEFT,
  NAV_RIGHT,
  NAV_UP,
  NAV_DOWN,
};

enum keycode_aliases {
  // The "magic" key is the Alternate Repeat Key.
  MAGIC = QK_AREP,
  // F20 mutes the mic on my system.
  MUTEMIC = KC_F20,
  // Short aliases for home row mods and other tap-hold keys.
  HRM_A = LCTL_T(KC_A),
  HRM_S = LALT_T(KC_S),
  HRM_D = LGUI_T(KC_D),
  HRM_F = LSFT_T(KC_F),
  HRM_G = LT(NUM, KC_G),
  HRM_B = LT(MOUSE, KC_B),
  HRM_J = LSFT_T(KC_J),
  HRM_K = LGUI_T(KC_K),
  HRM_L = LALT_T(KC_L),
  HRM_SEMI = LCTL_T(KC_SCLN),
  TAB_RT = RCTL(KC_TAB),
  TAB_LT = LCTL(LSFT(KC_TAB)),
  SPC_NAV = LT(NAV, KC_SPC),
  ENT_SHFT = LSFT_T(KC_ENTER),
  NAV_BSPC = KC_BSPC,
  NAV_DEL = KC_DEL,
  SWAP = A(KC_TAB),
  SELECT_ALL = LCTL(KC_A),
  WIN_UNDO = LCTL(KC_Z),
  WIN_CUT = LCTL(KC_X),
  WIN_COPY = S(C(KC_C)),
  WIN_PASTE = S(C(KC_V)),
  BSPC_RALT = RALT_T(KC_BSPC)
};

static bool isMacOS = false;
#if defined(COMMUNITY_MODULE_SELECT_WORD_ENABLE) && \
    defined(SELECT_WORD_OS_DYNAMIC)
bool select_word_host_is_mac(void) { return isMacOS; }
#endif /* defined(COMMUNITY_MODULE_SELECT_WORD_ENABLE) && \
          defined(SELECT_WORD_OS_DYNAMIC) */
#ifdef OS_DETECTION_ENABLE
bool process_detected_host_os_user(os_variant_t os) {
  isMacOS = (os == OS_MACOS || os == OS_IOS);
  return true;
}
#endif /* OS_DETECTION_ENABLE */

#ifdef COMMUNITY_MODULE_PALETTEFX_ENABLE
enum palette_names {  // User-defined PaletteFx palettes.
  PALETTEFX_MECHA = PALETTEFX_USER_0,
  PALETTEFX_HERO = PALETTEFX_USER_1,
};
#endif  // COMMUNITY_MODULE_PALETTEFX_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_LR(  // Base layer.
                     KC_EQUAL , KC_1, KC_2, KC_3, KC_4, KC_5,
                     KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
                     KC_ESC, HRM_A  , HRM_S  , HRM_D  , HRM_F  , HRM_G   ,
                     MO(NAV), KC_Z  , KC_X   , KC_C   , KC_V  , HRM_B   ,
                                                               SPC_NAV, BSPC_RALT ,

                     KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
                     KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BACKSLASH, 
                     KC_H   , HRM_J  , HRM_K  , HRM_L  , HRM_SEMI  , KC_QUOT,
                     KC_N   , KC_M  , KC_COMMA  , KC_DOT  , KC_SLSH, MO(NAV) ,
                                                                KC_DEL , ENT_SHFT
                     ),

  [NAV] = LAYOUT_LR(  // Navigation layer.
                    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,
                    _______, TAB_LT , TAB_RT , SWAP, S(KC_TAB), XXXXXXX,
                    KC_GRV, _______, _______, _______, _______, CLOSEAPP,
                    _______, WIN_UNDO, WIN_CUT, WIN_COPY, WIN_PASTE, _______,
                                                                _______, _______,

                    KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
                    KC_INS, KC_HOME, NAV_UP  , _______ , KC_PGUP, _______,
                    CW_TOGG, NAV_LEFT, NAV_DOWN, NAV_RIGHT, NAV_BSPC , _______,
                    _______, KC_END, KC_LBRC, KC_RBRC , KC_PGDN, _______,
                                                                _______, KC_ENTER
                    ),

  [NUM] = LAYOUT_LR(  // Number layer.
                    _______, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______, _______, _______,
                    _______, _______, _______, _______, _______, _______,
                                                                _______, _______,

                    KC_NUM, KC_PPLS, KC_PEQL, KC_PAST, KC_PSLS, KC_PMNS,
                    _______, KC_P7   , KC_P8   , KC_P9   , _______, _______,
                    _______, KC_P4   , KC_P5   , KC_P6   , KC_ENTER, _______,
                    _______, KC_P1   , KC_P2   , KC_P3   , KC_PENT, KC_RSFT,
                                                                KC_BSPC , KC_P0
                    ),

  [MOUSE] = LAYOUT_LR(  // Mouse + others layer.
                      _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______,
                      _______, _______, _______, _______, _______, _______,
                                                                _______, _______,

                      SELLINE,   SELWBAK, SELWORD, RGBHRND, RGBNEXT, _______,
                      _______, MS_WHLU , MS_UP   , MS_WHLD   , _______, _______,
                      _______, MS_LEFT , MS_DOWN , MS_RGHT   , _______, _______,
                      _______, MS_WHLL , MS_BTN3 , MS_WHLR   , _______, _______,
                                                                MS_BTN2 , MS_BTN1
                      ),    

  [TAP] = LAYOUT_LR(  // Tap-only, no home-row mods.
                    KC_EQUAL , KC_1, KC_2, KC_3, KC_4, KC_5,
                    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,
                    KC_ESC, KC_A  , KC_S  , KC_D  , KC_F  , KC_G   ,
                    KC_LSFT, KC_Z  , KC_X   , KC_C   , KC_V  , KC_B   ,
                                                                KC_SPC, KC_BSPC ,

                    KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS,
                    KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BACKSLASH, 
                    KC_H   , KC_J  , KC_K  , KC_L  , KC_SCLN  , KC_QUOT,
                    KC_N   , KC_M  , KC_COMMA  , KC_DOT  , KC_SLSH, KC_RSFT,
                                                                KC_DEL , KC_ENTER
                    ),
};

// Custom handling of NAV keys on NAV layer
// Tap - move cursor, repeat key enabled
// Hold - move to beg/end of page/line
//
// NAV_UP:    Tap: Up, Hold: Ctrl+Home
// NAV_DOWN:  Tap: Down, Hold: Ctrl+End
// NAV_LEFT:  Tap: Left, Hold: Home
// NAV_RIGHT: Tap: Right, Hold: End
void process_nav_key(uint16_t tap_keycode, uint16_t hold_keycode, 
                     nav_key_state_t *state, keyrecord_t *record) {
    if (record->event.pressed) {
        state->timer = timer_read();
        state->hold_triggered = false;
        state->is_pressed = true;

        // Check if this is a quick repeat tap
        if (timer_elapsed(state->last_tap_time) < NAV_QUICK_TAP_TERM) {
            state->tap_count++;
        } else {
            state->tap_count = 1;
        }

        state->last_tap_time = timer_read();

        // Always send tap immediately (tap-preferred)
        if (state->tap_count >= 2) {
            // Double tap or more - register and hold for key repeat
            register_code16(tap_keycode);
        } else {
            tap_code16(tap_keycode);
        }
    } else {
        // Release
        state->is_pressed = false;
        if (state->tap_count >= 2) {
            // Was double-tapped, unregister to stop repeat
            unregister_code16(tap_keycode);
        }
        // Don't reset tap_count immediately to allow chaining
        state->timer = 0;
    }
}

void nav_key_check_hold(uint16_t hold_keycode, nav_key_state_t *state) {
    if (!state->hold_triggered && state->is_pressed && state->timer > 0 && 
        timer_elapsed(state->timer) >= NAV_TAPPING_TERM && 
        state->tap_count == 1) {
        // Hold timeout reached, trigger hold action immediately
        state->hold_triggered = true;
        tap_code16(hold_keycode);
    }
}

void matrix_scan_user(void) {
    nav_key_check_hold(KC_HOME, &nav_left_state);
    nav_key_check_hold(KC_END, &nav_right_state);
    nav_key_check_hold(C(KC_HOME), &nav_up_state);
    nav_key_check_hold(C(KC_END), &nav_down_state);
}

// A cheap pseudorandom generator.
static uint8_t myrand(void) {
  static uint16_t state = 1;
#ifdef __CHIBIOS__  // Use high-res timer on ChibiOS.
  state += (uint16_t)chVTGetSystemTimeX();
#else
  state += timer_read();
#endif
  state *= UINT16_C(36563);
  return state >> 8;
}

///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////
const uint16_t tap_only_combo[] PROGMEM = {KC_5, KC_6, COMBO_END};
// clang-format off
combo_t key_combos[] = {
  COMBO(tap_only_combo, TG(TAP)),      // 5 and 6 => toggle TAP only layer
};
// clang-format on

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_CUSTOM_SHIFT_KEYS_ENABLE
const custom_shift_key_t custom_shift_keys[] = {
    // {KC_DOT, KC_RBRC},
    // {KC_COMM, KC_LBRC},
    // {KC_MPLY, KC_MNXT},
};
#endif  // COMMUNITY_MODULE_CUSTOM_SHIFT_KEYS_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case ENT_SHFT:
    case SPC_NAV:
    case BSPC_RALT:
      return TAPPING_TERM - 35;
    default:
      return TAPPING_TERM;
  }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t* record) {
  // Force Hold immediately on the three keys, to be able to roll quickly
  // those mods and subsequent keys.
  switch (keycode) {
    case ENT_SHFT:
    case SPC_NAV:
    case BSPC_RALT:
      // Immediately select the hold action when another key is pressed.
      return true;
    default:
      // Do not select the hold action when another key is pressed.
      return false;
  }
}

// uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
//   // If you quickly hold a tap-hold key after tapping it, the tap action is
//   // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
//   // lead to missed triggers in fast typing. Here, returning 0 means we
//   // instead want to "force hold" and disable key repeating.
//   switch (keycode) {
//     case NAV_LEFT:
//     case NAV_RIGHT:
//     case NAV_UP:
//     case NAV_DOWN:
//       return QUICK_TAP_TERM;  // Enable key repeating.
//     default:
//       return TAPPING_TERM;  // Otherwise, force hold and disable key
//       repeating.
//   }
// }

bool get_permissive_hold(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case ENT_SHFT:
    case SPC_NAV:
    case BSPC_RALT:
      // Immediately select the hold action when another key is tapped.
      return true;
    default:
      // Do not select the hold action when another key is tapped.
      return false;
  }
}

#ifdef FLOW_TAP_TERM
// uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record,
//                            uint16_t prev_keycode) {
//   // Only apply Flow Tap when following a letter key, and not hotkeys.
//   if (get_tap_keycode(prev_keycode) <= KC_Z &&
//       (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) == 0) {
//     switch (keycode) {
//       case HRM_A:
//       case HRM_SEMI:
//         return FLOW_TAP_TERM;
//
//         // case HRM_G:
//         // case HRM_H:
//         // return FLOW_TAP_TERM - 25;
//     }
//   }
//
//   return 0;  // Disable Flow Tap otherwise.
// }
#endif  // FLOW_TAP_TERM

#ifdef CHORDAL_HOLD
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case ENT_SHFT:
    case SPC_NAV:
    case BSPC_RALT:
      return true;

    case HRM_A:
    case HRM_SEMI:
      if (other_keycode == KC_C || other_keycode == KC_D ||
          other_keycode == KC_V || other_keycode == KC_U ||
          other_keycode == KC_Z) {
        return true;
      }
      break;
  }
  return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif  // CHORDAL_HOLD

#ifdef COMMUNITY_MODULE_TAP_FLOW_ENABLE
// uint16_t get_tap_flow_term(uint16_t keycode, keyrecord_t* record,
//                            uint16_t prev_keycode) {
//   // Only apply Tap Flow when following a letter key.
//   if (get_tap_keycode(prev_keycode) <= KC_Z) {
//     switch (keycode) {
//       case HRM_A:
//       case HRM_SEMI:
//         return g_tap_flow_term;
//
//       case HRM_S:
//       case HRM_F:
//       case HRM_J:
//       case HRM_L:
//         return g_tap_flow_term - 25;
//     }
//   }
//
//   return 0;
// }
#endif  // COMMUNITY_MODULE_TAP_FLOW_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Autocorrect (https://docs.qmk.fm/features/autocorrect)
///////////////////////////////////////////////////////////////////////////////
#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str, char* typo,
                       char* correct) {
  for (uint8_t i = 0; i < backspaces; ++i) {
    tap_code(KC_BSPC);
  }
  send_string_with_delay_P(str, TAP_CODE_DELAY);
  return false;
}
#endif  // AUTOCORRECT_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Caps word (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////
#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT_LSHIFT);  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
    case KC_COLN:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}
#endif  // CAPS_WORD_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Sentence case (https://getreuer.info/posts/keyboards/sentence-case)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
char sentence_case_press_user(uint16_t keycode, keyrecord_t* record,
                              uint8_t mods) {
  if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
    const bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
      case KC_A ... KC_Z:
        return 'a';  // Letter key.

      case KC_DOT:  // Both . and Shift . (?) punctuate sentence endings.
      case KC_EXLM:
      case KC_QUES:
        return '.';

      case KC_COMM:
        return shifted ? '.' : '#';

      case KC_2 ... KC_0:  // 2 3 4 5 6 7 8 9 0
      case KC_AT ... KC_RPRN:  // @ # $ % ^ & * ( )
      case KC_MINS ... KC_SCLN:  // - = [ ] backslash ;
      case KC_UNDS ... KC_COLN:  // _ + { } | :
      case KC_GRV:
        return '#';  // Symbol key.

      case KC_SPC:
        return ' ';  // Space key.

      case KC_QUOT:
      case KC_DQUO:
        return '\'';  // Quote key.
    }
  }

  // Otherwise clear Sentence Case to initial state.
  sentence_case_clear();
  return '\0';
}
#endif  // COMMUNITY_MODULE_SENTENCE_CASE_ENABLE

///////////////////////////////////////////////////////////////////////////////
// RGB Matrix Lighting (https://docs.qmk.fm/features/rgb_matrix)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_PALETTEFX_ENABLE
static void lighting_set_palette(uint8_t palette) {
  if (lumino_get_value() == 0) {
    lumino_cycle_3_state();
  }
  rgb_matrix_enable_noeeprom();
  rgb_matrix_sethsv_noeeprom(RGB_MATRIX_HUE_STEP * palette, 255,
                             rgb_matrix_get_val());
}

static void lighting_preset(uint8_t effect, uint8_t palette) {
  lighting_set_palette(palette);
  rgb_matrix_mode_noeeprom(effect);
  rgb_matrix_set_speed_noeeprom(80);
}
#endif  // COMMUNITY_MODULE_PALETTEFX_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Debug logging
///////////////////////////////////////////////////////////////////////////////
#ifndef NO_DEBUG
#pragma message "dlog_record: enabled"

#ifdef KEYCODE_STRING_ENABLE
// KEYCODE_STRING_NAMES_USER(
//     KEYCODE_STRING_NAME(ARROW), KEYCODE_STRING_NAME(UPDIR),
//     KEYCODE_STRING_NAME(STDCC), KEYCODE_STRING_NAME(USRNAME),
//     KEYCODE_STRING_NAME(TMUXESC), KEYCODE_STRING_NAME(SRCHSEL),
//     KEYCODE_STRING_NAME(SELWORD), KEYCODE_STRING_NAME(SELWBAK),
//     KEYCODE_STRING_NAME(SELLINE), KEYCODE_STRING_NAME(RGBNEXT),
//     KEYCODE_STRING_NAME(RGBHUP), KEYCODE_STRING_NAME(RGBHRND),
//     KEYCODE_STRING_NAME(RGBDEF1), KEYCODE_STRING_NAME(RGBDEF2), );
#endif  // KEYCODE_STRING_ENABLE

static void dlog_record(uint16_t keycode, keyrecord_t* record) {
  if (!debug_enable) {
    return;
  }
  uint8_t layer = read_source_layers_cache(record->event.key);
  bool is_tap_hold = IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
  xprintf("L%-2u ", layer);  // Log the layer.
  if (IS_COMBOEVENT(record->event)) {  // Combos don't have a position.
    xprintf("combo   ");
  } else {  // Log the "(row,col)" position.
    xprintf("(%2u,%2u) ", record->event.key.row, record->event.key.col);
  }
  xprintf("%-4s %-7s %s\n",  // "(tap|hold) (press|release) <keycode>".
          is_tap_hold ? (record->tap.count ? "tap" : "hold") : "",
          record->event.pressed ? "press" : "release",
          get_keycode_string(keycode));
}
#else
#pragma message "dlog_record: disabled"
#define dlog_record(keycode, record)
#endif  // NO_DEBUG

///////////////////////////////////////////////////////////////////////////////
// Status LEDs
///////////////////////////////////////////////////////////////////////////////
#ifdef STATUS_LED_1
// LED 1 indicates when any layer above the NAV layer is active.
layer_state_t layer_state_set_user(layer_state_t state) {
  STATUS_LED_1(get_highest_layer(layer_state) > NAV);
  return state;
}
#endif  // STATUS_LED_1

#ifdef STATUS_LED_2
// LED 2 indicates when Sentence case is in primed to capitalize the next key.
void sentence_case_primed(bool primed) { STATUS_LED_2(primed); }
#endif  // STATUS_LED_2

#ifdef STATUS_LED_3
// LED 3 indicates when Caps word is active.
void caps_word_set_user(bool active) { STATUS_LED_3(active); }
#endif  // STATUS_LED_3

///////////////////////////////////////////////////////////////////////////////
// User macro callbacks (https://docs.qmk.fm/feature_macros)
///////////////////////////////////////////////////////////////////////////////

void keyboard_post_init_user(void) {
#ifdef COMMUNITY_MODULE_PALETTEFX_ENABLE
  lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_FLOW + (myrand() % 4), myrand());
#endif  // COMMUNITY_MODULE_PALETTEFX_ENABLE

  // Play MUSHROOM_SOUND two seconds after init, if defined and audio enabled.
#if defined(AUDIO_ENABLE) && defined(MUSHROOM_SOUND)
  uint32_t play_init_song_callback(uint32_t trigger_time, void* cb_arg) {
    static float init_song[][2] = SONG(MUSHROOM_SOUND);
    PLAY_SONG(init_song);
    return 0;
  }
  defer_exec(2000, play_init_song_callback, NULL);
#endif  // defined(AUDIO_ENABLE) && defined(MUSHROOM_SOUND)
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  dlog_record(keycode, record);

  // // Track whether the left home ring and index keys are held, ignoring
  // layer. static bool left_home_ring_held = false; static bool
  // left_home_index_held = false; if (record->event.key.row == LEFT_HOME_ROW)
  // {
  //   switch (record->event.key.col) {
  //     case LEFT_HOME_RING_COL:
  //       left_home_ring_held = record->event.pressed;
  //       break;
  //     case LEFT_HOME_INDEX_COL:
  //       left_home_index_held = record->event.pressed;
  //       break;
  //   }
  //
  //   // NAV stays on while layer locked or while either ring or index is
  //   held. if (!(is_layer_locked(NAV) || left_home_ring_held ||
  //         left_home_index_held)) {
  //     layer_off(NAV);
  //   }
  // }

  // Logic for Alt mod when using alt-tabbing keys.
  if (keycode == SPC_NAV && record->tap.count == 0 && !record->event.pressed) {
    unregister_mods(MOD_BIT_LALT);
  } else if (record->event.pressed &&
             (keycode == S(A(KC_TAB)) || keycode == A(KC_TAB))) {
    register_mods(MOD_BIT_LALT);
  }

  const uint8_t mods = get_mods();
  const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                            | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
  );
  const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;

  switch (keycode) {
      /*
       * close app
       * gui+q in MacOS
       * alt+f4 in Windows
       */
    case NAV_LEFT:
      process_nav_key(KC_LEFT, KC_HOME, &nav_left_state, record);
      return false;

    case NAV_RIGHT:
      process_nav_key(KC_RIGHT, KC_END, &nav_right_state, record);
      return false;

    case NAV_UP:
      process_nav_key(KC_UP, C(KC_HOME), &nav_up_state, record);
      return false;

    case NAV_DOWN:
      process_nav_key(KC_DOWN, C(KC_END), &nav_down_state, record);
      return false;

    case C(KC_A)... C(KC_Z):
      // convert ctrl-<key> shortcuts to gui-<key> on MacOS
      if (isMacOS && record->event.pressed) {
        keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
        tap_code16_delay(G(keycode), TAP_CODE_DELAY);
#ifdef REPEAT_KEY_ENABLE
        set_last_keycode(G(keycode));
#endif /* REPEAT_KEY_ENABLE */
        return false;
      }
      break;
  }

  if (record->event.pressed) {
    switch (keycode) {
#ifdef COMMUNITY_MODULE_PALETTEFX_ENABLE
      case RGBNEXT:
        if (shift_mods) {
          rgb_matrix_step_reverse_noeeprom();
        } else {
          rgb_matrix_step_noeeprom();
        }
        break;

      case RGBHUP:
        if (shift_mods) {
          rgb_matrix_decrease_hue_noeeprom();
        } else {
          rgb_matrix_increase_hue_noeeprom();
        }
        break;

      case RGBHRND:
        lighting_set_palette(myrand());
        break;

      case RGBDEF1:
        lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_FLOW, PALETTEFX_MECHA);
        break;

      case RGBDEF2:
        lighting_preset(RGB_MATRIX_CUSTOM_PALETTEFX_RIPPLE, PALETTEFX_ROSEGOLD);
        break;
#endif  // COMMUNITY_MODULE_PALETTEFX_ENABLE
    }
  }

  return true;
}
