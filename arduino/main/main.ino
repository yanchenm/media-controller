// Using NicoHood's HID library 
// from https://github.com/NicoHood/HID
#include "HID-Project.h"

// Pin definitions (switches)
const int playButton = 8;
const int fwdButton = 7;
const int backButton = 9;

// Pin definitions (encoder)
const int clk = 15;
const int dt = 10;

// Encoder position trackers
int clkLast;
int clkVal;

void setup() {
  // Set up pin modes
  pinMode(playButton, INPUT);
  pinMode(fwdButton, INPUT);
  pinMode(backButton, INPUT);

  pinMode(clk, INPUT);
  pinMode(dt, INPUT);

  // Read CLK initial state
  clkLast = digitalRead(clk);

  // Begin serial connection
  Serial.begin(9600);

  // Begin HID connection
  Consumer.begin();
}

// Debounce control switches
boolean readSwitch(int pin) {
  // Check and debounce buttons
  if (digitalRead(pin) == HIGH) {
    delay(10);

    if (digitalRead(pin) == HIGH) {
      return true;
    }
  }

  return false;
}

// Implement button functionality
void onPress(int pin) {
  switch(pin) {
    case fwdButton:
      Consumer.write(MEDIA_NEXT);
      Serial.print("Next\n");
      delay(300);
      break;
    case playButton:
      Consumer.write(MEDIA_PLAY_PAUSE);
      Serial.print("Play\n");
      delay(300);
      break;
    case backButton:
      Consumer.write(MEDIA_PREVIOUS);
      Serial.print("Previous\n");
      delay(300);
      break;
  }
}

void loop() {
  // Check each switch for press
  for (int i = 7; i < 10; i++) {
    if (readSwitch(i)) {
      onPress(i);
    }
  }

  // Read encoder position
  clkVal = digitalRead(clk);

  // If encoder rotated
  if (clkVal != clkLast) {
    
    // Clockwise rotation
    if (digitalRead(dt) != clkVal) {
      Consumer.write(MEDIA_VOLUME_UP);
      Serial.print("Volume Up\n");
    } 
    
    // Counterclockwise rotation
    else {
      Consumer.write(MEDIA_VOLUME_DOWN);
      Serial.print("Volume Down\n");
    }
  }

  clkLast = clkVal;
}
