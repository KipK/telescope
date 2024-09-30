#define DEBUG			  // enable debug output to console
#define SOFTSERIALRXPIN 2 // Rx pin to DFPlayer Tx pin
#define SOFTSERIALTXPIN 4 // Tx pin to DFPlayer Rx pin
#define LEDPIN 9 		  // PWM pin for Led mosfet
#define LEDREGISTERDIVISOR 8// default 64. register divisor for led pin  1, 8, 64, 256, and 1024 for related freq in hz: 31372, 3920, 490,122,30hz
#define MOTORPWMPIN 3		  // PWM pin for dc motor controller
#define MOTORREGISTERDIVISOR 8  // default 64. register divisor to change frequency:  1, 8, 32, 64, 128, 256, and 1024 for related freq in htz: 31372,3921hz,980,490,245,122,30
#define MOTORDIRECTIONPIN 12 // digital pin for motor direction
#define MOTOR2PWMPIN 11		  // PWM pin for dc motor controller
#define MOTOR2REGISTERDIVISOR 8 // default 64.  register divisor to change frequency:  1, 8, 32, 64, 128, 256, and 1024 for related freq in htz: 31372,3921hz,980,490,245,122,30
#define MOTOR2DIRECTIONPIN 13 // digital pin for motor direction
#define IRSENSORPIN 8     // Ir Sensor digital pin 
#define WAITBEFORESTOP 5 // show stop when presence is LOW for x sec
#define WAITBEFORESTART 1 // show starts when presence is HIGH for x sec
#define AUDIOSTARTDELAY 0 // time in ms it takes for audio to start
#define AUDIODURATION 30 // mp3 track duration in sec