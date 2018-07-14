#include <Agent_RS485.h>


#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit HIGH

#define RS485Baudrate 4800
#define RS485Delay 5

int xPL0=0;
int xPL1=0;
int xPL2=0;
int xPL3=0;
int xPL4=0;

int destination=42;



Agent_RS485 Master(0, SSerialRX,SSerialTX,SSerialTxControl,RS485Transmit);
int counter=0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Master.begin(RS485Baudrate,RS485Delay);
  Serial.begin(9600);
  Serial.println("Master Agent (No. 0) ");
  Serial.println("Sending packets to Agent No. 42");
  Serial.println("======================================");  
}

// the loop function runs over and over again forever
void loop() {
  int errcode=0;
  Master.sendPacket(destination,counter,0,0,0,0);
  Serial.print("Packet sent to Agent #");
  Serial.print(destination);
  Serial.print(" : ");
  Serial.print(counter);
  Serial.println(";0;0;0;0");

 /*digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(10);                       // wait for a second*/

   
  if (Master.dataAvailable(3000)) {
    //Serial.println("Information available on the bus");
    if (Master.packetReceived(xPL0,xPL1,xPL2,xPL3,xPL4,2000,errcode)){
      Serial.print("Received:                  ");
      Serial.print(xPL0);
      Serial.print(";");
      Serial.print(xPL1);
      Serial.print(";");
      Serial.print(xPL2);
      Serial.print(";");
      Serial.print(xPL3);
      Serial.print(";");
      Serial.println(xPL4);
      } else if (errcode==1){
        Serial.println("Packet destitation was another agent");
      }
      else {
        Serial.print("Packet loss due to error: ");
        Master.explainErrCode(errcode);
      }
    } else { 
      Serial.println("No information available."); 
    }
    counter++;
    counter=(counter<100 ? counter : 0);
    Serial.println("--------------------------------------");
    delay(10);
  
}
