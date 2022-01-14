#include <FastLED.h>
#include "Looplicht.h"
#include "Sensor.h"

Looplicht::Looplicht() 
  : index(0)
  , onTimer(0)
  , state(Waiting)
  , sensor(nullptr)
  , hue(0) {
  for(auto& led:brightness) {
    led = hue;
  }
}

void Looplicht::SetSensor(Sensor* thesensor) {
  sensor = thesensor;
  sensor->arm();
}

unsigned int Looplicht::GetIndex() {
  return index;
}

void Looplicht::Trigger() {
  if(state == Waiting) {
    Serial.println("W-F");
    hue = random8();
    index = 0;
    state = Filling;
  }
}

void Looplicht::Update() {
  switch(state)
  {
  case Filling:
    if((index+MIN_BRIGHTNESS) > BRIGHTNESS)
    {
      brightness[GetPreviousLed()] = BRIGHTNESS;
      brightness[GetCurrentLed()] = BRIGHTNESS*2;
    }
    else
    {
      brightness[GetCurrentLed()] =  index+MIN_BRIGHTNESS;
    }
    index++;
    if(index >= NUM_LEDS) {
      index = NUM_LEDS-1;
      Serial.println("F-O");
      onTimer = 0;
      state = TurnedOn;
    }
    break;
  case TurnedOn:
    if(onTimer++ > 800)
    {
      onTimer = 0;
      Serial.println("F-f");
      index = 0;
      state = FadingOut;
    }
    break;
  case FadingOut:
    brightness[GetCurrentLed()] = 0;
    index++;
    if(index >= NUM_LEDS) {
      Serial.println("f-W");
      sensor->arm();
      state = Waiting;
    }
    break;
  default:
  case Waiting:
    break;
  }
}

LooplichtState Looplicht::getState()
{
  return state;
}

CRGB Looplicht::GetAt(unsigned int index)
{
  if(brightness[index]==0) return CRGB::Black;
  if(brightness[index]<22)  return CHSV(hue, 127, brightness[index]);
  if(brightness[index]<34)  return CHSV(hue, 191, brightness[index]);
  return CHSV(hue, 255, brightness[index]);
}

int DownLooplicht::GetCurrentLed() {
  return GetIndex();
}

int UpLooplicht::GetCurrentLed() {
  return (NUM_LEDS-1)-GetIndex();
}

int DownLooplicht::GetPreviousLed() {
  return GetIndex()-1;
}

int UpLooplicht::GetPreviousLed() {
  return (NUM_LEDS-1)-(GetIndex()-1);
}
