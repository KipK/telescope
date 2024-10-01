//#define DEBUG			  // enable debug output to console
#define HARDWARESERIAL   // use hardware serial instead of softserial
#define SOFTSERIALRXPIN 2 // Rx pin to DFPlayer Tx pin
#define SOFTSERIALTXPIN 4 // Tx pin to DFPlayer Rx pin
#define LEDPIN 9 		  // PWM pin for Led mosfet
#define MOTORPWMPIN 3		  // PWM pin for dc motor controller
#define MOTORDIRECTIONPIN 12 // digital pin for motor direction
#define MOTOR2PWMPIN 11		  // PWM pin for dc motor controller
#define MOTOR2DIRECTIONPIN 13 // digital pin for motor direction
#define IRSENSORPIN 8     // Ir Sensor digital pin 
#define WAITBEFORESTOP 5 // show stop when presence is LOW for x sec
#define WAITBEFORESTART 1 // show starts when presence is HIGH for x sec
#define AUDIOSTARTDELAY 0 // time in ms it takes for audio to start
#define AUDIODURATION 30 // mp3 track duration in sec