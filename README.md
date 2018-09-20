# Agent_RS485 library 0.9.2 #

Peter Dauscher
License: LGPL 3.0

Arduino library for master-slave communication in RS485 based buses. 
It uses SoftwareSerial.h.

-------
**WARNING: THERE IS ABSOLUTELY NO WARRANTY FOR THIS LIBRARY NOR THE DOCUMENTATION**
**It is in an experimental state and tested only in a very limited scope**
**Wrong wiring and software errors may cause hardware damages**
-------




## Commands ##

### Constructor ###
```
Agent_RS485 (byte _Address, byte _SSerialRX, byte _SSerialTX, byte _SSerialTxControl, byte _RS485Transmit);
```
Creates an `Agent_RS485` object with the given address, the three pins for bus control (RX,TX,Direction) and the value for transmit direction.

### Starting Communication ###
```  
void begin(int _baudrate, int _delay_between_bytes);
```
Starts communication (with given baud rate and delay between the single bytes transmitted)

### Sending Packets ###
```  
void sendPacket(int _Address, byte _PL0, byte _PL1, byte _PL2, byte _PL3, byte _PL4);
```
Sends a packet consisting of destination address and 5 payload bytes to a given adress.

### Check for Available Data ###
```  
int dataAvailable(int _millis);
```
Checks wheter data is available on the bus.
  
### Receiving Packets ###
```
int packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis, int &_errcode);
```
Receives a packet (the last two parameters represent the timeout interval and an error code)
+ bit 0: different address
+ bit 1: timeout 
+ bit 2: wrong start byte
+ bit 3: wrong checksum

```
  int packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis);
```
Receives a packet (the last parameter represents the timeout interval )

```
void explainErrCode(int _errcode);
```
Explains the Error Code in words.


## History ##

+ 0.9.2: error code generation corrected
+ 0.9.1: error code as status byte
+ 0.9.0: First version
