#ifndef STATISTICS_VALUES
#define STATISTICS_VALUES
 
#include <Preferences.h>
#include "RTClib.h"
//#include <WebServiceComunication.h>

using namespace std;

class StatisticsValues
{
    private:
        int counQRIni,counQRSoli,countQRValido,countQRValidoT,countQRInv,counErrServ,counErrServT,counWifiC,countErrWifi, counIni;
        String datetime;
    public:
        StatisticsValues();

        #define ESPBeginnings 0
        #define QRReadings 1
        #define QRValid  2
        #define TotalValidQR 3
        #define QRInvalid 4
        #define TotalFailuresServer 5 
        #define ServerFailures 6
        #define WifiConnections 7
        #define WifiDisconnections 8
        #define WifiConfig 9

        void SaveVariables(int Variable,int count);

        void getCountVariables();
        
        Preferences preferences;

        void reset();

        RTC_DS3231 rtc;

        void putDateTime(String dateTime_);

        String getDateTime();

        //WebServiceComunication myHour;

};

#endif