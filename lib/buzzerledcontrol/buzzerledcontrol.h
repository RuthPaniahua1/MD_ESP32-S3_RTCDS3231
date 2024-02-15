#ifndef _BUZZERLEDCONTROL_H_
#define _BUZZERLEDCONTROL_H_

#include <Arduino.h>

using namespace std;

class buzzerled
{
public:
    buzzerled(int red, int green, int blue, int buzzer);

    #define OFF 0
    #define RED 1
    #define GREEN 2
    #define BLUE 3
    #define YELLOW 4
    #define PINK 5
    #define WHITE 6
    #define CIAN 7

    void setPinOut (int red, int green, int blue, int buzzer);

    void setBuzzerPin (int buzzerPin);

    void ledON (int COLOR);

    void ledOFF ();

    boolean isledON ();

    void buzzerON ();

    void buzzerOFF ();

    void buzzerFlip ();

    boolean isbuzzerON();

    void flashLed (int cicles, int color, int timeOn, int timeOff, boolean buzzer);

    void delayLed (int timeToDelay, int color, int timeON, int timeOFF, boolean buzzer);

    
private:
    int pin_red;
    int pin_green;
    int pin_blue;
    int pin_buzzer;
    
    boolean is_ledON = false;
    boolean is_buzzerON = false;
};

#endif