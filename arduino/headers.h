#include <Arduino.h>

void setupLeds();
void ledController();
void setLeds(uint8_t pwm, uint32_t trans_t);
void setupMotors();
void setMotors(uint8_t motor, int8_t pwm, uint32_t trans_t);
void motorController();
void motor2Controller();
void stopLeds();
void stopMotors();