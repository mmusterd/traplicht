/// Looplicht class is responsible when triggered for the gradual 'filling' 
/// of the LED strip
/// and for slowly fading out afterwards. When off again, its sensor is armed
/// and ready again.
/// state is exposed to allow to filter out triggers based on this.
const int NUM_LEDS        = 271; // maximum on an arduino nano is 328
const byte BRIGHTNESS     = 80; // max 127
const byte MIN_BRIGHTNESS = 24; // below this value colors are not accurate

class Sensor;
enum LooplichtState {
  Waiting,
  Filling,
  TurnedOn,
  FadingOut
};

class Looplicht
{
public:
  Looplicht();

  void SetSensor(Sensor* thesensor);
  void Trigger();
  void Update();
  CRGB GetAt(unsigned int index);
  LooplichtState getState();

protected:
  // function must be overridden to get a special "walk" over the LED array
  virtual int GetCurrentLed() = 0;
  virtual int GetPreviousLed() = 0;
  // function for getting the index to allow index member to be private
  unsigned int GetIndex();
private:
  unsigned int index;
  unsigned int onTimer;
  LooplichtState state;
  Sensor* sensor;
  byte brightness[NUM_LEDS];
  byte hue;
};

class DownLooplicht : public Looplicht {
public:
  DownLooplicht() = default;
  int GetCurrentLed() override;
  int GetPreviousLed() override;
};

class UpLooplicht : public Looplicht {
public:
  UpLooplicht() = default;
  int GetCurrentLed() override;
  int GetPreviousLed() override;
};
