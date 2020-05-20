// KTANE Maze Module (independent operation test) by Nicholas Schwankl
// Uses FastLED [https://github.com/FastLED/FastLED], Bounce 2 [https://github.com/thomasfredericks/Bounce2], and Andreas Spiess's OTA library [https://github.com/SensorsIot/ESP32-OTA].
// Keep Talking and Nobody Explodes holds all rights to their game. Please buy a copy from https://keeptalkinggame.com/ before making these modules.

// PRE-PROCESSOR OPTIONS //

#define DEBUG true //makes maze visible
#define OTA   true //enables OTA

// PIN ASSIGNMENTS //

#define SCREEN      25
#define INDICATOR   26
#define SETUP       21

#define UP          19
#define LEFT        18
#define DOWN        17
#define RIGHT       16

#define UNATTACHED  34 //used for RNG

// END OPTIONS //

#include <FastLED.h>
#include <Bounce2.h>
#include "mazes.h"

#if OTA
  #include <OTA.h>
  unsigned long entry;
#endif

#define NUM_GRID_LEDS 144
CRGB leds[NUM_GRID_LEDS];
CRGB indicator[1];

#define debounceTime     20
Bounce up_db =    Bounce();
Bounce left_db =  Bounce();
Bounce down_db =  Bounce();
Bounce right_db = Bounce();

boolean upState         = false;
boolean upStateLast     = false;
boolean leftState       = false;
boolean leftStateLast   = false;
boolean downState       = false;
boolean downStateLast   = false;
boolean rightState      = false;
boolean rightStateLast  = false;

const byte mainDotsMap[6][6] = { //provides mapping from the 6x6 positions to the 144 pixels
  {104,  96, 56, 48,  8,  0},
  {106,  98, 58, 50, 10,  2},
  {120, 112, 72, 64, 24, 16},
  {122, 114, 74, 66, 26, 18},
  {136, 128, 88, 80, 40, 32},
  {138, 130, 90, 82, 42, 34}
};

const byte mazeMap[11][11] = { //provides mapping from the 11x11 arrays to the 144 pixels
  {104,103,96,63,56,55,48,15,8,07,0},
  {105,102,97,62,57,54,49,14,9,06,1},
  {106,101,98,61,58,53,50,13,10,05,2},
  {107,100,99,60,59,52,51,12,11,04,3},
  {120,119,112,79,72,71,64,31,24,23,16},
  {121,118,113,78,73,70,65,30,25,22,17},
  {122,117,114,77,74,69,66,29,26,21,18},
  {123,116,115,76,75,68,67,28,27,20,19},
  {136,135,128,95,88,87,80,47,40,39,32},
  {137,134,129,94,89,86,81,46,41,38,33},
  {138,133,130,93,90,85,82,45,42,37,34}
};

byte maze[11][11];

byte mazeChoice = random(0,9); //randomly pick a maze

byte posX = random(0,6); //randomize start position
byte posY = random(0,6); //randomize start position

byte goalX = posX;
byte goalY = posY;

boolean solvedState = false;
boolean strikeState = false;
long    strikeTime  = 0;

CRGB dark = CRGB(4,4,8);        //color of unoccupied spaces
CRGB white = CRGB(255,255,255); //color of player position
CRGB red = CRGB(255,0,0);       //color of walls
CRGB darkred = CRGB(25,0,0);    //color of debug walls
CRGB circle = CRGB(0,255,0);    //color of circle positions during setup

CRGB indicatorSolved = CRGB(0,100,0);   //color of indicator when module solved
CRGB indicatorStrike = CRGB(100,0,0);   //color of indicator when module solved
CRGB indicatorSetup  = CRGB(0,0,100);   //color of indicator when module solved

const int maxPulse   = 255;     //max brightness of goal pulse
const int minPulse   = 100;     //min brightness of goal pulse
CRGB pulse = CRGB(0,0,0);       //color of goal position (placeholder)


void setup() {

  #if OTA
    ArduinoOTA.setHostname("MazeModule");
    setupOTA();
  #endif

  pinMode(UP, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(SETUP, INPUT_PULLUP);

  up_db.attach(UP);
  up_db.interval(debounceTime);
  left_db.attach(LEFT);
  left_db.interval(debounceTime);
  down_db.attach(DOWN);
  down_db.interval(debounceTime);
  right_db.attach(RIGHT);
  right_db.interval(debounceTime);
  
  FastLED.addLeds<WS2812B, SCREEN, GRB>(leds, NUM_GRID_LEDS); //setup leds
  FastLED.addLeds<WS2812B, INDICATOR, GRB>(indicator, 1);
  delay(500);

  randomSeed(analogRead(UNATTACHED));
  while(goalX==posX && goalY==posY){ //randomize goal position to not be on player
    goalX = random(0,6);
    goalY = random(0,6);
  }
  
  for(int i=0; i<11; i++){
    for(int j=0; j<11; j++){
      maze[i][j] = mazes[mazeChoice][i][j]; //fill in the maze matrix from storage
    }
  }

  for(int i=0; i<NUM_GRID_LEDS; i++){ //clear screen
    leds[i] = CRGB(0,0,0);
  }
  indicator[0] = CRGB(0,0,0);
  FastLED.show();
  delay(100);

  indicator[0] = indicatorSetup; //display setup indicator, circle positions, and maze (if in DEBUG)
  for(int i=0; i<11; i++){
    for(int j=0; j<11; j++){
      if(maze[i][j] == 4){
        leds[ mazeMap[i][j] ] = circle;
      }
      if(maze[i][j] == 2){
        leds[ mazeMap[i][j] ] = dark;
      }
      #if DEBUG
        if(maze[i][j] == 1){
          leds[ mazeMap[i][j] ] = darkred;
        }
      #endif
    }
  }
  FastLED.show();
  delay(100);

  while(digitalRead(SETUP) == HIGH){ //wait for completed setup
    #if OTA
      ArduinoOTA.handle();
    #endif
  }

  indicator[0] = CRGB(0,0,0);
  for(int i=0; i<11; i++){
    for(int j=0; j<11; j++){
      if(maze[i][j] == 2 || maze[i][j] == 4){ //fill in dots
        leds[ mazeMap[i][j] ] = dark;
      }
    }
  }

  #if DEBUG
    for(int i=0; i<11; i++){
      for(int j=0; j<11; j++){
        if(maze[i][j] == 1){
          leds[ mazeMap[i][j] ] = darkred; //show maze if in debug
        }
      }
    }
  #endif

  FastLED.show();
  delay(100);

}


void loop() {

  #if OTA
    ArduinoOTA.handle();
  #endif

  up_db.update();    //update debouncing
  left_db.update();
  down_db.update();
  right_db.update();

  upStateLast = upState;    //update button states
  upState = (up_db.read()==LOW);
  leftStateLast = leftState;
  leftState = (left_db.read()==LOW);
  downStateLast = downState;
  downState = (down_db.read()==LOW);
  rightStateLast = rightState;
  rightState = (right_db.read()==LOW);

  pulse = CRGB( map(quadwave8(millis()/(20)*PI),0,255,minPulse,maxPulse) ,0,0); //update pulse animation

  if(solvedState){

    indicator[0] = indicatorSolved;
    leds[ mainDotsMap[goalY][goalX] ] = pulse;
    
  }else{

    if(strikeState){
      if(strikeTime+1000 < millis()){
        indicator[0] = CRGB(0,0,0);
      }else{
        indicator[0] = indicatorStrike;
      }
    }

    if(upState > upStateLast){
      if( posY > 0 ){                                 //check if move is in bounds
        if( maze[posY*2-1][posX*2] == 1 ){            //check if move is valid
          strikeState = true;                         //strike if it isn't
          indicator[0] = indicatorStrike;
          strikeTime = millis();
          leds[ mazeMap[posY*2-1][posX*2] ] = red;
        }else{
          leds[ mainDotsMap[posY][posX] ] = dark;
          posY = posY-1;                              //move if it is
        }
      }
    }

    if(leftState > leftStateLast){
      if( posX > 0 ){                                 //check if move is in bounds
        if( maze[posY*2][posX*2-1] == 1 ){            //check if move is valid
          strikeState = true;                         //strike if it isn't
          indicator[0] = indicatorStrike;
          strikeTime = millis();
          leds[ mazeMap[posY*2][posX*2-1] ] = red;
        }else{
          leds[ mainDotsMap[posY][posX] ] = dark;
          posX = posX-1;                              //move if it is
        }
      }
    }

    if(downState > downStateLast){
      if( posY < 5 ){                                 //check if move is in bounds
        if( maze[posY*2+1][posX*2] == 1 ){            //check if move is valid
          strikeState = true;                         //strike if it isn't
          indicator[0] = indicatorStrike;
          strikeTime = millis();
          leds[ mazeMap[posY*2+1][posX*2] ] = red;
        }else{
          leds[ mainDotsMap[posY][posX] ] = dark;
          posY = posY+1;                              //move if it is
        }
      }
    }

    if(rightState > rightStateLast){
      if( posX < 5 ){                                 //check if move is in bounds
        if( maze[posY*2][posX*2+1] == 1 ){            //check if move is valid
          strikeState = true;                         //strike if it isn't
          indicator[0] = indicatorStrike;
          strikeTime = millis();
          leds[ mazeMap[posY*2][posX*2+1] ] = red;
        }else{
          leds[ mainDotsMap[posY][posX] ] = dark;
          posX = posX+1;                              //move if it is
        }
      }
    }

    if( (posX == goalX) && (posY == goalY) ){         //set to solved if true
      solvedState = true;
      indicator[0] = indicatorSolved;
    }
    
    leds[ mainDotsMap[posY][posX] ] = white;          //update the lights
    leds[ mainDotsMap[goalY][goalX] ] = pulse;
    
  }
  
  FastLED.show();

  delay(1);

}
