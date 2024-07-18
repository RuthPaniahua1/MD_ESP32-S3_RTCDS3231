#include "StatisticsValues.h"


StatisticsValues::StatisticsValues()
{

}

void StatisticsValues::SaveVariables(int Variable,int count)
{
    switch (Variable)
    {
        case ESPBeginnings:
            counQRIni = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counQRIni",counQRIni);
            preferences.end();
            break;

          case WifiConfig:
            counIni = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counIni",counIni);
            preferences.end();
            break;

        case QRReadings:
            counQRSoli = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counQRSoli",counQRSoli);
            preferences.end();
            break;

        case QRValid:
            countQRValido = count;
            preferences.begin("Wifi", false);
            preferences.putInt("countQRValido",countQRValido);
            preferences.end();
            break;

        case TotalValidQR:
            countQRValidoT = count;
            preferences.begin("Wifi", false);
            preferences.putInt("countQRValidoT", countQRValidoT);
            preferences.end();
            break;

        case QRInvalid:
            countQRInv = count;
            preferences.begin("Wifi", false);
            preferences.putInt("countQRInv",countQRInv);
            preferences.end();
            break;

        case ServerFailures:
            counErrServ = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counErrServ",counErrServ);
            preferences.end();
            break;
        
        case TotalFailuresServer:
            counErrServT = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counErrServT", counErrServT);
            preferences.end();

        case WifiConnections:
            counWifiC = count;
            preferences.begin("Wifi", false);
            preferences.putInt("counWifiC",counWifiC);
            preferences.end();
            break;
        
        case WifiDisconnections:
            countErrWifi = count;
            preferences.begin("Wifi", false);
            preferences.putInt("countErrWifi",countErrWifi);
            preferences.end();
            break;

        default:
            break;
    }
}

void StatisticsValues::reset()
{
  preferences.begin("Wifi", false);
  //putDateTime(tim);
  preferences.putString("datetime", datetime);
  countQRValido = 0;
  preferences.putInt("countQRValido",countQRValido);
  countQRInv = 0;
  preferences.putInt("countQRInv",countQRInv);
  countErrWifi = 0;
  preferences.putInt("countErrWifi",countErrWifi);
  counQRIni = 0;
  preferences.putInt("counQRIni",counQRIni);
  counQRSoli = 0;
  preferences.putInt("counQRSoli",counQRSoli);
  counWifiC = 0;
  preferences.putInt("counWifiC",counWifiC);
  counErrServ = 0;
  preferences.putInt("counErrServ", counErrServ);
  preferences.end();
  ESP.restart();
}

void StatisticsValues::getCountVariables()
{
  preferences.begin("Wifi", true);
  countQRValido = preferences.getInt("countQRValido",0);
  countQRInv = preferences.getInt("countQRInv",0);
  countErrWifi = preferences.getInt("countErrWifi",0);
  counQRIni = preferences.getInt("counQRIni",0);
  counQRSoli = preferences.getInt("counQRSoli",0);
  counWifiC = preferences.getInt("counWifiC",0);
  counErrServ = preferences.getInt("counErrServ", 0);
  //datetime = preferences.getString("datetime", "");
  counErrServT = preferences.getInt("counErrServT", 0);
  countQRValidoT = preferences.getInt("countQRValidoT",0);
  preferences.end();
}
