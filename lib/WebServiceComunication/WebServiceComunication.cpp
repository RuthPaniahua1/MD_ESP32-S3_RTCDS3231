#include "WebServiceComunication.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h> 
#include <Preferences.h>
#include <QReaderWork.h>

WebServiceComunication::WebServiceComunication()
{
    
}

String WebServiceComunication::SerializeObject(bool Alive, String SerialNumber, String QR,String ipFija)
{
    String SerializeObjectData = "";
    if (Alive)
    {
        JsonDocument docs; 
        docs["funcion"]="alive"; 
        docs["valores"]["ip"] = (ipFija);
        docs["valores"]["modulo"]=SerialNumber; //N/S de ESP32
        docs["valores"]["Registro"]="2200-01-01 00:00:00.000";
        serializeJson(docs, SerializeObjectData);
       
    }
    else
    {
        JsonDocument doc;
        doc["qrcode"]=QR;
        //doc["token"]=token;
        doc["signature"]=SerialNumber;
        SerializeObjectData = "";
        serializeJson(doc, SerializeObjectData);
    }
    Serial.println(SerializeObjectData);
    return SerializeObjectData;
}

void WebServiceComunication::DeserializeObject(bool alive, String ServerResponse)
{
    String result;
    if(alive)
    {
        JsonDocument docd;
        DeserializationError error = deserializeJson(docd, ServerResponse);
        if (error) 
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            //return "error";
        }
        putResponse(ServerResponse);
        //return result;
    }
    else 
    {
        JsonDocument docd;
        DeserializationError error = deserializeJson(docd, ServerResponse);
        if (error) 
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            //return "error";
        }
        const char* resul;
        resul = docd["Result"];
        String tok(resul);
        result =tok;
        putResponseQR(result.toInt());
    }
}

void WebServiceComunication::putResponse(String _response)
{
    response = _response;
}

String WebServiceComunication::getResponse()
{
    return response;
}

void WebServiceComunication::putResponseQR(int _response)
{
    responseQR = _response;
}

int WebServiceComunication::getResponseQR()
{
    return responseQR;
}

String WebServiceComunication::URLServer(bool Alive, String IP, int https,String ConectionPort, String ConsultPort)
{
    String ip = IP;
    // preferences.begin("Wifi", true);
    // ip = preferences.getString("ip", "");
    // preferences.end();
    String URL;
    if (Alive)
    {
        if (https == 1)
        {
        URL = "https://"+ip+":"+ConectionPort+"/api/ConexionModulo";
        }
        if (https ==0)
        {
        URL = "http://"+ip+":"+ConectionPort+"/api/ConexionModulo";
        }
    }
    else
    {
        if (https == 1)
        {
        URL = "https://"+ip+":"+ConectionPort+"/api/DescuentoModulo";
        }
        if (https == 0)
        {
        URL = "http://"+ip+":"+ConectionPort+"/api/DescuentoModulo";
        }
    }
    Serial.print("URL: ");
    Serial.println(URL);
    return URL;
}

bool WebServiceComunication::HttpClientRequest(bool Alive, String QR,bool Buzzer,String ip,String ipFija,String SerialNumber,int https,String ConectionPort, String ConsultPort)
{
    String ServerSerializeObject, ServerR;
    String URL, ServerResponse;
    int response;
    if (WiFi.status() == WL_CONNECTED) 
    {
        //WiFiClientSecure *client = new WiFiClientSecure;
        //if(client) 
        //{
            //client->setInsecure();
            HTTPClient http;
            ServerSerializeObject = SerializeObject(Alive,SerialNumber,QR,ipFija);
            URL = URLServer(Alive,ip,https,ConectionPort,ConsultPort);
            http.begin(/**client,*/ URL);
            http.addHeader("Content-Type", "application/json");
            response = http.POST(ServerSerializeObject);
            Serial.print("Server Response: ");
            Serial.println(response);
            if(response == 200)
            {
                //led->buzzerOFF();
                ServerResponse = http.getString();
                Serial.println(ServerResponse);
                DeserializeObject(Alive,ServerResponse);
                return true;
            }
            else
            {
                return false;
            }
            http.end();
        //}
    }
    else
    {
        return false;
        ESP.restart();
    }
}

String WebServiceComunication::Hour()
{
  String* Horarray;
  String* fechaarray;
  String SR;
  String DataTime;
  int LongSR;
  ServerResponse2 = getResponse();
  LongSR = ServerResponse2.length();
  ServerResponse = ServerResponse2.substring(1,LongSR-1);
  Horarray = mySeparator.SeparatorIndex(ServerResponse,":");
  //Serial.println(ServerResponse);
  for ( int h = 0; h < 4; h++ ) 
      {
        Horarray[h];
      }
  int seconds = Horarray[2].toInt();
  int minutes = Horarray[1].toInt();
  int hours = Horarray[0].toInt();
  SR = ServerResponse.substring(14,LongSR-1);
  //Serial.println(SR);
  fechaarray = mySeparator.SeparatorIndex(SR,"/");
  for ( int d = 0; d < 3; d++ ) 
      {
        Horarray[d];
      }
 
  int day = fechaarray[2].toInt();
  int month = fechaarray[1].toInt();
  int year = fechaarray[0].toInt();
  rtc.setTime(seconds,minutes,hours,day,month,year);
  //rtc.setTime(30, 24, 15, 17, 1, 2021);
  DataTime = rtc.getTime("%d/%m/%Y %H:%M:%S");
  struct tm timeinfo = rtc.getTimeStruct();
  return DataTime;
}