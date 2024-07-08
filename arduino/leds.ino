#include <Arduino.h>
#include "define.h"

uint8_t led_pwm_cur = 0; // current pwm value
uint8_t led_pwm_start = 0; // starting pwm value
uint8_t led_pwm_sp = 0;  // set point pwm value
uint32_t led_trans_t = 0; // transition duration for current task
bool led_is_trans = false; // are we in transition phase
uint32_t led_timer = 0;

void setupLeds() {
	pinMode(LEDPIN, OUTPUT);  // sets the led pin as output	
	analogWrite(LEDPIN, 0);
}

void ledController() {
	if (led_is_trans) {
		led_timer += 20;
		if (led_timer <= led_trans_t) {
			//transition
			if (!led_pwm_start) {
				led_pwm_start = led_pwm_cur;
			}
			led_pwm_cur = map(led_timer, 0, led_trans_t, led_pwm_start, led_pwm_sp);
		}
		else {
			// transition end, reinitialise vars
#ifdef DEBUG
			Serial.println("end led transition");
#endif
			led_timer = 0;
			led_pwm_start = 0;
			led_is_trans = false;
			led_pwm_cur = led_pwm_sp;
		}
		
	}
	analogWrite(LEDPIN,led_pwm_cur);

}

void setLeds(uint8_t pwm, uint32_t trans_t) {
    // pwm 0-254 , trans_t in ms
	led_pwm_sp = pwm;
	led_trans_t = trans_t;
	led_is_trans = true;
}