
#include "define.h" // config section
#include "headers.h"

// import Libraries
#include <TaskManagerIO.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>


// global variables, do not edit
bool presence = false;     // presence state
bool presence_prev = false;       // previous presence reading
bool isRunning = false;    // is device currently running
uint32_t timer_pres = 0;

// instanciate Objects
SoftwareSerial mySerial(SOFTSERIALRXPIN, SOFTSERIALTXPIN); 
DFPlayerMini_Fast myMP3;


void setup() {
  // initialise components

  // serial for debug
  Serial.begin(115200);

  // soft serial for dfplayer
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
  delay(1000); // wait 1 sec for dfplayer to initialise
  // Led setup
  setupLeds();
  // set IR pin as input
  pinMode(IRSENSORPIN, INPUT); 
  digitalWrite(IRSENSORPIN, LOW);

  // Motors setup
  setupMotors();

  // Create Tasks
  addShowTasks();

}

void loop() {
 
  // presence detection loop ( 10hz)
  if ( millis() - timer_pres >= 100) {
    timer_pres = millis();
    presenceController();
  }

  // run taskManager only when show is running
  if (isRunning) {
    taskManager.runLoop();
      // if (!myMP3.isPlaying()) {
      //   stopAll();
      // }
  }



}


// functions

void stopAll() {
#ifdef DEBUG
  Serial.println("stop all");
#endif
  isRunning = false;

  // stop music
  myMP3.stop();

  // stop motors
  analogWrite(MOTORDIRECTIONPIN, 0);

  // shutdown lights
  analogWrite(LEDPIN,0);

    // reset taskManager
  taskManager.reset();
  // readd tasks
  addShowTasks();

}

void presenceController() {
  static uint32_t timer_t = 0;
  presence = digitalRead(IRSENSORPIN);
  if (presence) { //presence
    if (presence_prev != presence) {
      if (!timer_t) {
        timer_t = millis();
      }
      if (millis() - timer_t >= WAITBEFORESTART * 1000) {
        isRunning = true;
        presence_prev = presence;
        timer_t = 0;
  #ifdef DEBUG
        Serial.println("starting show");
  #endif
      }
    }
  }
  else { // no presence
    if (presence_prev != presence) {
      if (!timer_t) {
        timer_t = millis();
      }
      if (millis() - timer_t >= WAITBEFORESTOP * 1000) {
        stopAll();
        presence_prev = presence;
        timer_t = 0;
      }
    }
  }
}



void addShowTasks() {

	  taskid_t ledCtrl_t = taskManager.schedule(repeatMillis(20), [] {
		// led controller refreshing at 50hz
    ledController();
	});

  taskid_t motorCtrl_t = taskManager.schedule(repeatMillis(20), [] {
		// motor controller refreshing at 50hz
    motorController();
	});

  taskid_t mainTask_t = taskManager.scheduleOnce( 0, [] {
  #ifdef DEBUG
    Serial.println("Setting volume to max");
  #endif
    myMP3.volume(30);
  #ifdef DEBUG
    Serial.println("playing first track 001.mp3");
  #endif
    myMP3.play(1);
	});

  mainTask_t = taskManager.scheduleOnce(2000-AUDIOSTARTDELAY, [] {
    setLeds(125, 3000);
    setMotors(60, 3000);

	});

  mainTask_t = taskManager.scheduleOnce( 8000-AUDIOSTARTDELAY, [] {
    setLeds(55, 4000);
    setMotors(-110, 8000);
	});
}