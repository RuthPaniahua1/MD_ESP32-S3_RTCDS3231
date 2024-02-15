#include "QReaderWork.h"

QReaderWork::QReaderWork()
{
    
}

void QReaderWork::StartBR(int RXPin, int TXPin, int baudRate)
{
    bDebug = false;
    maxWait = 80;
    msWait = 50;
    //maxWait = 20;
    //msWait = 100;
    mySerial = new SoftwareSerial (RXPin, TXPin);
    mySerial->begin (baudRate);
}

bool QReaderWork::StartQR(uint8_t* buffData, uint8_t* length_resp)
{
    return true;
}

int QReaderWork::ReadQR()
{
    return true;
}

void QReaderWork::WriteSerial(uint8_t* val, uint8_t length)
{
    while(mySerial->available() > 0)
    {
        //String qr =  qr = ReadSerial(50,200,1000,2,true);
        mySerial->read();
        mySerial->flush();
    }
    for( int i = 0; i < length; i++)
    {
        mySerial->write (val[i]);
        //Serial.printf ("%#02X ", val[i]);
    }
    if (bDebug)
    {
        Serial.print ("** Enviando a lector QR: ");
        for(int i = 0; i < length; i++)
            //Serial.printf ("%02x", val[i]);
            //Serial.printf ("%#.2X ", val[i]);
        Serial.print ("\r\n");
    }    
}

String QReaderWork::ReadSerial (int minlength, int maxlength, int maxtime, int intraCharTime, boolean waitfor)
{
  //minlength longitud minima esperada
  //maxlength longitud maxima esperada
  //maxtime tiempo maximo a esperar por la respuest
  //intraChartime tiempo de espera entre baytes de la cadeda
  //waitfor espera a obtener la respuesta o solo valida si la respuesta disponible
  char data;
  int count = 0;
  unsigned long endtime = 0;
  String stringReaded = "";
  endtime = maxtime + millis ();
  while (((mySerial->available()>0) | (waitfor && (minlength > count))) && (count < maxlength) && (endtime > millis()))
  {
    if (mySerial->available()>0)
    {
      count ++;
      data = mySerial->read();
      stringReaded += data;
      delay(intraCharTime);
      //Serial.printf("%02x", data);
    }
  }
  //Serial.println("");
  //Serial.printf("%02x", data);
  //Serial.println(stringReaded.length());
  return stringReaded;

}

void QReaderWork::putQR(String _qr)
{
    qr =_qr;
    //Serial.println(qr);
}

String QReaderWork::getQR()
{
    return qr;
}

String* QReaderWork::SeparatorIndex(String _array,String Separador)
{
    static String array[9]={"","","","","","","","",""};
    int stop;
    int LongArray = _array.length();
    String Array=_array;
    int i = 0;
    int index;
    do
    {
        index = Array.indexOf(Separador);
        array[i] = Array.substring(0,index);
        i++;
        Array=Array.substring(index+1,LongArray);
        stop = Array.length();
    } 
    while (stop>=1 & i<9);
    return array;
}

void QReaderWork::VariableConfigure(String array[9],int l)
{
    String password, ssid, ip, h, ipFija, gateway, mascSub,https;
    int j=0;
    int i=9;
    int LongArray;
    int index;
    String key;
    char IniChar;
    for ( int j = 0; j < 9; j++ ) 
    {
        LongArray = array[j].length();
        index = array[j].indexOf(":");
        key = array[j].substring(0,index);
        IniChar = array[j].charAt(0);
        if (key == "P")
            {
                password = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi", false);
                preferences.putString("password",password);
                preferences.end();
            }
        if (key == "S")
            {
                ssid = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi", false);
                preferences.putString("ssid",ssid);
                preferences.end();
            }
        if (key == "H")
            {
                h = array[j].substring(index+1,LongArray);
            }
        if (key == "D")
            {    ip = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi", false);
                preferences.putString("ipWS",ip);
                preferences.end();
            }
        if (key == "I")
            {
                ipFija = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi",false);
                preferences.putString("ip", ipFija);
                preferences.end();
            }
        if (key == "G")
            {   
                gateway = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi",false);
                preferences.putString("gateway", gateway);
                preferences.end();
            }
        if (key == "N")
            {   
                mascSub = array[j].substring(index+1,LongArray);
                preferences.begin("Wifi",false);
                preferences.putString("mascSub", mascSub);
                preferences.end();
            }
        if (key =="SSL")
            {    
                https = array[j].substring(index+1,LongArray);
                int Https = https.toInt();
                preferences.begin("Wifi",false);
                preferences.putInt("Https", Https);
                preferences.end();
            }
       // j++; 
    }

}

bool QReaderWork::WaitUntilResp()
{
    unsigned int cont = 0;
    while(true)
    {
        if(mySerial->available() > 3)
            return true;
        else if(cont >= maxWait)
            break;
        cont++;
        delay(msWait);
    }
    return false;
}

