// OTA and Multi-core Template by Nicholas Schwankl (Syber-Space)

// Uses 0TA (if OTA enabled).
// Feel free to use this template for any project (no need to include attribution).
// If OTA does now work properly consider increasing stackSize.

//++++++++++ PRE-PROCESSOR OPTIONS ++++++++++//

#define DEBUG true  //enables Serial and debugging
#define OTA   true  //enables OTA

char NAME[] = "Template Sketch";

//+++++++++++++ PIN ASSIGNMENTS +++++++++++++//



//+++++++++++++ GENERAL OPTIONS +++++++++++++//



//+++++++++++++++ END OPTIONS +++++++++++++++//

#if OTA
  #include <OTA.h>
#endif

#define stackSize   1024*8
TaskHandle_t Loop2;


void setup() {

  #if DEBUG
    Serial.begin(115200);
    Serial.println((String)"Booting - [DEBUG: TRUE] [OTA: " + (OTA ? "True" : "False") + "]");
    delay(100);
  #endif

  #if OTA
    ArduinoOTA.setHostname(NAME);
    setupOTA();
    delay(100);
  #endif
  
  xTaskCreatePinnedToCore(loop2, "Loop2", stackSize, NULL, 1, &Loop2, 0);
  delay(500);

//+++++++++++++++ BEGIN SETUP +++++++++++++++//
  
  
  
//++++++++++++++++ END SETUP ++++++++++++++++//

}


void loop() {
  
//+++++++++++++ BEGIN MAIN LOOP +++++++++++++//
  
  
  
//++++++++++++++ END MAIN LOOP ++++++++++++++//

}


void loop2(void * parameter) {
  for(;;) {

    #if OTA
      ArduinoOTA.handle();
    #endif

//++++++++++++ BEGIN SECOND LOOP ++++++++++++//
    
    
    
//+++++++++++++ END SECOND LOOP +++++++++++++//
    
    vTaskDelay(1);
    
  }
}
