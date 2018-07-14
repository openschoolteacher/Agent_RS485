// Arduino library for master-slave communication in RS485 based buses
// Peter Dauscher
// License: LGPL 3.0

// !! WARNING: THERE IS ABSOLUTELY NO WARRANTY FOR THIS LIBRARY!!
// It is in an experimental state and tested only in a very limited scope.
// Wrong wiring and software errors may cause hardware damages 
 

#ifndef __AGENT_RS485__
#define __AGENT_RS485__

#include <Arduino.h>
#include <SoftwareSerial.h>


class Agent_RS485 {
private:

  byte Address;           // Bus address of the agent  

  byte SSerialRX;         // Serial Receive pin
  byte SSerialTX;         // Serial Transmit pin

  byte SSerialTxControl;  // RS485 Direction control

  byte RS485Transmit;
  byte RS485Receive;       // The inverse of RS485

  int delay_between_bytes;
  int baudrate; 
  
  
  SoftwareSerial *RS485Serial;


public:
  
  Agent_RS485 (byte _Address, byte _SSerialRX, byte _SSerialTX, byte _SSerialTxControl, byte _RS485Transmit);
  // Constructor
  // Creates a Agent_RS485 object with the given address, the three pins for bus control (RX,TX,Direction) and 
  // the value for transmit direction.
  
  void begin(int _baudrate, int _delay_between_bytes);
  // Starts communication (with given baud rate and delay between the single bytes transmitted)
  
  void sendPacket(int _Address, byte _PL0, byte _PL1, byte _PL2, byte _PL3, byte _PL4);
  // Sends a packet consisting of destination address and 5 payload bytes to a given adress
  
  int dataAvailable(int _millis);
  // Checks wheter data is available on the bus.
  
  int packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis, int &_errcode);
  // Receives a packet (the last two parameters represent the timeout interval and an error code)
  // bit 0: different address
  // bit 1: timeout 
  // bit 2: wrong start byte
  // bit 3: wrong checksum
  
  int packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis);
  // Receives a packet (the last parameter represents the timeout interval )
  
  void explainErrCode(int _errcode);
  // Explains the Error Code in words.
  
  
};

#endif
