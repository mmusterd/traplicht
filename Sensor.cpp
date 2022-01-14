#include <FastLED.h>
#include "Sensor.h"

Sensor::Sensor(byte pin) 
  : triggered(false)
  , armed(false)
  , signalPin(pin)
  , firstTime(true)
{
}
bool Sensor::rising() {
  if(firstTime)
  {
    firstTime = false;
    armed = false;
    return true;
  }
  if(digitalRead(signalPin)==HIGH) {
    if(!armed) {
      // !a && !t
      // !a && t
      Serial.println("[I]!A");
      return false;
    }
    if(triggered) {
      // armed and previously triggered: ignore
      Serial.println("[I]T");
      return false;
    }
    // armed and first trigger; GO!
    Serial.println("Activating");
    triggered = true;
    armed = false;
    return true;
  } 
  reset();
  return false;
}

void Sensor::arm() {
  if(!armed)
  {
    Serial.println("A[0->1]");
    armed = true;
  }
}
void Sensor::reset() {
  if(triggered)
  {
    Serial.println("T[1->0]");
    triggered = false;
  }
}
