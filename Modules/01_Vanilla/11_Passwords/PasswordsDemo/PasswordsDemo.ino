// KTANE Passwords Module (algorithm test) by Nicholas Schwankl
// This code only outputs to the Serial Monitor and is only designed to operate with English at the moment.
// Keep Talking and Nobody Explodes holds all rights to their game. Please buy a copy from https://keeptalkinggame.com/ before making these modules.

#include <Arduino.h>

//********** UPLOAD OPTIONS **********//

#define DEBUG true // Enables serial monitor



//********** VARIABLES **********//

bool allOptions[5][26];                           // Array to track what letters are possible in each position
int lettersLeft[5] = {26,26,26,26,26};            // Array to track number of True values in allOptions[]
String allWords[35] = {                           // Array of all possible words being considered (chosen or made impossible)
  "ABOUT", "AFTER", "AGAIN", "BELOW", "COULD",
  "EVERY", "FIRST", "FOUND", "GREAT", "HOUSE",
  "LARGE", "LEARN", "NEVER", "OTHER", "PLACE",
  "PLANT", "POINT", "RIGHT", "SMALL", "SOUND",
  "SPELL", "STILL", "STUDY", "THEIR", "THERE",
  "THESE", "THING", "THINK", "THREE", "WATER",
  "WHERE", "WHICH", "WORLD", "WOULD", "WRITE"
};
const int wordCount = 35;                         // Number of items within the allWords[] array
const int numOptions = 6;                         // Number of letters that will be made options in final output
char finalOptions[5][6];                          // Array to track the final output



//********** USER FUNCTIONS **********//

void resetEverything() {                                // Prepares arrays for new password generation
  for(int i=0; i<5; i++){
    for(int j=0; j<26; j++){
      allOptions[i][j] = true;                          // Sets all letters in all positions as possible options
    }
  }
  for(int i=0; i<5; i++){
    lettersLeft[i] = 26;                                // Resets lettersLeft totals
  }
  for(int i=0; i<wordCount-1; i++){                     // Shuffles all words in allWords array using Durstenfeld Shuffle
    int swapPos = random(0,wordCount-i);                // * Pick position at random from range to be swapped with far right
    String temp = allWords[wordCount-(i+1)];            // * Store rightmost item temporarily
    allWords[wordCount-(i+1)] = allWords[swapPos];      // * Replace rightmost position with randomly chosen position
    allWords[swapPos] = temp;                           // * Replace randomly chosen position with stored item
  }                                                     // * Repeat, removing an item from the right every cycle, until complete
}

bool isWordPossible(String testWord) {                              // Checks if a word is currently possible by comparing against allOptions
  bool letterNotPossible = false;
  for(int i=0; i<5; i++){                                           // For every position
    letterNotPossible |= !allOptions[i][testWord.charAt(i)-'A'];    // check if the letter of the test word is still an option
  }
  return !letterNotPossible;                                        // If entire word is possible return True
}

bool removeLetters(){                                   // Removes letters as options from random positions until only the first is possible
  #if DEBUG
    Serial.println("Removing Letters");
  #endif
  bool everythingIsFlamesAllIsLost = false;             // This will be set to true if something prevents a word from being made impossible
  for(int i=1; i<wordCount; i++){
    #if DEBUG
      Serial.print(" • "+allWords[i]+" || ");
    #endif
    if(isWordPossible(allWords[i])){                    // Only remove a letter if the word is currently possible
      int order[5] = {0,1,2,3,4};                       // Create an array for tracking the order of removal candidates
      for(int j=0; j<4; j++){                           // Shuffle the array using Durstenfeld Shuffle
        int swapPos = random(0,5-j);                    // *
        int temp = order[4-j];                          // *
        order[4-j] = order[swapPos];                    // *
        order[swapPos] = temp;                          // *
      }
      int loopNum = 0;
      while(loopNum<5){                                 // Go through the options in order until success (10) or all options are attempted (5)
        int letterPos = order[loopNum];
        #if DEBUG
          Serial.print(String(order[loopNum])+" - "+char(allWords[0].charAt(letterPos))+":"+char(allWords[i].charAt(letterPos))+" | ");
        #endif
        // If the selected word's letter conflicts with the "correct word" or if removing the letter would remove too many options in that position
        if((allWords[i].charAt(letterPos) != allWords[0].charAt(letterPos)) || (lettersLeft[letterPos] <= numOptions)){
          allOptions[letterPos][allWords[i].charAt(letterPos)-'A'] = false;     // remove the letter from allOptions
          lettersLeft[letterPos]--;                                             // remove one from the allOptions length tracker
          loopNum = 9;                                                          // exit the loop
        }
        loopNum++;
      }
      everythingIsFlamesAllIsLost |= (loopNum==5)?true:false;                   //A successful loop will exit with 10, a fail will exit with 5
    }
    #if DEBUG
      Serial.println();
    #endif
  }
  return(!everythingIsFlamesAllIsLost);                 // Return true if successful
}

void generateFinalOptions(){                                          // Reduce the allOptions into the desired number and shuffle them
  for(int i=0; i<5; i++){                                             // Repeat for every letter position
    finalOptions[i][0] = allWords[0].charAt(i);                       // Add the correct word's letter to the set and remove it from future loops
    allOptions[i][allWords[0].charAt(i)-'A'] = false;                 // *
    lettersLeft[i]--;                                                 // *
    for(int j=1; j<numOptions; j++){                                  // For every remaining position:
      int currentPos = 0;                                                 // Create a tracker for position within allOptions array
      while(!allOptions[i][currentPos]){currentPos++;}                    // Move the tracker to the first possible option
      for(int k=0; k<random(0,lettersLeft[i]); k++){                      // Select a random number from 0 to the number of possible options
        currentPos++;                                                     // Move forward that number of times
        while(!allOptions[i][currentPos]){currentPos++;}                  // Ensuring the tracker lands on a valid position for every count
      }
      finalOptions[i][j] = 'A'+currentPos;                            // Add the selected letter to the final set and remove it from future loops
      allOptions[i][currentPos] = false;                              // *
      lettersLeft[i]--;                                               // *
    }
    for(int j=0; j<numOptions-1; j++){                                // Shuffle the chosen options using Durstenfeld Shuffle
      int swapPos = random(0,numOptions-j);                           // *
      char temp = finalOptions[i][numOptions-(j+1)];                  // *
      finalOptions[i][numOptions-(j+1)] = finalOptions[i][swapPos];   // *
      finalOptions[i][swapPos] = temp;                                // *
    }
  }
}

#if DEBUG
  void printOptions(){                      // Prints out allOptions[][] as horizontal alphabets with removed letters as spaces
    for(int i=0; i<5; i++){
      Serial.print("|");
      for(int j=0; j<26; j++){
        Serial.print(char(allOptions[i][j]?'A'+j:' '));
      }
      Serial.println("| "+String(lettersLeft[i]));
    }
  }

  void printFinalOptions(){                 // Prints out finalOptions as vertical columns with the correct answer's letters marked by dots
    for(int j=0; j<numOptions; j++){
      for(int i=0; i<5; i++){
        char letter = finalOptions[i][j];
        Serial.print("|");
        if(letter==allWords[0].charAt(i)){
          Serial.print("•");
          Serial.print(letter);
          Serial.print("•");
        }else{
          Serial.print(" ");
          Serial.print(letter);
          Serial.print(" ");
        }
      }
      Serial.println("|");
    }
  }
#endif



//********** SETUP **********//

void setup() {                        // This process can be moved into a function for multiple uses on the same boot cycle
  
  #if DEBUG
    Serial.begin(115200);
    delay(500);
  #endif

  resetEverything();                  // Must be run before each password generation process, correct word is item 0 after shuffle

  #if DEBUG
    Serial.println();
    Serial.print("The selected word is: ");
    Serial.println(allWords[0]);
    Serial.println();
  #endif

  bool success = removeLetters();     // Removes letters until only correct option remains and reports if process was successful

  #if DEBUG
    Serial.println(success ? "SUCCESS" : "FAILURE");
    Serial.println();
    
    printOptions();
    Serial.println();
  #endif

  generateFinalOptions();             // Reduces the options down to the desired number, stores them to finalOptions[][], and shuffles them

  #if DEBUG
    printFinalOptions();
  #endif
  
}



//********** LOOP **********//

void loop() {

}
