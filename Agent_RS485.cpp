#include "Agent_RS485.h"
#include <SoftwareSerial.h> 
 
 Agent_RS485::Agent_RS485 (byte _Address, byte _SSerialRX, byte _SSerialTX, byte _SSerialTxControl, byte _RS485Transmit){
    Address=_Address;
    SSerialRX = _SSerialRX;
    SSerialTX = _SSerialTX;
    SSerialTxControl = _SSerialTxControl;
    RS485Transmit = _RS485Transmit;
    RS485Receive = !(_RS485Transmit);
  }

 void Agent_RS485::begin(int _baudrate, int _delay_between_bytes){
  baudrate=_baudrate;
  delay_between_bytes=_delay_between_bytes;
  pinMode(SSerialTxControl, OUTPUT);    
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   
  
  RS485Serial = new SoftwareSerial(SSerialRX, SSerialTX);
  RS485Serial->begin(baudrate);
 }


 void Agent_RS485::sendPacket(int _Address, byte _PL0, byte _PL1, byte _PL2, byte _PL3, byte _PL4) {
    int chsum=0;
    chsum = (_Address+_PL0+_PL1+_PL2+_PL3+_PL4) % 256;
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
    delay(10);  
    delay(delay_between_bytes);
    RS485Serial->write(64);  // @-Zeichen, das den Start des Pakets markiert
    delay(delay_between_bytes);
    RS485Serial->write(_Address);
    delay(delay_between_bytes);
    RS485Serial->write(_PL0);
    delay(delay_between_bytes);
    RS485Serial->write(_PL1);
    delay(delay_between_bytes);
    RS485Serial->write(_PL2);
    delay(delay_between_bytes);
    RS485Serial->write(_PL3);
    delay(delay_between_bytes);
    RS485Serial->write(_PL4);
    delay(delay_between_bytes);
    RS485Serial->write(chsum);
    delay(delay_between_bytes);
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit       
    delay(10);
}


int Agent_RS485::dataAvailable(int _millis){
    int etime=0;  
    while (!RS485Serial->available() && (etime <_millis)){  
      delay(10);
      etime+=10;
    }
  return (etime<_millis ? 1 : 0); 
  delay(10);
  
}


int Agent_RS485::packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis, int &_errcode){

// Receives a packet (the last two parameters represent the timeout interval and an error code)
  // bit 0: different address
  // bit 1: timeout 
  // bit 2: wrong start byte
  // bit 3: wrong checksum

  
  int etime=0;
  int rbyte[8];
  int chsum=0;
  _errcode=0;

  for (int i=0; i<=7; i++){
    while (!RS485Serial->available() && (etime <_millis)){  
      delay(10);
      etime+=10;
    }
    if (etime>_millis){
      _errcode+=2;
      return 0;
    }
    rbyte[i]=RS485Serial->read();
     
  } 
    _PL0=rbyte[2];
    _PL1=rbyte[3];
    _PL2=rbyte[4];
    _PL3=rbyte[5];
    _PL4=rbyte[6];
	 chsum = (Address+_PL0+_PL1+_PL2+_PL3+_PL4) % 256;
  if ((rbyte[0]==64) && (etime<_millis) && (rbyte[1]==Address)){
        
    return (chsum!=rbyte[7] ? 0 : 1);
    }   else {
   
      if (rbyte[0]!=64){
        _errcode+=4;
      }
      if (chsum!=rbyte[7]){
        _errcode+=1;
      }
      if (rbyte[1]!=Address){
         _errcode=1;
      }

      return 0;
    }        
}

int Agent_RS485::packetReceived(int &_PL0, int &_PL1, int &_PL2, int &_PL3, int &_PL4, int _millis){
  int errcode=0;
  return packetReceived(_PL0,_PL1,_PL2,_PL3,_PL4,_millis,errcode);
}

void Agent_RS485::explainErrCode(int _errcode){
  if (_errcode & 1) {
	  Serial.println("Address differs");  
  }	  
  if (_errcode & 2) {
	  Serial.println("Timeout");  
  }	  
  if (_errcode & 4) {
	  Serial.println("Wrong start byte");  
  }	  
  if (_errcode & 8) {
	  Serial.println("Wrong checksum");  
  }	  
  if (_errcode==0) {
	  Serial.println("No error detected");  
  }
}