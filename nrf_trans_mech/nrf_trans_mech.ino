/*
* Arduino Wireless Communication Tutorial
* 
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8); // CE, CSN
#define steer A0
#define acc A1
const byte address[6] = "00001";
int gear=0;
struct data_package
{
  byte steer_val;
  byte acc_val;
  bool car;
};
data_package data;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  pinMode(4,INPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);

Serial.begin(9600);
}

void read_data()
{ unsigned int acc1=0;
  unsigned int str1=0;
  //gear
  gear=analogRead(A2);
  if(gear<100)
  data.car=0;
  else
  data.car=1;
//accelerator


acc1=analogRead(acc);
if(acc1<300) data.acc_val=0;
if(acc1>750) acc1=750;
if(acc1>=300) data.acc_val=map(acc1,300,750,60,255);


//steerin
str1=analogRead(steer);
if(str1<300) str1=300;
if(str1>750) str1=750;
if(str1>=300) data.steer_val=map(str1,300,750,140,30);

}

void loop() {
read_data();
Serial.println(data.steer_val);
delay(10);
radio.write(&data,sizeof(data_package));
}
