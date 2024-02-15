#include "SPG75QR.h"
//#include <SoftwareSerial.h>

const uint8_t SPG75::startScan[9] =  {0x7E,0x00,0x08,0x01,0x00,0x02,0x01, 0XAB, 0xCD};
const uint8_t SPG75::scanStarted[7] = {0x02,0x00,0x00,0x01,0x00,0x33,0x31};

SPG75::SPG75() : QReaderWork()
{
}   

void SPG75::StartBaudRate(int RXPin, int TXPin, int baudRate)
{
    StartBR(RXPin,TXPin,baudRate);
}

bool SPG75::StartQR()
{
    uint8_t tempData[9];
    String QRAnsw,qr,QRAnsw2;
    mySerial->flush();
    Serial.flush();

    tempData[0] = startScan[0];
    tempData[1] = startScan[1];
    tempData[2] = startScan[2];
    tempData[3] = startScan[3];//Comando
    tempData[4] = startScan[4];
    tempData[5] = startScan[5];
    tempData[6] = startScan[6];
    tempData[7] = startScan[7];
    tempData[8] = startScan[8];
    WriteSerial(tempData,sizeof(startScan));
    QRAnsw = ReadSerial(7,7,200,2,true);
    boolean error = false;
    for (int i = 0; i<=6;i++)
    {
        //Serial.print(QRAnsw[i]);
        if (QRAnsw[i] != scanStarted[i])
        {
            //Serial.println("Recibi QR confirmacion");
            mySerial->read();
            mySerial->flush();
            error = true;
        }
        else 
        {
            error = false;
        }
    }
    return !error;
}

int SPG75::ReadQR()
{   
        if(mySerial->available()>0)
        {
            String qr = ReadSerial(50,200,500,2,true);
            int LongQR = qr.length();
            putQR(qr);
            return 1;
            
        }
        else return 0;
}

