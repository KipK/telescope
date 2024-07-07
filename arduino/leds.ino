#include <Arduino.h>
#include "define.h"

uint8_t led_pwm_cur = 0; // current pwm value
uint8_t led_pwm_start = 0; // starting pwm value
uint8_t led_pwm_sp = 0;  // set point pwm value
uint32_t led_trans_t = 0; // transition duration for current task
bool led_is_trans = false; // are we in transition phase
uint32_t led_timer = 0;

void setupLeds() {
	pinMode(ledPin, OUTPUT);  // sets the led pin as output	
	digitalWrite(ledPin, 0);
}

void ledController() {
	if (led_is_trans) {
		if (!led_timer) {
			led_timer = millis();
		}
		
		if (millis() - led_timer <= led_trans_t) {
			//transition
			if (!led_pwm_start) {
				led_pwm_start = led_pwm_cur;
			}
			led_pwm_cur = map(led_pwm_cur, millis() - led_timer, led_trans_t, led_pwm_start, led_pwm_sp);
		}
		else {
			// transition end, reinitialise vars
			led_timer = 0;
			led_pwm_start = 0;
			led_is_trans = false;
			led_pwm_cur = led_pwm_sp;
		}
	}
	digitalWrite(ledPin,led_pwm_cur);
}

void setLeds(uint8_t pwm, uint32_t trans_t) {
	led_pwm_sp = pwm;
	led_trans_t = trans_t;
	led_is_trans = true;
}