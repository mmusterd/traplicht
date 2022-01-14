/// Sensor class is responsible for translating the digital input of an SR-HC505
/// into a single pulse on the rising edge, and only when the sensor is 
/// armed. To allow debugging a boot time trigger is always issued
/// 
/// the PIR sensor has the following properties:
/// the output is HIGH at detection and remains HIGH for about 8 seconds
/// datasheet is here: https://www.rapidonline.com/pdf/78-4110_v1.pdf
///
class Sensor {
public:
  explicit Sensor(byte pin); 
  // this function can be polled periodically and will return true on a rising edge
  bool rising();
  // call this function to arm the sensor; unarmed it will never trigger
  void arm();
private:
  void reset();
  bool triggered;
  bool armed;
  byte signalPin;
  bool firstTime;
};
