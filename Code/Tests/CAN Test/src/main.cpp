#include <Arduino.h>

#define FASTLED_INTERNAL
#include <FastLED.h>
#include <CAN.h>
#include <Bounce2.h>
#include "KTANE_Control.h"

// MODULE VARIABLES //

unsigned long randSeed = 0;             // Seed used to generate random game state
int gameLength = 0;                     // Length of game in seconds
int maxStrikes = 0;                     // Strikes allowed before game over
char serialNumber[6] = {0,0,0,0,0,0};   // Serial tag

Edgework edgeWork1;
Edgework edgeWork2;
Edgework edgeWork3;
Edgework edgeWork4;
Edgework edgeWork5;

void setup() {

  edgeWork1.fillBattery('A');
  edgeWork2.fillIndicator("FRK",true);
  edgeWork3.fillPort(1,0,1,0,0,1);
  edgeWork4.fillIndicator("CAR",false);
  edgeWork5.create(edgeWork2.isType(), edgeWork2.bigByte(), edgeWork2.littleByte());

  Serial.begin(115200);
  delay(100);

  Serial.println(edgeWork1.numBatteries());
  Serial.println(edgeWork2.label());
  Serial.println(edgeWork3.hasParallel());
  Serial.println(edgeWork4.isLit());
  Serial.println(edgeWork5.isType());
  Serial.println(edgeWork5.label());
  Serial.println(edgeWork5.isLit());
}

void loop() {

}