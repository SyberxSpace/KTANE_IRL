#ifndef KTANE_CONTROL
#define KTANE_CONTROL

#include <Arduino.h>

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

class ModuleID {

    public:
        enum moduleClasses : byte {
            MODULE_CLASS_NORMAL, MODULE_CLASS_NEEDY, MODULE_CLASS_SPECIAL, MODULE_CLASS_CONTROL
        };

        void create(byte bigByte, byte littleByte);
        bool fill(String moduleType, moduleClasses moduleClass, byte moduleSign);

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

    private:
        short data = 0;     // Stores the complete module CANID

        byte checkChar(byte letterIn);

};

#endif