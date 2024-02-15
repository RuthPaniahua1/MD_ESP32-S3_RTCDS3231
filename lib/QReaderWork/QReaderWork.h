#ifndef QR_READER_H
#define QR_READER_H

#include <SoftwareSerial.h>
#include <iostream> //Validar
#include <Preferences.h>

using namespace std;

class QReaderWork
{
//public:
//    QRReaderWork();

    public:
        Preferences preferences;
        SoftwareSerial *mySerial;

        explicit QReaderWork();

        virtual bool StartQR(uint8_t* buffData, uint8_t* length_resp);
        
        virtual int ReadQR();

        String* SeparatorIndex(String _array,String Element);

        void VariableConfigure(String [9], int i);

        void putQR(String _qr);

        String getQR();

        boolean bDebug;
        
        //void ipFijaConfig(String _ipFija, String _gateway, String _mascSub);

    protected:
        //bool SelectQR(String QR, int LongQR);
        void StartBR(int RXPin, int TXPin, int baudRate);
        String ReadSerial(int minlength, int maxlength, int maxtime, int intraCharTime, boolean waitfor);
        void WriteSerial(uint8_t* val, uint8_t length);
        //void WriteSerial(uint8_t* val, uint8_t length);

        //uint8_t SendCommand(uint8_t* command,uint8_t length, uint8_t* respTYCD, uint8_t length_resp);
        //uint8_t SendCommand(const uint8_t* command,uint8_t length, uint8_t* respTYCD, uint8_t length_resp);

        bool WaitUntilResp();
        unsigned int maxWait;
        unsigned int msWait;

    private:
        String qr;
        bool WaitRead = false;

};

#endif // QRReaderWork_H
