
#include "define.h" // config section
#include "headers.h"

// import Libraries
#include <TaskManagerIO.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>


// global variables, do not edit
uint32_t lastSeen = 99999; // last time in ms since the IR sensor has not been HIGH
uint32_t timer_pres = 0; // timer for presence loop
bool presence = false;     // presence state
bool isRunning = false;    // is device currently running


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

  // Create Tasks

  taskid_t ledCtrl_t = taskManager.schedule(repeatMillis(20), [] {
		// led controller refreshing at 50hz
    ledController();
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
  #ifdef DEBUG
    Serial.println("set leds to 125");
  #endif
	});

  mainTask_t = taskManager.scheduleOnce( 8000-AUDIOSTARTDELAY, [] {
    setLeds(55, 4000);
  #ifdef DEBUG
    Serial.println("set leds to 55");
  #endif
	});
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
      if (!myMP3.isPlaying()) {
        stopAll();
      }
  }



}


// functions

void stopAll() {
  isRunning = false;
  // stop music
  myMP3.stop();

  // stop motors
  // todo

  // shutdown lights
  digitalWrite(LEDPIN,0);

  // reset taskManager
  taskManager.reset();
}

void presenceController() {
  presence = digitalRead(IRSENSORPIN);

  if (!presence) {
    if (isRunning) {
      lastSeen = millis();

      // check if there's nobody since x sec
      if (lastSeen >= WAITBEFORERESET * 1000) {
        // reset to waiting state
#ifdef DEBUG
        Serial.println("Arrêt, pas de présence depuis 5s");
#endif
        stopAll();
        lastSeen = 0;
      }
    }

  }
  else {
    lastSeen = 0;
    if (!isRunning) {
      isRunning = true;
#ifdef DEBUG
      Serial.println("Présence, démarrage");
#endif   
    }
  }
}