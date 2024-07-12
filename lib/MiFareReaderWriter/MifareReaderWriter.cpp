#include "mifareReaderWriter.h"

// TimeOut mifareReaderWriter::OkStatusLedTimeout;
// TimeOut mifareReaderWriter::ErrorStatusLedTimeout;
// TimeOut mifareReaderWriter::MifareActivateTimeout;
bool mifareReaderWriter::MifareReaderAvailable = false;

// Constructor
mifareReaderWriter::mifareReaderWriter(uint8_t SS_PIN,uint8_t RST_PIN,int red,int green,int blue,int buzzer) // Inicializa RfChip con los pines SS y RST
{
  RfChip = new MFRC522(SS_PIN,RST_PIN);
  mySignal=new buzzerled(red,green,blue,buzzer);
  RfChip->PCD_Init();
}

String mifareReaderWriter::returnArrayWS(byte *buffer, byte bufferSize, String validatorSN, String card)
{
    String dateTime, Day, Month, Year, Hour, Min, Sec, ME;
    String Provedor = "01";
    String tarjeta = card;
    String paridad = "0000";
    String Tienda ="1234";
    String returnarrayWS = "";
    String index = " ";
    String* DateTime;
    for(byte i = 0; i < bufferSize; i++)
    {
        dateTime += (buffer[i] < 10 ? " 0" : " ");
        dateTime += String(buffer[i], DEC);
    }
    DateTime = separator.SeparatorIndex(dateTime,index);
    for ( int h = 0; h < 8; h++ ) 
    {
     DateTime[h];
    }
    Day = DateTime[1];
    Month = DateTime[2];
    Year = DateTime[3];
    Hour = DateTime[4];
    Min = DateTime[5];
    Sec = DateTime[6];
    ME = DateTime[7];
    if (ME.length()<3)
    {
        for(int i = ME.length(); i<=2;i++)
        ME= "0"+ME;
    }
    Serial.print("ME: ");
    Serial.println(ME);
    Serial.print("tarjeta: ");
    Serial.println(tarjeta);
    returnarrayWS = Provedor + Year + Month + Day + Hour + Min + Sec + Tienda + ME + tarjeta + validatorSN + paridad;
    return returnarrayWS;
}

String mifareReaderWriter::returnValidator(byte *buffer, byte bufferSize)
{
    String validator;
    String Validator;
    for (byte i = 0; i < bufferSize; i++) 
    {
        validator += (buffer[i] < 0x10 ? "0" : "");
        validator += String(buffer[i], HEX);
    }
    validator = toUpperCaseString(validator);
    Validator = (validator.substring(0,8))+ (validator.substring(16,24));
    return Validator;
}

void mifareReaderWriter::MifareActivateTimeoutHandler(mifareReaderWriter* instance)
{
    mifareReaderWriter::MifareReaderAvailable = true;
    if (MifareReaderAvailable)
    instance->mySignal->ledON(BLUE); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
 
}

void mifareReaderWriter::OkStatusLedTimeoutHandler(mifareReaderWriter* instance)
{
    //analogWrite(Buzzer, 0);
    instance->mySignal->buzzerOFF(); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
    //MifareActivateTimeout.attach(&MifareActivateTimeoutHandler, TIME_SLEEP_MIFARE);
    MifareActivateTimeout.timeOut(400, mifareReaderWriter::MifareActivateTimeoutHandler); //delay 10000=10seg, callback function  

}


void mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler(mifareReaderWriter* instance)
{
    Serial.println("ErrorStatusLedTimeout"); 
    instance->mySignal->buzzerOFF(); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
    MifareActivateTimeout.timeOut(400, mifareReaderWriter::MifareActivateTimeoutHandler); //delay 10000=10seg, callback function  
}

void mifareReaderWriter::printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++) 
    {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    }
}

String mifareReaderWriter::returnCard(byte *buffer, byte bufferSize)
{
    String card;
    String* Card;
    String ArrayCard;
    String index = " ";
    for (byte i = 0; i < bufferSize; i++) 
    {
        card += (buffer[i] < 0x10 ? "0" :  " ");
        card += String(buffer[i], HEX);
    }
    card = toUpperCaseString(card);
    Card = separator.SeparatorIndex(card,index);
    for ( int h = 0; h < 8; h++ ) 
      {
        Card[h];
      }
    ArrayCard = Card[4] + Card[3] + Card[2] + Card[1];
    if(ArrayCard.length()>8)
    {
        ArrayCard = Card[3] + Card[2] + Card[1];
    }
    return ArrayCard;
}

String mifareReaderWriter::toUpperCaseString(String input) 
{
  String result = "";
  for (int i = 0; i < input.length(); i++) {
    result += char(toupper(input.charAt(i)));
  }
  return result;
}

void mifareReaderWriter::IDArduino()
{
    memcpy(UniqueIDArduino, UniqueID8, 4);//copia los primeros 4 en UniqueIDArduino
    memcpy(UniqueIDArduino+8, UniqueID8+4, 4);//UniqueID8+4 recorre 4 y copia los 2dos 4 a UniqueIDArduino+8 a partir de la posicion 8
}

void mifareReaderWriter::printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++) 
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}

bool mifareReaderWriter::MifareReadProcess(byte SectorAccess,byte BlockAccess)
{
    if(!RfChip->PICC_IsNewCardPresent())
    {    
        return false;
    } 
    if (!RfChip->PICC_ReadCardSerial())
    {
        return false;
    }
    //Debug.printf("PICC Card UID: "); 
    for (byte i = 0; i < 4; i++) 
    {
        Serial.println(" Card present");
        nuidPICC[i] = RfChip->uid.uidByte[i];
    }
    Serial.print(F("UID::"));
    printHex(RfChip->uid.uidByte, RfChip->uid.size);
    Card = returnCard(RfChip->uid.uidByte, RfChip->uid.size);
    //printDec(RfChip->uid.uidByte, RfChip->uid.size);
    Serial.println("Card: ");
    Serial.println(Card);
    Serial.println();
    Serial.println(" Card present");
     //Card type
    uint8_t piccType = RfChip->PICC_GetType(RfChip->uid.sak);
    // Serial.println(RfChip->PICC_GetTypeName(piccType));

    for(int mainCounter = 0; mainCounter < MFRC522::MF_KEY_SIZE; mainCounter++)
      key.keyByte[mainCounter] = 0xFF;   
    //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    //Serial.println(); 

    MFRC522::StatusCode status;
    //Authenticate using key A
    //Serial.println("Authenticating using key A...");
    MFRC522::PICC_Command piccAuthKey = MFRC522::PICC_CMD_MF_AUTH_KEY_A;
    uint8_t piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, SectorAccess, &key, &RfChip->uid);
    //status = (MFRC522::StatusCode) RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, PICC_TRAILER_BLOCK_1, &key, &(RfChip->uid));
    if (piccStatus != MFRC522::STATUS_OK) 
    {
      Serial.println("Authenticate key A::failed: ");
      Serial.println("");
      RfChip->PCD_StopCrypto1();
      RfChip->PICC_HaltA();
      if(!RfChip->PICC_IsNewCardPresent())
        {
          //Debug.printf("NO Card Detected\r\n");
          return false;
        }
      if (!RfChip->PICC_ReadCardSerial())
        {
           //Debug.printf("Cannot read serial number\r\n");
          return false;
        }
        
      //Debug.printf("PICC Card UID: ");
      for (byte i = 0; i < 4; i++) 
        {
          nuidPICC[i] = RfChip->uid.uidByte[i];
        }

      piccType = RfChip->PICC_GetType(RfChip->uid.sak);

      for(int mainCounter = 0; mainCounter < MFRC522::MF_KEY_SIZE; mainCounter++)
          key.keyByte[mainCounter] = 0xFF;
      
      piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, SectorAccess, &key, &RfChip->uid);

      if(piccStatus != MFRC522::STATUS_OK)
      {
        Serial.println("Authenticate key B::failed: ");
        Serial.println("");
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/GreenReadingStatusLed/BlueOkStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
      }
      //Serial.println("Authenticating using key B...");
      piccAuthKey = MFRC522::PICC_CMD_MF_AUTH_KEY_B;
    }
    else
    //Serial.println("Authentification type -> A");

    memset(UniqueIDArduino, 0 , 16); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //UNIQUE ID ARDUINO
    IDArduino();
   
    uint8_t piccReadBuffer[PICC_RX_SIZE];
    uint8_t piccReadBufferSize = sizeof(piccReadBuffer);
    piccStatus = RfChip->PCD_Authenticate(piccAuthKey, SectorAccess, &key, &RfChip->uid);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Authentification block[%d] -> %s\r\n\r\n", PICC_TRAILER_BLOCK_1, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
    }
    piccStatus = RfChip->MIFARE_Read(BlockAccess, piccReadBuffer, &piccReadBufferSize);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Reading Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }  

    //Serial.print(F("ReadDataS1B0::"));
    //printHex(piccReadBuffer, 16);
    //Serial.println("");       
  
    //Serial.print(TTE_MSB, HEX);
    //Serial.print(" ");  
    //Serial.println(TTE_LSB, HEX);      
    //Serial.println(""); 
    //int TolEntryCard = (( TTE_MSB << 7) |  TTE_LSB);
    //Serial.println(TolEntryCard, HEX);   
  
    TTE_MSB = ((TTE >> 8) & 0xFF);
    TTE_LSB = (TTE & 0xFF);
    memset(EntryDate, 0 , 16);
    memcpy(EntryDate, piccReadBuffer, 16);
         
    EntryDate[7]=TTE_MSB;
    EntryDate[8]=TTE_LSB;

    piccStatus = RfChip->MIFARE_Write(BlockAccess, (uint8_t *)&EntryDate[0], 16);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Writing Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA(); 
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
    }

    Serial.print(F("EntryDate&TTE:"));
    printHex(EntryDate, 16);
    Serial.println("");
 
    /*ACCESO SECTOR 2*/

    //Serial.print(F("S2B0"));
    //KEYACCESS_SECTOR_1,BLOCK_0 + 4

    SectorAccess = KEYACCESS_SECTOR_2;  
    BlockAccess = BLOCK_0 + 8;

    //MFRC522::StatusCode status;
 
    //Authenticate using key A
    //Serial.println("Authenticating using key A...");
    //MFRC522::PICC_Command piccAuthKey = MFRC522::PICC_CMD_MF_AUTH_KEY_A;
  
    piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, SectorAccess, &key, &RfChip->uid);
    if (piccStatus != MFRC522::STATUS_OK) 
    {
      Serial.println("Authenticate key A::failed: ");
      Serial.println("");
      RfChip->PCD_StopCrypto1();
      RfChip->PICC_HaltA();
      if(!RfChip->PICC_IsNewCardPresent())
        {
          //Debug.printf("NO Card Detected\r\n");
          return false;
        }
      if (!RfChip->PICC_ReadCardSerial())
        {
           //Debug.printf("Cannot read serial number\r\n");
          return false;
        }
        
      //Debug.printf("PICC Card UID: ");
      for (byte i = 0; i < 4; i++) 
        {
          nuidPICC[i] = RfChip->uid.uidByte[i];
        }

      piccType = RfChip->PICC_GetType(RfChip->uid.sak);

      for(int mainCounter = 0; mainCounter < MFRC522::MF_KEY_SIZE; mainCounter++)
          key.keyByte[mainCounter] = 0xFF;
      
      piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, SectorAccess, &key, &RfChip->uid);
      if(piccStatus != MFRC522::STATUS_OK)
      {
        Serial.println("Authenticate key B::failed: ");
        Serial.println("");
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/GreenReadingStatusLed/BlueOkStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
      }
      //Serial.println("Authenticating using key B...");
      piccAuthKey = MFRC522::PICC_CMD_MF_AUTH_KEY_B;
    }
    else
  
    //Serial.println("Authentification type -> A");

    memset(UniqueIDArduino, 0 , 16); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //UNIQUE ID ARDUINO
    IDArduino();
  
    // Serial.print(F("ID Arduino::"));
    // printHex(UniqueIDArduino, 16);
    // Serial.println("");
    //  printDec(UniqueIDArduino,16);
    // Serial.println("");
   
    //uint8_t piccReadBuffer[PICC_RX_SIZE];
    //uint8_t piccReadBufferSize = sizeof(piccReadBuffer);
    piccStatus = RfChip->PCD_Authenticate(piccAuthKey, SectorAccess, &key, &RfChip->uid);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Authentification block[%d] -> %s\r\n\r\n", PICC_TRAILER_BLOCK_1, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
    }
    
    piccStatus = RfChip->MIFARE_Write(BlockAccess,(uint8_t *)&UniqueIDArduino[0], 16);
    //Serial.println(SN);
    //piccStatus = RfChip->MIFARE_Write(BlockAccess,(uint8_t *)&SN[0], 16);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Writing Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA(); 
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
    }
    piccStatus = RfChip->MIFARE_Write(BlockAccess + 1, tv, 16);
    if(piccStatus != MFRC522::STATUS_OK)
    {
    //   Debug.printf("ERROR: Writing Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK + 1, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }

    piccStatus = RfChip->MIFARE_Read(BlockAccess, piccReadBuffer, &piccReadBufferSize);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        //Debug.printf("ERROR: Reading Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }  

    Serial.print(F("Validator::"));
    //printHex(piccReadBuffer, 16);
    //Serial.println("");
    validatorSN = returnValidator(piccReadBuffer, 16);
    Serial.println(validatorSN);
    //Web Service Comunication
    // if (myServerCom.HttpClientRequest(false,arrayWS,false,ipWS,ipFija,validatorSN,0)){
    arrayWS = returnArrayWS(EntryDate, 16, validatorSN,Card);
    Serial.print("ArrayWS: ");
    Serial.println(arrayWS);
    //   }


    Serial.println("Key acess sector 0");
    SectorAccess = KEYACCESS_SECTOR_0;  
    BlockAccess = BLOCK_0 + 8;

    piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, SectorAccess, &key, &RfChip->uid);
    if (piccStatus != MFRC522::STATUS_OK) 
    {
      Serial.println("Authenticate key A::failed: ");
      Serial.println("");
      RfChip->PCD_StopCrypto1();
      RfChip->PICC_HaltA();
      if(!RfChip->PICC_IsNewCardPresent())
        {
          //Debug.printf("NO Card Detected\r\n");
          return false;
        }
      if (!RfChip->PICC_ReadCardSerial())
        {
           //Debug.printf("Cannot read serial number\r\n");
          return false;
        }
        
      //Debug.printf("PICC Card UID: ");
      for (byte i = 0; i < 4; i++) 
        {
          nuidPICC[i] = RfChip->uid.uidByte[i];
        }

      piccType = RfChip->PICC_GetType(RfChip->uid.sak);

      for(int mainCounter = 0; mainCounter < MFRC522::MF_KEY_SIZE; mainCounter++)
          key.keyByte[mainCounter] = 0xFF;
      
      piccStatus = RfChip->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, SectorAccess, &key, &RfChip->uid);
      if(piccStatus != MFRC522::STATUS_OK)
      {
        Serial.println("Authenticate key B::failed: ");
        Serial.println("");
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/GreenReadingStatusLed/BlueOkStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
      }
      //Serial.println("Authenticating using key B...");
      piccAuthKey = MFRC522::PICC_CMD_MF_AUTH_KEY_B;
    }
    else
  
    //Serial.println("Authentification type -> A");

    memset(UniqueIDArduino, 0 , 16); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //UNIQUE ID ARDUINO
    IDArduino();
  
    // Serial.print(F("ID Arduino::"));
    // printHex(UniqueIDArduino, 16);
    // Serial.println("");
    //  printDec(UniqueIDArduino,16);
    // Serial.println("");
   
    //uint8_t piccReadBuffer[PICC_RX_SIZE];
    //uint8_t piccReadBufferSize = sizeof(piccReadBuffer);
    piccStatus = RfChip->PCD_Authenticate(piccAuthKey, SectorAccess, &key, &RfChip->uid);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        Serial.println("Autentification1 error ");
        //Debug.printf("ERROR: Authentification block[%d] -> %s\r\n\r\n", PICC_TRAILER_BLOCK_1, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        return false;
    }

    piccStatus = RfChip->MIFARE_Read(2, piccReadBuffer, &piccReadBufferSize);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        Serial.println("error reading before");
        //Debug.printf("ERROR: Reading Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }  
    Serial.print(F("ET before:"));
    printHex(piccReadBuffer, 16);

   
    piccStatus = RfChip->MIFARE_Write(2, ET, 16);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        Serial.println("ET error write");
    //   Debug.printf("ERROR: Writing Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK + 1, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }

    piccStatus = RfChip->MIFARE_Read(2, piccReadBuffer, &piccReadBufferSize);
    if(piccStatus != MFRC522::STATUS_OK)
    {
        Serial.println("error reading");
        //Debug.printf("ERROR: Reading Block [%d] -> %s\r\n\r\n", PICC_LPC_UID_BLOCK, RfChip->GetStatusCodeName(piccStatus));
        RfChip->PCD_StopCrypto1();
        RfChip->PICC_HaltA();
        mySignal->ledON(RED);
        //setColor(0, 255, 255); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
        MifareReaderAvailable = false;
        ErrorStatusLedTimeout.timeOut(500, mifareReaderWriter::ErrorStatusLedTimeoutHandler); //delay 10000=10seg, callback function
        //ErrorStatusLedTimeout.attach(&mifareReaderWriter::ErrorStatusLedTimeoutHandler, TIME_SLEEP_MIFARE);
        return false;
    }  
    Serial.print(F("ET after:"));
    printHex(piccReadBuffer, 16);


    /*End Acces*/
    RfChip->PCD_StopCrypto1();
    RfChip->PICC_HaltA();
    mySignal->ledON(GREEN);
    //setColor(255, 255, 0); //RGB= RedErrorStatusLed/BlueOkStatusLed/GreenReadingStatusLed
    //analogWrite(Buzzer, 255);
    mySignal->buzzerON();
    delay(300);
    //Beep_Buzzer(2058,300, 2);
    MifareReaderAvailable = false;
    mySignal->flashLed(1,GREEN,500,0,true);
    //toggleCounting(true,LastTimeMifare);
    OkStatusLedTimeout.timeOut(400, mifareReaderWriter::OkStatusLedTimeoutHandler); //delay 10000=10seg, callback function
    //MifareActivateTimeoutHandler();
    Serial.println("");  
    return true;
    }