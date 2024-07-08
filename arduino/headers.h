#include <Arduino.h>

void setupLeds();
void ledController();
void setLeds(uint8_t pwm, uint32_t trans_t);
void setupMotors();
void setMotors(int8_t pwm, uint32_t trans_t);
void motorController();
void stopLeds();
void stopMotors();