#include <time.h>
#include <Arduino.h>

class Time {
  //private:
    unsigned long currTime, initTime, elapsed;
    unsigned long extra;
    unsigned int seconds, minutes;

    
  
  unsigned int getSeconds()
  {
    recharge();
    return (unsigned int) ((elapsed / 1000) % 60);
  }

  unsigned int getMinutes()
  {
    recharge();
    return (unsigned int) (elapsed / 60000) % 60;
  }
  
  void recharge()
  {
    currTime = millis();
    elapsed = extra + currTime - initTime;
  }
  public:

  void initalizeTime()
  {
    currTime = millis();
    initTime = millis();
    elapsed = extra + currTime - initTime;
  }

  unsigned int LEDTime()
  {
    recharge();
    return ((getMinutes() * 100) + getSeconds());
  }

  void addMinutes(unsigned long mins)
  {
    extra += (mins * 60000);
  }

  void addSeconds(unsigned long secs)
  {
    extra += (secs * 1000);
  }

  void reset()
  {
    currTime = millis();
    initTime = millis();
    elapsed = currTime - initTime;
  }

  
  String fLEDTime()
  {
    recharge();
    int number = LEDTime();
    String temp = String(number);
/*    for(int i = 0; i < (4-temp.length()); i++)
    {
      temp = "0" + temp;
    } */
    return temp;
  }
};