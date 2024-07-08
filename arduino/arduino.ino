
#include "define.h" // config section
#include "headers.h"

// import Librairies
#include <TaskManagerIO.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>


// global variables globale, ne pas editer
bool presence = false;     // presence state
bool presence_prev = false;       // previous presence reading
bool isRunning = false;    // is device currently running
uint32_t timer_pres = 0;

SoftwareSerial mySerial(SOFTSERIALRXPIN, SOFTSERIALTXPIN); 
DFPlayerMini_Fast myMP3;


void setup() {

  // serial for debug
  Serial.begin(115200);

  // activer le soft serial pour dfplayer
  mySerial.begin(9600);
  // dfplayer
  myMP3.begin(mySerial, true);
  delay(1000); // wait 1 sec for dfplayer to initialise
  // setup Leds
  setupLeds();
  // Pin IR sur entrée
  pinMode(IRSENSORPIN, INPUT); 
  digitalWrite(IRSENSORPIN, LOW);

  // setup moteur
  setupMotors();

  // Créer les tâches
  addShowTasks();

#ifdef DEBUG
  Serial.println("Waiting for presence to start");
#endif

}

void loop() {
 
  // boucle détection de présence ( 10hz)
  if ( millis() - timer_pres >= 100) {
    timer_pres = millis();
    presenceController();
  }

  // faire tourner taskManager seulement quand le show est en cours
  if (isRunning) {
    taskManager.runLoop();
      // if (!myMP3.isPlaying()) {
      //   stopAll();
      // }
  }



}


// fonctions

void stopAll() {
#ifdef DEBUG
  Serial.println("stop all");
#endif
  isRunning = false;

  // Arrêt musique
  myMP3.stop();

  // Arrêt moteur
  stopMotors();
  // Arrêt leds
  stopLeds();
    // reset taskManager
  taskManager.reset();
  // Ajout des tâches
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
  ////// Tâches système ne pas éditer \\\\\\//
  taskid_t ledCtrl_t = taskManager.schedule(repeatMillis(20), [] {
  // contrôle des leds à 50hz
  ledController();
	});

  taskid_t motorCtrl_t = taskManager.schedule(repeatMillis(20), [] {
		// controle des moteurs à 50hz
    motorController();
	});

 ////// Tâches du show, Editer ici \\\\//
  taskid_t mainTask_t = taskManager.scheduleOnce( 0, [] {
  #ifdef DEBUG
    Serial.println("Setting volume to max");
  #endif
    myMP3.volume(30); // min 0 - 30 max
    myMP3.play(1);
      #ifdef DEBUG
    Serial.println("audio track 001.mp3 started");
  #endif
	});

  // modèle: 
  // mainTask_t = taskManager.scheduleOnce({timecode} + AUDIOSTARTDELAY, [] { // CHOSES A FAIRE ICI // });
  // setLeds(pwm 0/254, transition time in ms)
  // setMotors(pwm -125/125, transition time in ms)

  mainTask_t = taskManager.scheduleOnce(2000+AUDIOSTARTDELAY, [] {
    setLeds(125, 3000);
    setMotors(60, 3000);

	});

  mainTask_t = taskManager.scheduleOnce( 8000+AUDIOSTARTDELAY, [] {
    setLeds(55, 4000);
    setMotors(-110, 8000);
	});

  // Fin du show
  mainTask_t = taskManager.scheduleOnce( (AUDIODURATION * 1000) + AUDIOSTARTDELAY, [] {
    stopAll();
	});
}