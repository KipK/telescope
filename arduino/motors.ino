#include <Arduino.h>
#include "define.h"

uint8_t motor_pwm_cur; 
uint8_t motor_pwm_start = 0; // starting pwm value
uint8_t motor_pwm_sp = 0;  // set point pwm value
uint32_t motor_trans_t = 0; // transition duration for current task
bool motor_is_trans = false; // are we in transition phase
uint32_t motor_timer = 0;

void setupMotors() {
	pinMode(MOTORPWMPIN, OUTPUT);  // sets the led pin as output	
	analogWrite(MOTORPWMPIN, 0);
	pinMode(MOTORDIRECTIONPIN, OUTPUT);  // sets the led pin as output	
	digitalWrite(MOTORDIRECTIONPIN, HIGH); // default to forward
}

void motorController() {
	// calculer pwm moteur
	if (motor_is_trans) {
		motor_timer += 20;
		if (motor_timer <= motor_trans_t) {
			//transition
			if (!motor_pwm_start) {
				motor_pwm_start = motor_pwm_cur;
			}
			motor_pwm_cur = map(motor_timer, 0, motor_trans_t, motor_pwm_start, motor_pwm_sp);
		}
		else {
			// transition end, reinitialise vars
#ifdef DEBUG
			Serial.println("end motor transition");
#endif
			motor_timer = 0;
			motor_pwm_start = 0;
			motor_is_trans = false;
			motor_pwm_cur = motor_pwm_sp;
		}
		
	}
	// set direction
	if (motor_pwm_sp >= 0) {
		digitalWrite(MOTORDIRECTIONPIN, HIGH);
	}
	else {
		digitalWrite(MOTORDIRECTIONPIN, LOW);
	}
	analogWrite(LEDPIN,abs(motor_pwm_cur)*2);

}

void setMotors(int8_t pwm, uint32_t trans_t) { // pwm, direction 0: backward 1: forward, transition time 
    // pwm 0-254 , trans_t in ms
	motor_pwm_sp = pwm;
	motor_trans_t = trans_t;
	motor_is_trans = true;
#ifdef DEBUG
	Serial.print("set motor pwm to ");
	Serial.println(motor_pwm_sp);
#endif
}

void stopMotors() {
	analogWrite(MOTORPWMPIN,0);
	digitalWrite(MOTORDIRECTIONPIN,HIGH);
	motor_pwm_cur = 0;
}