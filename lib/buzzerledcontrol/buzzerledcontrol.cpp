#include "buzzerledcontrol.h"
//#include <SoftwareSerial.h>
buzzerled::buzzerled(int red, int green, int blue, int buzzer)
{
  pin_red = red;
  pin_green = green;
  pin_blue = blue;
  pin_buzzer = buzzer;
  setPinOut (pin_red,pin_green,pin_blue,pin_buzzer);
}

void buzzerled::setPinOut (int red, int green, int blue, int buzzer)
{
  pinMode (red, OUTPUT);
  pinMode (green, OUTPUT);
  pinMode (blue, OUTPUT);
  pinMode (buzzer, OUTPUT);
}

void buzzerled::ledON (int COLOR)
{
  is_ledON = true;
  switch (COLOR){            
    case RED:
      digitalWrite (pin_red, LOW);
      digitalWrite (pin_green, HIGH);
      digitalWrite (pin_blue, HIGH);
      break;
    case GREEN:
      digitalWrite (pin_red, HIGH);
      digitalWrite (pin_green, LOW);
      digitalWrite (pin_blue, HIGH);
      break;
    case BLUE:
      digitalWrite (pin_red, HIGH);
      digitalWrite (pin_green, HIGH);
      digitalWrite (pin_blue, LOW);
      break;
    case YELLOW:
      digitalWrite (pin_red, LOW);
      digitalWrite (pin_green, LOW);
      digitalWrite (pin_blue, HIGH);
      break;
    case PINK:
      digitalWrite (pin_red, LOW);
      digitalWrite (pin_green, HIGH);
      digitalWrite (pin_blue, LOW);
      break;
    case WHITE:
      digitalWrite (pin_red, LOW);
      digitalWrite (pin_green, LOW);
      digitalWrite (pin_blue, LOW);
      break;
    case CIAN:
      digitalWrite (pin_red, HIGH);
      digitalWrite (pin_green, LOW);
      digitalWrite (pin_blue, LOW);
      break;
    case OFF:
      digitalWrite (pin_red, HIGH);
      digitalWrite (pin_green, HIGH);
      digitalWrite (pin_blue, HIGH);
      is_ledON = false;
    break;
    default:
      break;
  }
}

void buzzerled::ledOFF ()
{
  is_ledON = false;
  digitalWrite (pin_red, HIGH);
  digitalWrite (pin_green, HIGH);
  digitalWrite (pin_blue, HIGH);
}

boolean buzzerled::isledON (){
    return is_ledON;
}

void buzzerled::buzzerON ()
{
  digitalWrite (pin_buzzer, HIGH);
  is_buzzerON = true;
}

void buzzerled::buzzerOFF ()
{
  digitalWrite (pin_buzzer, LOW);
  is_buzzerON = false;
}

void buzzerled::buzzerFlip ()
{
  if (is_buzzerON)
    buzzerOFF();
  else
    buzzerON();
}

boolean buzzerled::isbuzzerON(){
    return is_buzzerON;
}

void buzzerled::flashLed (int cicles, int color, int timeOn, int timeOff, boolean buzzer)
{
  if (buzzer) pinMode (pin_buzzer, OUTPUT);
  for (int count = 0; count < cicles; count ++)
  {
    ledON (color);
    if (buzzer) buzzerON();
    delay (timeOn);
    ledON (OFF);
    if (buzzer) buzzerOFF();
    delay (timeOff);
  }
}

void buzzerled::delayLed (int timeToDelay, int color, int timeON, int timeOFF, boolean buzzer)
{
  int cicleTime= timeON + timeOFF;
  int cicles = int(timeToDelay / (cicleTime));
  if (buzzer) pinMode (pin_buzzer, OUTPUT);
  for (int count = 0; count < cicles; count ++)
  {
    ledON (color);
    if (buzzer) buzzerON();
    delay (timeON);
    ledON (OFF);
    if (buzzer) buzzerOFF();
    delay (timeOFF);
  }
  int rest = (timeToDelay - (cicles * (cicleTime)));
  if (rest >0)
  {
    ledON (color);
    if (buzzer) buzzerON();
    if (rest > timeON)
    {
      delay (timeON);
      ledON (OFF);
      if (buzzer) buzzerOFF();      
      delay (rest -timeON);
    }
    else
      delay (rest);
  }
  ledON (OFF);
  if (buzzer) buzzerOFF();
}


