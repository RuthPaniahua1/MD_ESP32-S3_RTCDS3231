#ifndef _MIFAREREADERWRITER_H_
#define _MIFAREREADERWRITER_H_

#include <Arduino.h>
#include <SPI.h>
#include "MFRC522.h"
#include "Separator.h"
#include "buzzerledcontrol.h"
#include "TimeOut.h"
#include "ArduinoUniqueID.h"

using namespace std;

class mifareReaderWriter
{
    
    private:

    #define PICC_RX_SIZE      18
    #define PICC_TX_SIZE      16
    #define BLOCK_0 0x00 //Bloque 0 y accedes al bloque sumandole su mismo numero

    #define KEYACCESS_SECTOR_0  0x03 //Sector0
    #define KEYACCESS_SECTOR_1  0x07 //Sector1
    #define KEYACCESS_SECTOR_2  0x0B //Sector2
    

    public:

    //Varbiales de mifare
    String Card;
    byte nuidPICC[4];
    byte UniqueIDArduino[16];
    static TimeOut OkStatusLedTimeout;
    static TimeOut ErrorStatusLedTimeout;
    static TimeOut MifareActivateTimeout;
    uint8_t tv[16] ={2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t ET[16] ={2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int TTE = 30;
    byte TTE_MSB = 0;
    byte TTE_LSB = 0;
    String validatorSN;
    String arrayWS;
    byte EntryDate[16];
    byte ReadDatas[16];
    MFRC522 *RfChip;
    MFRC522::MIFARE_Key key;

    Separator separator;

    buzzerled *mySignal;

    static bool MifareReaderAvailable;

    mifareReaderWriter(uint8_t SS_PIN,uint8_t RST_PIN,int red,int green,int blue,int buzzer);
    
    String returnArrayWS(byte *buffer, byte bufferSize, String validatorSN, String card);

    String returnValidator(byte *buffer, byte bufferSize);

    static void MifareActivateTimeoutHandler(mifareReaderWriter* instance);

    static void OkStatusLedTimeoutHandler(mifareReaderWriter* instance);
    
    static void ErrorStatusLedTimeoutHandler(mifareReaderWriter* instance);
    
    void printHex(byte *buffer, byte bufferSize);

    String returnCard(byte *buffer, byte bufferSize);

    String toUpperCaseString(String input);
    
    void IDArduino();

    void printDec(byte *buffer, byte bufferSize);

    bool MifareReadProcess(byte SectorAccess,byte BlockAccess);

    
};
#endif