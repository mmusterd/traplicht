#include <FastLED.h>
#include "Sensor.h"
#include "Looplicht.h"

// the Up and Down PIR sensor input numbers
#define SIGNAL_PIN_UP 3    
#define SIGNAL_PIN_DOWN 2

// the WS2812B LED array is connected here

#define DATA_PIN 6


CRGB leds[NUM_LEDS];

DownLooplicht downLooplicht;
UpLooplicht upLooplicht;
Sensor upSensor(SIGNAL_PIN_UP);
Sensor downSensor(SIGNAL_PIN_DOWN);

void setup() {
  Serial.begin(9600);
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  pinMode(SIGNAL_PIN_UP, INPUT);
  pinMode(SIGNAL_PIN_DOWN, INPUT);
  pinMode(DATA_PIN, OUTPUT);
  // if you omit the following two initialisation lines the sensors are continuously HIGH
  digitalWrite(SIGNAL_PIN_UP, LOW);
  digitalWrite(SIGNAL_PIN_DOWN, LOW);
  random16_add_entropy(analogRead(0));

  // marry the sensors to the Looplicht instances
  downLooplicht.SetSensor(&downSensor);
  upLooplicht.SetSensor(&upSensor);
}

void loop() {
  
  // 60 Leds/meter --> filling speed of 0.8m / second. Perfect for ascending a staircase
  EVERY_N_MILLISECONDS( 20 ) { 
    downLooplicht.Update();
    upLooplicht.Update();
    
    //the LED strip shows the sum of both instances for each LED
    for(int i=0; i<NUM_LEDS;++i) {
      leds[i] = downLooplicht.GetAt(i) + upLooplicht.GetAt(i);
    }
    // if both up&down are on at the same time, it's party time :-)
    if(((downLooplicht.getState()==TurnedOn) &&
      (upLooplicht.getState()==TurnedOn)))   {
      if( random8() < 100) {
        leds[ 80 + random16(NUM_LEDS-160) ] += CRGB::White;
      }
    }
    FastLED.show();
  }
  
  // polling for sensors every second is plenty fast enough
  EVERY_N_SECONDS( 1 ) { 
    Serial.println(".");
    // special guard to prevent downfilling when you're upstairs
    // or vice versa
    if(!((downLooplicht.getState()==TurnedOn) ||
         (upLooplicht.getState()==TurnedOn))) {
      if(upSensor.rising()) {
        upLooplicht.Trigger();
      }
      if(downSensor.rising()) {
        downLooplicht.Trigger();
      }
    }
  }
}
