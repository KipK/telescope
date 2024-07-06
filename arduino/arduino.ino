
#include "define.h" // config section
#include <Arduino.h>
#include <TaskManagerIO.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

// instanciate Objects
SoftwareSerial mySerial(softSerialRxPin, softSerialTxPin); 
DFPlayerMini_Fast myMP3;


void setup() {
  // initialise components

  // serial for debug
  Serial.begin(115200);

  // soft serial for dfplayer
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
  delay(1000); // wait 1 sec for dfplayer to initialise


  // Create Tasks
  taskid_t dfplayer_t = taskManager.scheduleOnce( 0, [] {
    Serial.println("Setting volume to max");
    myMP3.volume(30);
    Serial.print("play first track 001.mp3");
    myMP3.play(1);
	});

  taskid_t motor_t = taskManager.scheduleOnce( 10000, [] {
	  Serial.print("first motor task");
	});
  motor_t = taskManager.scheduleOnce( 20000, [] {
	  Serial.print("second motor task");
	});

  taskid_t light_t = taskManager.scheduleOnce( 10000, [] {
	  Serial.print("first light task");
	});

  light_t = taskManager.scheduleOnce( 25000, [] {
	  Serial.print("second light task");
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
  // todo

  // reset taskManager
  taskManager.reset();
}

void presenceController() {
  presence = digitalRead(irSensorPin);

  if (!presence) {
    if (isRunning) {
      lastSeen = millis();

      // check if there's nobody since x sec
      if (lastSeen >= waitBeforeReset * 1000) {
        // reset to waiting state
        stopAll();
      }
    }

  }
  else {
    lastSeen = 0;
    if (!isRunning) {
      isRunning = true;
    }
  }
}