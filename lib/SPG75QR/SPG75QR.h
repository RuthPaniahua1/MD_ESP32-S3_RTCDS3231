#ifndef SPG75QR_H
#define SPG75QR_H

#include "QReaderWork.h"
#include "buzzerledcontrol.h"

class SPG75: public QReaderWork
{
    public:

        buzzerled *MySignal();
        
        explicit SPG75();
        void StartBaudRate(int RXPin, int TXPin, int baudRate);
        bool StartQR();
        int ReadQR();
        //uint8_t Read(uint8_t* buffData, uint8_t* length_resp);

    private:
        bool WaitRead = false;
        static const uint8_t startScan[9];
        static const uint8_t scanStarted[7];
        int RXPIN, TXPIN, BAUDRATE;
};

#endif // SPG75_H
