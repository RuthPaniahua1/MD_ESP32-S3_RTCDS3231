#ifndef WEB_SERVICE_COMUNICATION
#define WEB_SERVICE_COMUNICATION

#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <QReaderWork.h>
#include "RTClib.h"
#include <SPI.h>
#include <StatisticsValues.h>

using namespace std;

class WebServiceComunication
{
    private:
        String response;
        int responseQR;
        String ServerResponse, ServerResponse2;

    public:
        
        char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

        explicit WebServiceComunication();

        void hourBegin();

        String SerializeObject(bool Alive, String SerialNumber, String QR,String ipFija);

        void DeserializeObject(bool alive, String ServerResponse);

        bool HttpClientRequest(bool Alive, String QR,bool Buzzer,String ip,String ipFija, String SerialNumber,int https,String ConectionPort, String ConsultPort);
        
        String URLServer(bool Alive, String IP, int https,String ConectionPort, String ConsultPort);

        void putResponse(String _url);

        String getResponse();

        void putResponseQR(int _url);

        int getResponseQR();

        DateTime Hour();

        Preferences preferences;

        QReaderWork mySeparator;

        RTC_DS3231 rtc;

        //buzzerled* led;

        StatisticsValues Values;
};
#endif