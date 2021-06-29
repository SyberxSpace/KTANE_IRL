#include <Arduino.h>

#define FASTLED_INTERNAL
#include <FastLED.h>
#include <CAN.h>
#include <Bounce2.h>
#include "KTANE_Control.h"
#include "credentials.h"

bool useSerial = true;
bool useTelnet = true;

#define KTANEPrint(...) if(useSerial){Serial.printf(__VA_ARGS__);}if(useTelnet){TelnetStream.printf(__VA_ARGS__);}

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

ModuleID thisModule;
ModuleID copyModule;

SerialNumber testSerial;

void setup() {

	Serial.begin(115200);
	delay(100);

	thisModule.fill(THIS_MODULE_TYPE, THIS_MODULE_CLASS, THIS_MODULE_SIGNATURE);
	setupOTA(thisModule, mySSID, myPassword);

}

void loop() {

	KTANEPrint("\r\n\r\n++++ Begin Test ++++\r\n");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Attempting Serial with \"AAAAAA\" : %s\r\n", testSerial.fill("AAAAAA") ? "Success" : "Fail");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Attempting Serial with \"5B6QA2\" : %s\r\n", testSerial.fill("5B6QA2") ? "Success" : "Fail");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial data : %.2X%.2X%.2X%.2X : %s\r\n", testSerial.byte1(), testSerial.byte2(), testSerial.byte3(), testSerial.byte4(), testSerial.label());
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial %s a vowel.\r\n", testSerial.hasVowel ? "has" : "does not have");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial %s even.\r\n", testSerial.isEven ? "is" : "is not");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Attempting Serial with \"2B0MB5\" : %s\r\n", testSerial.fill("2B0MB5") ? "Success" : "Fail");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial data : %.2X%.2X%.2X%.2X : %s\r\n", testSerial.byte1(), testSerial.byte2(), testSerial.byte3(), testSerial.byte4(), testSerial.label());
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial %s a vowel.\r\n", testSerial.hasVowel ? "has" : "does not have");
	ArduinoOTA.handle();
	delay(500);

	KTANEPrint("Serial %s even.\r\n", testSerial.isEven ? "is" : "is not");
	ArduinoOTA.handle();
	delay(500);

	ArduinoOTA.handle();
	delay(500);
}