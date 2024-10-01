#include <Arduino.h>
#include "define.h"


int motor_pwm_cur; 
int motor_pwm_start = 0; // starting pwm value
int motor_pwm_sp = 0;  // set point pwm value
uint32_t motor_trans_t = 0; // transition duration for current task
bool motor_is_trans = false; // are we in transition phase
uint32_t motor_timer = 0;

int motor2_pwm_cur; 
int motor2_pwm_start = 0; // starting pwm value
int motor2_pwm_sp = 0;  // set point pwm value
uint32_t motor2_trans_t = 0; // transition duration for current task
bool motor2_is_trans = false; // are we in transition phase
uint32_t motor2_timer = 0;

void setupMotors() {

	pinMode(MOTORPWMPIN, OUTPUT);  // sets the led pin as output	
	analogWrite(MOTORPWMPIN, 0);
	pinMode(MOTORDIRECTIONPIN, OUTPUT);  // sets the led pin as output	
	digitalWrite(MOTORDIRECTIONPIN, HIGH); // default to forward

	pinMode(MOTOR2PWMPIN, OUTPUT);  // sets the led pin as output	
	analogWrite(MOTOR2PWMPIN, 0);
	pinMode(MOTOR2DIRECTIONPIN, OUTPUT);  // sets the led pin as output	
	digitalWrite(MOTOR2DIRECTIONPIN, HIGH); // default to forward
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
	if (motor_pwm_cur >= 0) {
		digitalWrite(MOTORDIRECTIONPIN, HIGH);
	}
	else {
		digitalWrite(MOTORDIRECTIONPIN, LOW);
	}
	analogWrite(MOTORPWMPIN,abs(motor_pwm_cur));

}

void motor2Controller() {
	// calculer pwm moteur
	if (motor2_is_trans) {
		motor2_timer += 20;
		if (motor2_timer <= motor2_trans_t) {
			//transition
			if (!motor2_pwm_start) {
				motor2_pwm_start = motor2_pwm_cur;
			}
			motor2_pwm_cur = map(motor2_timer, 0, motor2_trans_t, motor2_pwm_start, motor2_pwm_sp);
		}
		else {
			// transition end, reinitialise vars
#ifdef DEBUG
			Serial.println("end motor 2 transition");
#endif
			motor2_timer = 0;
			motor2_pwm_start = 0;
			motor2_is_trans = false;
			motor2_pwm_cur = motor2_pwm_sp;
		}
		
	}
	// set direction
	if (motor2_pwm_cur >= 0) {
		digitalWrite(MOTOR2DIRECTIONPIN, HIGH);
	}
	else {
		digitalWrite(MOTOR2DIRECTIONPIN, LOW);
	}
	analogWrite(MOTOR2PWMPIN,abs(motor2_pwm_cur)*2);

}
void setMotors(uint8_t motor_n, int pwm,  uint32_t trans_t) { // motor number, pwm, direction 0: backward 1: forward, transition time 
    // pwm 0-254 , trans_t in ms
	if (motor_n == 1) {
		motor_pwm_sp = pwm;
		motor_trans_t = trans_t;
		motor_is_trans = true;
#ifdef DEBUG
		Serial.print("set motor pwm to ");
		Serial.println(motor_pwm_sp);
#endif
	}
	else if (motor_n == 2) {
		motor2_pwm_sp = pwm;
		motor2_trans_t = trans_t;
		motor2_is_trans = true;
#ifdef DEBUG
		Serial.print("set motor 2 pwm to ");
		Serial.println(motor_pwm_sp);
#endif
	}

}

void stopMotors() {
	analogWrite(MOTORPWMPIN,0);
	digitalWrite(MOTORDIRECTIONPIN,HIGH);
	motor_pwm_cur = 0;
	analogWrite(MOTOR2PWMPIN,0);
	digitalWrite(MOTOR2DIRECTIONPIN,HIGH);
	motor2_pwm_cur = 0;
}