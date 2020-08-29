/* Copyright 2018 Jack Humbert
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
#include <unistd.h>

#define _MA 0
#define _FN 1
int timer = 0;
char wpm_text[5];
int x = 31;
int currwpm = 0;

//USER CONFIG PARAMS
float max_wpm = 110.0f; //WPM value at the top of the graph window
int graph_refresh_interval = 80; //in milliseconds
int graph_area_fill_interval = 3; //determines how dense the lines under the graph line are; lower = more dense

enum custom_keycodes {
  KC_CUST = SAFE_RANGE,
};

#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

void oled_task_user(void) {
	
	//get current WPM value
	currwpm = get_current_wpm();
	
	//check if it's been long enough before refreshing graph
	if(timer_elapsed(timer) > graph_refresh_interval){
	
		// main calculation to plot graph line
		x = 32 - ((currwpm / max_wpm) * 32);
		
		//first draw actual value line
		oled_write_pixel(1, x, true);
		
		//then fill in area below the value line
		for(int i = 32; i > x; i--){
			if(i % graph_area_fill_interval == 0){
				oled_write_pixel(1, i, true);
			}
		}
		
		//then move the entire graph one pixel to the right
		oled_pan(false); 
		
		//refresh the timer for the next iteration
		timer = timer_read();
		
	}
	
	//format current WPM value into a printable string
	sprintf(wpm_text,"%i", currwpm);
	
	//formatting for triple digit WPM vs double digits, then print WPM readout
	if(currwpm >= 100){
		oled_set_cursor(14, 3);
		oled_write("WPM: ", false);
		oled_set_cursor(18, 3);
		oled_write(wpm_text, false);
	} else if (currwpm >= 10){
		oled_set_cursor(15, 3);
		oled_write("WPM: ", false);
		oled_set_cursor(19, 3);
		oled_write(wpm_text, false);
	} else if (currwpm > 0) {
		oled_set_cursor(16, 3);
		oled_write("WPM: ", false);
		oled_set_cursor(20, 3);
		oled_write(wpm_text, false);		
	}
	
}

#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MA] = LAYOUT_ansi(
             KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
    KC_MUTE, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
    KC_MPLY, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    _______, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
    _______, KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    MO(_FN), KC_RALT, MO(_FN), KC_LEFT,          KC_DOWN, KC_RGHT
  ),
  [_FN] = LAYOUT_ansi(
               RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_HOME,  KC_INS,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, _______,                   _______,                   _______, _______, _______, _______,          _______, _______
  ),

};

/*bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);
  switch(keycode) {
    case KC_CUST: //custom macro
      if (record->event.pressed) {
      }
    break;

    case RM_1: //remote macro 1
      if (record->event.pressed) {
      }
    break;

    case RM_2: //remote macro 2
      if (record->event.pressed) {
      }
    break;

    case RM_3: //remote macro 3
      if (record->event.pressed) {
      }
    break;

    case RM_4: //remote macro 4
      if (record->event.pressed) {
      }
    break;
    
  }
return true;
}*/

void matrix_init_user(void) {
  matrix_init_remote_kb();
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}