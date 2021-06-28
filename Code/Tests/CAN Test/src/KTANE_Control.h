#ifndef KTANE_CONTROL
#define KTANE_CONTROL

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>

#ifndef THIS_MODULE_TYPE
#define THIS_MODULE_TYPE "AA"
#endif

#ifndef THIS_MODULE_CLASS
#define THIS_MODULE_CLASS 0
#endif

#ifndef THIS_MODULE_SIGNATURE
#define THIS_MODULE_SIGNATURE 0
#endif

class Edgework {
    
    public:
        enum edgeworkType : byte {
            EDGEWORK_TYPE_NONE, EDGEWORK_TYPE_BATTERY, EDGEWORK_TYPE_INDICATOR, EDGEWORK_TYPE_PORT
        };

        void create(edgeworkType type, byte bigByte, byte littleByte);
        bool fillBattery(char batteryType);
        bool fillIndicator(String label, bool light);
        bool fillPort(bool DVI, bool Parallel, bool PS2, bool RJ45, bool Serial, bool RCA);

        byte bigByte();
        byte littleByte();
        edgeworkType isType();
        bool isAA();
        bool isD();
        int numBatteries();
        String label();
        bool isLit();
        bool hasDVI();
        bool hasParallel();
        bool hasPS2();
        bool hasRJ45();
        bool hasSerial();
        bool hasRCA();
        bool isBlank();

    private:
        edgeworkType type = EDGEWORK_TYPE_NONE;    // Stores the type of edgework
        short data = 0;                            // Stores the data the edgework contains
    
};

class SerialNumber {

    public:
        bool hasVowel = false;
        bool isEven = false;
        bool isOdd = false;

        String label();

    private:
        long data = 0x00000000;
        void updateTags();

    public:
        void create(byte byte1, byte byte2, byte byte3, byte byte4);
        bool fill(String serialString);

        byte byte1();
        byte byte2();
        byte byte3();
        byte byte4();

};

class ModuleID {

    private:
        short data = 0;     // Stores the complete module CANID

        byte checkChar(byte letterIn);

    public:
        enum moduleClasses : byte {
            MODULE_CLASS_NORMAL, MODULE_CLASS_NEEDY, MODULE_CLASS_SPECIAL, MODULE_CLASS_CONTROL
        };

        void create(byte bigByte, byte littleByte);
        bool fill(String moduleType, moduleClasses moduleClass, byte moduleSign);
        bool fill(String moduleType, int moduleClass, int moduleSign);

        byte bigByte();
        byte littleByte();
        String moduleType();
        char letter1();
        char letter2();
        moduleClasses moduleClass();
        bool isModule();
        bool isNeedy();
        bool isSpecial();
        bool isControl();
        byte moduleSign();

};

/*
class KTANE {

    public:
        KTANE(String type, int class, int signature);
        Edgework edgework1;
        Edgework edgework2;
        Edgework edgework3;
        Edgework edgework4;
        Edgework edgework5;
        SerialNumber serialNum;
};
*/

void setupOTA(ModuleID thisModule, const char* ssid, const char* password);

#endif