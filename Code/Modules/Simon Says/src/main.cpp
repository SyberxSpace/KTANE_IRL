// Included Libraries //

#include <Arduino.h>

#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Bounce2.h>

#include <KTANE_XT_DAC_Audio.h>

// Compilation Options //

#define DEBUG_SERIAL      true
#define DEBUG_FIXED_SEED  false
unsigned long fixed_seed = 562;
#define DEBUG_SILENT      false

// Pin Definitions //

#define soundPin      25
#define indicatorPin  26

#define rLEDPin       16
#define bLEDPin       19
#define gLEDPin       17
#define yLEDPin       18

#define rButtonPin    33
#define bButtonPin    27
#define gButtonPin    32
#define yButtonPin    23

#define randPin       34  // Used for random seed if none is given

// Data Structures //

enum colors {Red, Blue, Green, Yellow};   // Set up color tracker

struct ledController {
  unsigned long timer;
  int state;
  int wait;
  int sound;
  float brightness;
  colors color;
  void reset();
  bool isLit();
  void update();
  void fadeOut();
  void startBlink(int wait, int sound);
};

// Constants //

#define dacVolume 20      // Volume for beeps

#define ledcFreq  5000    // FastLED PWM Frequency
#define ledcRes   8       // FastLED PWM Resolution

#define fadeDuration    50
#define blinkDuration   300
#define blinkWait       300
#define repeatWait      4750
#define correctWait     1600
#define incorrectWait   4100
#define strikeDuration  500

const colors solutionOptions[4] = {Red, Blue, Green, Yellow};

const colors answerMap[2][3][4] = {     // hasVowel, numStrike, Color[R,B,G,Y]
  {                                       // No Vowel
    {Blue,    Yellow,   Green,    Red  },   // 0 Strike
    {Red,     Blue,     Yellow,   Green},   // 1 Strike
    {Yellow,  Green,    Blue,     Red  }    // 2 Strikes
  },{                                     // Has Vowel
    {Blue,    Red,      Yellow,   Green},   // 0 Strike
    {Yellow,  Green,    Blue,     Red  },   // 1 Strike
    {Green,   Red,      Yellow,   Blue }    // 2 Strikes
  }
};


// Variables //

colors solution[5];
int solutionLength;

bool hasVowel = true;
int numStrikes = 0;

int gameState = 0;          // Tracks overall behavior state
int currentStage = 0;       // Tracks progress in game
int playerStage = 0;        // Tracks the inputs for the current round
int animationStage = -1;    // Tracks progress through colors in animations
int lastButtonPressed = -1; // Tracks the last button that was pressed

bool solved = false;
bool indicatorRed = false;

int buttonPressed = 0;

unsigned long currentTime;
unsigned long indicatorTime;

ledController ledControllers[4] = {
  {0, 0, 0, 0, 0.95, Red},
  {0, 0, 0, 0, 1.00, Blue},
  {0, 0, 0, 0, 0.85, Green},
  {0, 0, 0, 0, 1.00, Yellow}
};

// Setups //

CRGB indicator[1];    // Set up FastLED strike indicator

Bounce2::Button rButton =  Bounce2::Button();   // Create button objects to track debouncing
Bounce2::Button gButton =  Bounce2::Button();
Bounce2::Button bButton =  Bounce2::Button();
Bounce2::Button yButton =  Bounce2::Button();

XT_DAC_Audio_Class DacAudio(soundPin, 0);             // Create an audio instance on the soundPin
XT_Instrument_Class SimonSays;                        // Set up the instrument used to play notes
int8_t PROGMEM ToneRed[] = {NOTE_RED, SCORE_END};     // Load the custom tone notes into progmem
int8_t PROGMEM ToneBlue[] = {NOTE_BLUE, SCORE_END};
int8_t PROGMEM ToneGreen[] = {NOTE_GREEN, SCORE_END};
int8_t PROGMEM ToneYellow[] = {NOTE_YELLOW, SCORE_END};
XT_MusicScore_Class SimonRed(ToneRed, TEMPO_MODERATO, &SimonSays);    // Create custom tracks for each note
XT_MusicScore_Class SimonBlue(ToneBlue, TEMPO_MODERATO, &SimonSays);
XT_MusicScore_Class SimonGreen(ToneGreen, TEMPO_MODERATO, &SimonSays);
XT_MusicScore_Class SimonYellow(ToneYellow, TEMPO_MODERATO, &SimonSays);
XT_MusicScore_Class *soundSelect[4] = {&SimonRed, &SimonGreen, &SimonBlue, &SimonYellow};



// Debug Functions //

#if DEBUG_SERIAL

  const String colorNames[4] = {"Red", "Blue", "Green", "Yellow"};

  void printColor(int ColorID){        // Print color name
    Serial.print(colorNames[ColorID]);
  }

  void printColorChar(int ColorID){              // Print the first character of the color name
    Serial.print(colorNames[ColorID].charAt(0));
  }

  void printSwitchError(int caseValue){
    Serial.println("WARNING: Invalid switch statement state!");
    Serial.print("State is : ");
    Serial.println(caseValue);
  }

#endif



// ledController Methods //

void ledController::reset(){
  this->timer = currentTime;
  this->state = -1;
  this->wait = 0;
  this->sound = 0;
  ledcWrite(this->color, 0);
}

bool ledController::isLit(){
  return( this->state == -1 ? false : true );
}

void ledController::update(){

  switch(this->state){

    case 0:                           // Begin animation
      this->timer = currentTime;      // Reset timer
      this->state = 1;
      #if !DEBUG_SILENT
        if(this->sound==2){
          DacAudio.Play(soundSelect[this->color], false);
        }
      #endif
      break;
    
    case 1:                                       // Fade up LED
      if(currentTime - this->timer < fadeDuration){
        ledcWrite(this->color, (float(currentTime-(this->timer))/fadeDuration) * (pow(2,ledcRes)-1) * this->brightness);
      }else{
        ledcWrite(this->color, (pow(2,ledcRes)-1) * this->brightness);
        #if !DEBUG_SILENT
          if(sound==1){
            DacAudio.Play(soundSelect[this->color], false);
          }
        #endif
        this->timer += fadeDuration;
        this->state = 2;
      }
      break;
    
    case 2:                                       // Wait while LED is lit
      if(!(currentTime - this->timer < blinkDuration)){
        this->timer += blinkDuration;
        this->state = 3;
      }
      break;

    case 3:                                       // Fade down LED
      if(currentTime - this->timer < fadeDuration){
        ledcWrite(this->color, (1-(float(currentTime-(this->timer))/fadeDuration)) * (pow(2,ledcRes)-1) * this->brightness);
      }else{
        ledcWrite(this->color, 0);
        this->timer += fadeDuration;
        this->state = 4;
      }
      break;

    case 4:                                       // Wait while LED is dark
      if(!(currentTime - this->timer < this->wait)){
        this->timer += this->wait;
        this->state = -1;
      }
      break;

    case -1:
      break;

    default:                                       // This should never happen
      #if DEBUG_SERIAL
        printSwitchError(this->state);
      #endif
      break;

  }

}

void ledController::fadeOut(){
  if(this->state == 1){
    this->state = 3;
    this->timer = 2*currentTime - (this->timer) - 50;
  }
  if(this->state == 2){
    this->state = 3;
    this->timer = currentTime;
  }
  this->wait = 0;
}

void ledController::startBlink(int wait, int sound){
  this->state = 0;
  this->wait = wait;
  this->sound = sound;
  ledcWrite(this->color, 0);
}



// Functions //

void randomizeSequence(){

  #if DEBUG_FIXED_SEED
    srand(fixed_seed);                               // Use the fixed seed if requested
  #else
    randomSeed(analogRead(randPin));
    srand(random(2147483647));                       // Otherwise use analogRead to get a random value
  #endif

  solutionLength = rand() % 3 + 3;                   // Randomize solution sequence length (3-5)

  for(int i = 0; i < solutionLength; i++){
    solution[i] = solutionOptions[rand() % 4];       // Randomize each item in the sequence
  }

  #if DEBUG_SERIAL
    Serial.print("Solution is:");                    // Print solution to Serial if enabled
    for(int i = 0; i < solutionLength; i++){
      Serial.print(" ");
      printColorChar(solution[i]);
    }
    Serial.println("");
    Serial.println("");
  #endif

}



void allReset(){
  for(int i=0; i<4; i++){
    ledControllers[i].reset();
  }
}

void allUpdate(){
  for(int i=0; i<4; i++){
    ledControllers[i].update();
  }
}

void allDark(){
  animationStage = -1;
  for(int i=0; i<4; i++){
    ledControllers[i].fadeOut();
  }
}



// Setup //

void setup(){

  FastLED.addLeds<WS2812B, indicatorPin, GRB>(indicator, 1);    // Setup indicator LED

  rButton.attach(rButtonPin, INPUT_PULLUP);   // Attach the button pin to the Bounce2 button using the internal pullup resistor
  rButton.interval(20);                       // Set a 20ms debounce interval
  rButton.setPressedState(LOW);               // Indicate that the activated state is when the pin is pulled low
  bButton.attach(bButtonPin, INPUT_PULLUP);
  bButton.interval(20);
  bButton.setPressedState(LOW);
  gButton.attach(gButtonPin, INPUT_PULLUP);
  gButton.interval(20);
  gButton.setPressedState(LOW);
  yButton.attach(yButtonPin, INPUT_PULLUP);
  yButton.interval(20);
  yButton.setPressedState(LOW);

  SimonSays.SetInstrument(5);       // Set the instrument to the custom Simon instrument
  DacAudio.DacVolume = dacVolume;   // Set the volume
  
  pinMode(rLEDPin, OUTPUT);         // set LED pins as outputs
  pinMode(bLEDPin, OUTPUT);
  pinMode(gLEDPin, OUTPUT);
  pinMode(yLEDPin, OUTPUT);
  
  ledcSetup(Red, ledcFreq, ledcRes);   // set up an ledc channel using the set frequency and resolution
  ledcAttachPin(rLEDPin, Red);         // attach led pins to the ledc channels
  ledcSetup(Blue, ledcFreq, ledcRes);
  ledcAttachPin(bLEDPin, Blue);
  ledcSetup(Green, ledcFreq, ledcRes);
  ledcAttachPin(gLEDPin, Green);
  ledcSetup(Yellow, ledcFreq, ledcRes);
  ledcAttachPin(yLEDPin, Yellow);

  #if DEBUG_SERIAL

    Serial.begin(115200);   // Begin serial connection if enabled

    delay(500);

    Serial.println("KTANE Simon Says");
    Serial.println("");

  #endif

  ledcWrite(Red, 0);   // Turn all LEDs off
  ledcWrite(Blue, 0);
  ledcWrite(Green, 0);
  ledcWrite(Yellow, 0);

  indicator[0] = CRGB(0,0,0);   // Turn off the indicator light
  FastLED.show();

  randomizeSequence();    // Randomize the solution

  allReset();
  
}



// Loop //

void loop(){

  allUpdate();        // Update button lights
  rButton.update();   // Update button states
  bButton.update();
  gButton.update();
  yButton.update();
  DacAudio.FillBuffer();  // Fill audio buffer
  currentTime = millis(); // Update time

  if(indicatorRed){
    if(currentTime - indicatorTime >= strikeDuration){
      indicator[0] = CRGB(0,0,0);
      FastLED.show();
      indicatorRed = false;
    }
  }

  if(gameState == 5){

    if(rButton.pressed()) {ledControllers[Red].startBlink(0, 0);}
    if(bButton.pressed()) {ledControllers[Blue].startBlink(0, 0);}
    if(gButton.pressed()) {ledControllers[Green].startBlink(0, 0);}
    if(yButton.pressed()) {ledControllers[Yellow].startBlink(0, 0);}

  }else if(gameState != 6){

    buttonPressed = 0;
    if(rButton.pressed()) {buttonPressed = 1;}
    if(bButton.pressed()) {buttonPressed = 2;}
    if(gButton.pressed()) {buttonPressed = 3;}
    if(yButton.pressed()) {buttonPressed = 4;}

    if(buttonPressed){
      allDark();
      if(buttonPressed-1 == answerMap[hasVowel][numStrikes][solution[playerStage]]){
        if(playerStage == currentStage){
          if(playerStage == solutionLength-1){                    // Module solved

            gameState = 5;
            indicator[0] = CRGB(0,255,0);
            FastLED.show();
            indicatorRed = false;
            ledControllers[buttonPressed-1].startBlink(0, 2);

            #if DEBUG_SERIAL
              Serial.print("Correct Button Pressed: ");
              printColor(buttonPressed-1);
              Serial.println("");
              Serial.println(" - Module Solved!");
            #endif

          }else{                                                  // Stage solved

            gameState = 3;
            currentStage++;
            playerStage = 0;
            ledControllers[buttonPressed-1].startBlink(correctWait, 2);
            lastButtonPressed = buttonPressed-1;

            #if DEBUG_SERIAL
              Serial.print("Correct Button Pressed: ");
              printColor(buttonPressed-1);
              Serial.println("");
              Serial.print(" - Stage ");
              Serial.print(currentStage);
              Serial.println(" Solved");
            #endif

          }
        }else{                                                    // Stage not completed

          gameState = 3;
          playerStage++;
          ledControllers[buttonPressed-1].startBlink(repeatWait, 2);
          lastButtonPressed = buttonPressed-1;

          #if DEBUG_SERIAL
            Serial.print("Correct Button Pressed: ");
            printColor(buttonPressed-1);
            Serial.println("");
            Serial.print(" - Moved to Step ");
            Serial.println(playerStage);
          #endif

        }
      }else{                                                      // Incorrect button pressed

        gameState = 4;
        playerStage = 0;
        indicator[0] = CRGB(255,0,0);
        FastLED.show();
        indicatorRed = true;
        indicatorTime = currentTime;
        ledControllers[buttonPressed-1].startBlink(incorrectWait, 2);
        lastButtonPressed = buttonPressed-1;
        if(numStrikes < 2){numStrikes++;}

        #if DEBUG_SERIAL
          Serial.print("Incorrect Button Pressed: ");
          printColor(buttonPressed-1);
          Serial.println("");
          Serial.print(" - Reset Progress to Stage ");
          Serial.println(currentStage);
        #endif

      }
    }

  }

  switch(gameState){
    
    // Pre-Game State //
    case 0:
      
      #if DEBUG_SERIAL
        Serial.println("Beginning Game");
      #endif

      gameState = 1;
      allReset();

      break;
    
    // Pre-Interaction Animation //
    case 1:
      if(!ledControllers[solution[0]].isLit()){
        ledControllers[solution[0]].startBlink(repeatWait, 0);
      }
      break;
    
    // Post-Interaction Animation //
    case 2:
      if(animationStage == -1 || !ledControllers[solution[animationStage]].isLit()){
        animationStage = (animationStage == currentStage) ? 0 : animationStage+1;
        ledControllers[solution[animationStage]].startBlink( (animationStage == currentStage ? repeatWait : blinkWait), 1);
      }
      break;
    
    // Correct Button Press Animation //
    case 3:
      if(!ledControllers[lastButtonPressed].isLit()){
        gameState = 2;
        animationStage = -1;
        playerStage = 0;
      }
      break;
    
    // Incorrect Button Press Animation //
    case 4:
      if(!ledControllers[lastButtonPressed].isLit()){
        gameState = 2;
        animationStage = -1;
        playerStage = 0;
      }
      break;
    
    // Solved State //
    case 5:
      break;

    // Exploded State //
    case 6:
      break;
    
    // Unused Default Case //
    default:
      #if DEBUG_SERIAL
        printSwitchError(gameState);
      #endif
      break;
  }

}