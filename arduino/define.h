#define softSerialRxPin 2
#define softSerialTxPin 3
#define irSensorPin 8  // Ir Sensor digital pin 
#define waitBeforeReset 5 // time in sec to wait before reset when no presence is detected


// global variables, do not edit
unsigned int lastSeen = 99999; // last time in ms since the IR sensor has not been HIGH
unsigned int timer_pres = 0; // timer for presence loop
bool presence = false;     // presence state
bool isRunning = false;    // is device currently running

