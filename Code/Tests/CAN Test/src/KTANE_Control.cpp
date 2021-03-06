#include "KTANE_Control.h"

// ++++++++ OTA ++++++++ //

void setupOTA(ModuleID thisModule, const char* ssid, const char* password) {
    // Configure the hostname
    uint16_t maxlen = 12;
    char *fullhostname = new char[maxlen];
    snprintf(fullhostname, maxlen, "KTANE-%c%c-%.2i", thisModule.letter1(), thisModule.letter2(), (int) thisModule.moduleSign());
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;

    // Configure and start the WiFi station
    byte rebootCounter = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        if(rebootCounter < 3){
            Serial.println("Connection Failed! Retrying...");
            WiFi.persistent(false);
            WiFi.disconnect();
            WiFi.mode(WIFI_OFF);
            delay (1000);
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
            delay(2000);
        }else{
            Serial.println("Connection Failed! Rebooting...");
            delay(5000);
            ESP.restart();
        }

    }

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("\nAuth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("\nBegin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("\nConnect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("\nReceive Failed");
        else if (error == OTA_END_ERROR) Serial.println("\nEnd Failed");
    });

    ArduinoOTA.begin();
    TelnetStream.begin();

    Serial.println("OTA Initialized");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}


// ++++++++ EDGEWORK ++++++++ //

void Edgework::create(edgeworkType type, byte bigByte, byte littleByte) {
    this->type = type;
    data = 0x0000;
    data = data + bigByte;
    data = data << 8;
    data = data + littleByte;
}

bool Edgework::fillBattery(char batteryType) {
    type = EDGEWORK_TYPE_BATTERY;
    switch(batteryType) {
    case 'A':
        data = 2;
        return 1;
        break;
    case 'D':
        data = 1;
        return 1;
        break;
    default:
        type = EDGEWORK_TYPE_NONE;
        return 0;
        break;
    }
}

bool Edgework::fillIndicator(String label, bool light) {
    bool success = true;
    data = 0x0000;
    for(int i = 0; i < 3; i++) {
    if(label.charAt(i) >= 'A' && label.charAt(i) <= 'Z') {
        data = data << 5;
        data = data + (label.charAt(i) - 'A');
    }else{
        success = false;
    }
    }
    data = light ? data + 0b1000000000000000 : data;
    type = success ? EDGEWORK_TYPE_INDICATOR : EDGEWORK_TYPE_NONE;
    return success;
}

bool Edgework::fillPort(bool DVI, bool Parallel, bool PS2, bool RJ45, bool Serial, bool RCA) {
    data = 0x0000;
    data += DVI ?       0b0000000000100000 : 0;
    data += Parallel ?  0b0000000000010000 : 0;
    data += PS2 ?       0b0000000000001000 : 0;
    data += RJ45 ?      0b0000000000000100 : 0;
    data += Serial ?    0b0000000000000010 : 0;
    data += RCA ?       0b0000000000000001 : 0;
    type = EDGEWORK_TYPE_PORT;
    return true;
}

byte Edgework::bigByte() {
    return ((data & 0b1111111100000000) >> 8);
}

byte Edgework::littleByte() {
    return (data & 0b0000000011111111);
}

Edgework::edgeworkType Edgework::isType() {
    return type;
}

bool Edgework::isAA() {
    return type==EDGEWORK_TYPE_BATTERY && data==2;
}

bool Edgework::isD() {
    return type==EDGEWORK_TYPE_BATTERY && data==1;
}

int Edgework::numBatteries() {
    return type==EDGEWORK_TYPE_BATTERY ? data : 0;
}

String Edgework::label() {
    if(type==EDGEWORK_TYPE_INDICATOR){
    String text = "   ";
    text.setCharAt(0, ((data & 0b0111110000000000) >> 10) + 'A');
    text.setCharAt(1, ((data & 0b0000001111100000) >> 5) + 'A');
    text.setCharAt(2, (data & 0b0000000000011111) + 'A');
    return text;
    }else{
    return "   ";
    }
}

bool Edgework::isLit() {
    return type==EDGEWORK_TYPE_INDICATOR ? ((data & 0b1000000000000000) != 0) : false;
}

bool Edgework::hasDVI() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000100000) != 0) : false;
}

bool Edgework::hasParallel() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000010000) != 0) : false;
}

bool Edgework::hasPS2() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000001000) != 0) : false;
}

bool Edgework::hasRJ45() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000000100) != 0) : false;
}

bool Edgework::hasSerial() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000000010) != 0) : false;
}

bool Edgework::hasRCA() {
    return type==EDGEWORK_TYPE_PORT ? ((data & 0b0000000000000001) != 0) : false;
}

bool Edgework::isBlank() {
    return type==EDGEWORK_TYPE_PORT ? (data == 0) : false;
}


// ++++++++ Serial Number ++++++++ //

String SerialNumber::label() {
    String output = "??????";
    if((data & 0xFC000000) != 0){
        if((data & 0xFC000000) >> 26 <= 26){
            output.setCharAt(0, ((data & 0xFC000000) >> 26) - 1 + 'A');
        }else{
            output.setCharAt(0, ((data & 0xFC000000) >> 26) - 27 + '0');
        }
    }
    if((data & 0x03F00000) != 0){
        if((data & 0x03F00000) >> 20 <= 26){
            output.setCharAt(1, ((data & 0x03F00000) >> 20) - 1 + 'A');
        }else{
            output.setCharAt(1, ((data & 0x03F00000) >> 20) - 27 + '0');
        }
    }
    if((data & 0x000F0000) != 0){
        output.setCharAt(2, ((data & 0x000F0000) >> 16) - 1 + '0');
    }
    if((data & 0x0000F800) != 0){
        output.setCharAt(3, ((data & 0x0000F800) >> 11) - 1 + 'A');
    }
    if((data & 0x000007C0) != 0){
        output.setCharAt(4, ((data & 0x000007C0) >> 6) - 1 + 'A');
    }
    if((data & 0x0000003C) != 0){
        output.setCharAt(5, ((data & 0x0000003C) >> 2) - 1 + '0');
    }
    return output;
}

void SerialNumber::updateTags() {
    String localString = label();
    hasVowel = false;
    for(int i = 0; i < 6; i++){
        hasVowel = hasVowel || (localString.charAt(i) == 'A');
        hasVowel = hasVowel || (localString.charAt(i) == 'E');
        hasVowel = hasVowel || (localString.charAt(i) == 'I');
        hasVowel = hasVowel || (localString.charAt(i) == 'O');
        hasVowel = hasVowel || (localString.charAt(i) == 'U');
    }
    if(localString.charAt(5) != '?'){
        isEven = (localString.charAt(5) - '0') % 2 == 0 ? true : false;
        isOdd = !isEven;
    }
}

void SerialNumber::create(byte byte1, byte byte2, byte byte3, byte byte4) {
    data = 0x00000000;
    data = data + byte1;
    data = data << 8;
    data = data + byte2;
    data = data << 8;
    data = data + byte3;
    data = data << 8;
    data = data + byte4;
    updateTags();
}

bool SerialNumber::fill(String serialString) {
    bool fail = false;
    data = 0x00000000;

    if(serialString.charAt(0) >= 'A' && serialString.charAt(0) <= 'Z'){
        data = data + (serialString.charAt(0) - 'A' + 1);
    }else if(serialString.charAt(0) >= '0' && serialString.charAt(0) <= '9'){
        data = data + (serialString.charAt(0) - '0' + 27);
    }else{
        fail = true;
    }
    data = data << 6;

    if(serialString.charAt(1) >= 'A' && serialString.charAt(1) <= 'Z'){
        data = data + (serialString.charAt(1) - 'A' + 1);
    }else if(serialString.charAt(1) >= '0' && serialString.charAt(1) <= '9'){
        data = data + (serialString.charAt(1) - '0' + 27);
    }else{
        fail = true;
    }
    data = data << 4;

    if(serialString.charAt(2) >= '0' && serialString.charAt(2) <= '9'){
        data = data + (serialString.charAt(2) - '0' + 1);
    }else{
        fail = true;
    }
    data = data << 5;

    if(serialString.charAt(3) >= 'A' && serialString.charAt(3) <= 'Z'){
        data = data + (serialString.charAt(3) - 'A' + 1);
    }else{
        fail = true;
    }
    data = data << 5;

    if(serialString.charAt(4) >= 'A' && serialString.charAt(4) <= 'Z'){
        data = data + (serialString.charAt(4) - 'A' + 1);
    }else{
        fail = true;
    }
    data = data << 4;

    if(serialString.charAt(5) >= '0' && serialString.charAt(5) <= '9'){
        data = data + (serialString.charAt(5) - '0' + 1);
    }else{
        fail = true;
    }
    data = data << 2;

    if(fail) {
        data = 0x00000000;
    }
    updateTags();
    return !fail;
}

byte SerialNumber::byte1() {
    return (byte) ((data & 0xFF000000) >> 24);
}

byte SerialNumber::byte2() {
    return (byte) ((data & 0x00FF0000) >> 16);
}

byte SerialNumber::byte3() {
    return (byte) ((data & 0x0000FF00) >> 8);
}

byte SerialNumber::byte4() {
    return (byte) (data & 0x000000FF);
}


// ++++++++ CAN ID ++++++++ //

byte ModuleID::checkChar(byte letterIn) {
    if(letterIn >= 'A' && letterIn <= 'Z'){
        return (byte) letterIn - ('A'-1);
    }else{
        return 0;
    }
}

void ModuleID::create(byte bigByte, byte littleByte) {
    data = 0x0000;
    data = data + bigByte;
    data = data << 8;
    data = data + littleByte;
}

bool ModuleID::fill(String moduleType, moduleClasses moduleClass, byte moduleSign) {
    data = 0x0000;
    byte letter1 = moduleType.charAt(0);
    byte letter2 = moduleType.charAt(1);
    letter1 = ModuleID::checkChar(letter1);
    letter2 = ModuleID::checkChar(letter2);
    if (letter1==0 || letter2==0 || moduleSign > 0b00001111){
        return false;
    }else{
        data = data + letter1;
        data = data << 5;
        data = data + letter2;
        data = data << 2;
        data = data + moduleClass;
        data = data << 4;
        data = data + moduleSign;
        return true;
    }
}

bool ModuleID::fill(String moduleType, int moduleClass, int moduleSign) {
    return ModuleID::fill(moduleType, (ModuleID::moduleClasses) moduleClass, (byte) moduleSign);
}

byte ModuleID::bigByte() {
    return ((data & 0b1111111100000000) >> 8);
}

byte ModuleID::littleByte() {
    return (data & 0b0000000011111111);
}

String ModuleID::moduleType() {
    if((data & 0b1111111111000000) == 0){
        return "??";
    }else{
        String toReturn;
        toReturn.concat( (char) ('A' + ((data & 0b1111100000000000) >> 11) - 1) );
        toReturn.concat( (char) ('A' + ((data & 0b0000011111000000) >> 6) - 1) );
        return toReturn;
    }
}

char ModuleID::letter1() {
    if((data & 0b1111100000000000) == 0){
        return '?';
    }else{
        return 'A' + ((data & 0b1111100000000000) >> 11) - 1;
    }
}

char ModuleID::letter2() {
    if((data & 0b0000011111000000) == 0){
        return '?';
    }else{
        return 'A' + ((data & 0b0000011111000000) >> 6) - 1;
    }
}

ModuleID::moduleClasses ModuleID::moduleClass() {
    return (ModuleID::moduleClasses) ((data & 0b0000000000110000) >> 4);
}

bool ModuleID::isModule() {
    return (ModuleID::moduleClasses) ((data & 0b0000000000110000) >> 4) == MODULE_CLASS_NORMAL;
}

bool ModuleID::isNeedy() {
    return (ModuleID::moduleClasses) ((data & 0b0000000000110000) >> 4) == MODULE_CLASS_NEEDY;
}

bool ModuleID::isSpecial() {
    return (ModuleID::moduleClasses) ((data & 0b0000000000110000) >> 4) == MODULE_CLASS_SPECIAL;
}

bool ModuleID::isControl() {
    return (ModuleID::moduleClasses) ((data & 0b0000000000110000) >> 4) == MODULE_CLASS_CONTROL;
}

byte ModuleID::moduleSign() {
    return data & 0b0000000000001111;
}