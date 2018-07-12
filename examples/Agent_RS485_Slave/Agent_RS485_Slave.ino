#include <Agent_RS485.h>


// Variables vor Payload ("xPL" because "PL" is reserved for some Arduino Controllers)
int xPL0=0;
int xPL1=0;
int xPL2=0;
int xPL3=0;
int xPL4=0;


#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit HIGH

#define RS485Baudrate 300
#define RS485Delay 20


#define AgentNr 42


Agent_RS485 Slave(AgentNr, SSerialRX,SSerialTX,SSerialTxControl,RS485Transmit);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Slave.begin(RS485Baudrate,RS485Delay);
  Serial.begin(9600);
  Serial.print("Slave Agent No. ");
  Serial.println(AgentNr);
  Serial.println("====================");
}

// the loop function runs over and over again forever
void loop() {
  int errcode=0;
   
  if (Slave.dataAvailable(3000)) {
    Serial.println("Information available on the bus");
    if (Slave.packetReceived(xPL0,xPL1,xPL2,xPL3,xPL4,2000,errcode)){
      Serial.print("Received: ");
      Serial.print(xPL0);
      Serial.print(";");
      Serial.print(xPL1);
      Serial.print(";");
      Serial.print(xPL2);
      Serial.print(";");
      Serial.print(xPL3);
      Serial.print(";");
      Serial.println(xPL4);
     
      // Send the packet back, exchanging the last byte by the Agent Number.
      Slave.sendPacket(0,xPL0,xPL2,xPL2,xPL3,AgentNr); 

      Serial.print("Sent back (Agent No. as last payload byte):");
      Serial.print(xPL0);
      Serial.print(";");
      Serial.print(xPL1);
      Serial.print(";");
      Serial.print(xPL2);
      Serial.print(";");
      Serial.print(xPL3);
      Serial.print(";");
      Serial.println(AgentNr);
     
     } else if (errcode==1){
        Serial.println("Packet destitation was another agent");
      }
      else {
        Serial.print("Ppacket loss due to error: ");
        Serial.println(errcode);
      }
    } else { 
      Serial.println("No Information available on the bus"); 
    }
}
