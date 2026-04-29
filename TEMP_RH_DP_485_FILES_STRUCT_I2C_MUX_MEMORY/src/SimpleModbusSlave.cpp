#include "SimpleModbusSlave.h"

#define BUFFER_SIZE 128
#define ModbusSerial Serial2
// frame[] is used to recieve and transmit packages. 
// The maximum serial ring buffer size is 128
unsigned char frame[BUFFER_SIZE];
unsigned int holdingRegsSize=100; // size of the register array 
unsigned char broadcastFlag;
unsigned char slaveID;
unsigned char function;
unsigned char TxEnablePin;
unsigned int errorCount;
unsigned int T1_5; // inter character time out
unsigned int T3_5; // frame delay

// #define TX 16
// #define RX 17

bool SimpleModbusSlave::modbus_update(signed int *holdingRegs)
{
  unsigned char buffer = 0;
  unsigned char overflow = 0;
  bool status=false;
  while (ModbusSerial.available())
  {
    // The maximum number of bytes is limited to the serial buffer size of 128 bytes
    // If more bytes is received than the BUFFER_SIZE the overflow flag will be set and the 
    // serial buffer will be red untill all the data is cleared from the receive buffer.
    if (overflow) 
      ModbusSerial.read();
    else
    {
      if (buffer == BUFFER_SIZE)
        overflow = 1;
      frame[buffer] = ModbusSerial.read();
      buffer++;
    }
    delayMicroseconds(T1_5); // inter character time out
  }

  // Serial.print("T_Byte : " + String(myLinkedList.size()));
  // for (byte i=0 ; i < ModbusSerial.available() ; i++)
  // {
  //   byte b=ModbusSerial.read();
  //   //Serial.print(" "); Serial.print(b,HEX);
  //   myLinkedList.add(b);
  //     //frame[i] = b;
  // }
  //Serial.println();
  //Serial.print("myLinkedList : " + String(myLinkedList.size()));
  // If an overflow occurred increment the errorCount
  // variable and return to the main sketch without 
  // responding to the request i.e. force a timeout
  if (overflow)
    return status;
  
  // The minimum request packet is 8 bytes for function 3 & 16
  if (buffer >= 8) 
  {
    unsigned char id = frame[0];
    
    broadcastFlag = 0;
    
    if (id == 0)
      broadcastFlag = 1;
    
    if (id == slaveID || broadcastFlag) // if the recieved ID matches the slaveID or broadcasting id (0), continue
    {
      last_connected_millis = millis();
      isStore=false;
      unsigned int crc = ((frame[buffer - 2] << 8) | frame[buffer - 1]); // combine the crc Low & High bytes
      if (calculateCRC(buffer - 2) == crc) // if the calculated crc matches the recieved crc continue
      {
        function = frame[1];
        unsigned int startingAddress = ((frame[2] << 8) | frame[3]); // combine the starting address bytes
        unsigned int no_of_registers = ((frame[4] << 8) | frame[5]); // combine the number of register bytes  
        unsigned int maxData = startingAddress + no_of_registers;
        unsigned char index;
        unsigned char address;
        unsigned int crc16;
        
        //Serial.println("Funcation code : "+String(function)+" | " + String(startingAddress) + " | " + String(maxData) + " | " + String(holdingRegsSize));

        // broadcasting is not supported for function 3 
        //if (!broadcastFlag && (function == 3))
        if((function == 3) && (no_of_registers<=25))
        {
          if (startingAddress < holdingRegsSize) // check exception 2 ILLEGAL DATA ADDRESS
          {
            if (maxData <= holdingRegsSize) // check exception 3 ILLEGAL DATA VALUE
            {
              unsigned char noOfBytes = no_of_registers * 2;
              unsigned char responseFrameSize = 5 + noOfBytes; // ID, function, noOfBytes, (dataLo + dataHi) * number of registers, crcLo, crcHi
              //frame[0] = slaveID;
              frame[1] = function;
              frame[2] = noOfBytes;
              address = 3; // PDU starts at the 4th byte
              unsigned int temp;
              
              for (index = startingAddress; index < maxData; index++)
              {
                temp = holdingRegs[index];
                frame[address] = temp >> 8; // split the register into 2 bytes
                address++;
                frame[address] = temp & 0xFF;
                address++;
              } 
              
              crc16 = calculateCRC(responseFrameSize - 2);
              frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
              frame[responseFrameSize - 1] = crc16 & 0xFF;
              sendPacket(responseFrameSize);
              status=true;
            }
            else  
              exceptionResponse(3); // exception 3 ILLEGAL DATA VALUE
          }
          else
            exceptionResponse(2); // exception 2 ILLEGAL DATA ADDRESS
        }
        else if (function == 6)
        {
          if (startingAddress < holdingRegsSize) // check exception 2 ILLEGAL DATA ADDRESS
          {
              unsigned int startingAddress = ((frame[2] << 8) | frame[3]);
              unsigned int regStatus = ((frame[4] << 8) | frame[5]);
              unsigned char responseFrameSize = 8;
              // if(startingAddress==60)
              //   Serial.println("Read Next Memory Data Request : " + String(regStatus));
              holdingRegs[startingAddress] = (short)regStatus;
              
              crc16 = calculateCRC(responseFrameSize - 2);
              frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
              frame[responseFrameSize - 1] = crc16 & 0xFF;
              sendPacket(responseFrameSize);
              status=true;
          }
          else
            exceptionResponse(2); // exception 2 ILLEGAL DATA ADDRESS
          }
        else if (function == 16)
        {
          //Serial.println("Funcation code : 16 | " + String(startingAddress) + " | " + String(maxData) + " | " + String(holdingRegsSize));
          // check if the recieved number of bytes matches the calculated bytes minus the request bytes
          // id + function + (2 * address bytes) + (2 * no of register bytes) + byte count + (2 * CRC bytes) = 9 bytes
          if (frame[6] == (buffer - 9)) 
          {
            if (startingAddress < holdingRegsSize) // check exception 2 ILLEGAL DATA ADDRESS
            {
              if (maxData <= holdingRegsSize) // check exception 3 ILLEGAL DATA VALUE
              {
                address = 7; // start at the 8th byte in the frame
                
                for (index = startingAddress; index < maxData; index++)
                {
                  holdingRegs[index] = (short)((frame[address] << 8) | frame[address + 1]);
                  address += 2;
                }
                
                // only the first 6 bytes are used for CRC calculation
                crc16 = calculateCRC(6); 
                frame[6] = crc16 >> 8; // split crc into 2 bytes
                frame[7] = crc16 & 0xFF;
                
                // a function 16 response is an echo of the first 6 bytes from the request + 2 crc bytes
                if (!broadcastFlag) // don't respond if it's a broadcast message
                {
                  //Serial.println("Write Command Recivied");
                  sendPacket(8); 
                  status=true;
                }
              }
              else  
              {
                exceptionResponse(3); // exception 3 ILLEGAL DATA VALUE
                //Serial.println("exception 3 ILLEGAL DATA VALUE");
              }
            }
            else
            {
              exceptionResponse(2); // exception 2 ILLEGAL DATA ADDRESS
              //Serial.println("exception 2 ILLEGAL DATA ADDRESS");
            }
          }
          else 
          {
            errorCount++; // corrupted packet
            //Serial.println("exception 10 ILLEGAL DATA ADDRESS");
          }
        }         
        else
        {
          exceptionResponse(1); // exception 1 ILLEGAL FUNCTION
          //Serial.println("exception 1 ILLEGAL FUNCTION");
        }
      }
      else // checksum failed
        errorCount++;
    } // incorrect id
  }
  else if (buffer > 0 && buffer < 8)
    errorCount++; // corrupted packet
    
  return status;
}       

void SimpleModbusSlave::exceptionResponse(unsigned char exception)
{
  errorCount++; // each call to exceptionResponse() will increment the errorCount
  if (!broadcastFlag) // don't respond if its a broadcast message
  {
    frame[0] = slaveID;
    frame[1] = (function | 0x80); // set the MSB bit high, informs the master of an exception
    frame[2] = exception;
    unsigned int crc16 = calculateCRC(3); // ID, function + 0x80, exception code == 3 bytes
    frame[3] = crc16 >> 8;
    frame[4] = crc16 & 0xFF;
    sendPacket(5); // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc
  }
}

void SimpleModbusSlave::modbus_configure(long baud, unsigned char _slaveID, unsigned int parity, unsigned int stop_bit, unsigned char _TxEnablePin, unsigned int _holdingRegsSize, unsigned char _lowLatency)
{
  slaveID = _slaveID;
  //ModbusSerial.begin(baud);
  // Serial.println(String(parity)+String(stop_bit));
  if (parity == 0 && stop_bit == 1)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8N1);
  }
  else if (parity == 1 && stop_bit == 1)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8E1);
  }
  else if (parity == 2 && stop_bit == 1)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8O1);
  }
  else if (parity == 0 && stop_bit == 2)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8N2);
  }
  else if (parity == 1 && stop_bit == 2)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8E2);
  }
  else if (parity == 2 && stop_bit == 2)
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8O2);
  }
  else
  {
    ModbusSerial.flush();
    ModbusSerial.end();
    ModbusSerial.begin(baud, SERIAL_8N1 );
  } 
  
  
  if (_TxEnablePin > 1) 
  { // pin 0 & pin 1 are reserved for RX/TX. To disable set txenpin < 2
    TxEnablePin = _TxEnablePin; 
    pinMode(TxEnablePin, OUTPUT);
    digitalWrite(TxEnablePin, LOW);
  }
  
  // Modbus states that a baud rate higher than 19200 must use a fixed 750 us 
  // for inter character time out and 1.75 ms for a frame delay.
  // For baud rates below 19200 the timeing is more critical and has to be calculated.
  // E.g. 9600 baud in a 10 bit packet is 960 characters per second
  // In milliseconds this will be 960characters per 1000ms. So for 1 character
  // 1000ms/960characters is 1.04167ms per character and finaly modbus states an
  // intercharacter must be 1.5T or 1.5 times longer than a normal character and thus
  // 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.
  // Added experimental low latency delays. This makes the implementation
  // non-standard but practically it works with all major modbus master implementations.
  
  if (baud == 1000000 && _lowLatency)
  {
      T1_5 = 1; 
      T3_5 = 10;
  }
  else if (baud >= 115200 && _lowLatency){
      T1_5 = 75; 
      T3_5 = 175; 
  }
  else if (baud > 19200)
  {
    T1_5 = 750; 
    T3_5 = 1750;
  }
  else 
  {
    T1_5 = 15000000/baud; // 1T * 1.5 = T1.5
    T3_5 = 35000000/baud; // 1T * 3.5 = T3.5
  }
  
  holdingRegsSize = _holdingRegsSize;
  errorCount = 0; // initialize errorCount
}   

unsigned int SimpleModbusSlave::calculateCRC(byte bufferSize) 
{
  unsigned int temp, temp2, flag;
  temp = 0xFFFF;
  for (unsigned char i = 0; i < bufferSize; i++)
  {
    temp = temp ^ frame[i];
    for (unsigned char j = 1; j <= 8; j++)
    {
      flag = temp & 0x0001;
      temp >>= 1;
      if (flag)
        temp ^= 0xA001;
    }
  }
  // Reverse byte order. 
  temp2 = temp >> 8;
  temp = (temp << 8) | temp2;
  temp &= 0xFFFF;
  return temp; // the returned value is already swopped - crcLo byte is first & crcHi byte is last
}

void SimpleModbusSlave::sendPacket(unsigned char bufferSize)
{
  if (TxEnablePin > 1)
    digitalWrite(TxEnablePin, HIGH);
    
  for (unsigned char i = 0; i < bufferSize; i++)
    ModbusSerial.write(frame[i]);
    
  
  ModbusSerial.flush();
  // allow a frame delay to indicate end of transmission
  delayMicroseconds(T3_5); 

  while(ModbusSerial.available())
    ModbusSerial.read();
  
  if (TxEnablePin > 1)
    digitalWrite(TxEnablePin, LOW);
}