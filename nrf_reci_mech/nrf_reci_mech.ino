
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>]
#include<Servo.h>

Servo car_steer;

RF24 radio(7, 8); // CE, CSN
#define steer A0
#define acc A1
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

bool gear=0;
struct data_package
{
  byte steer_val;
  byte acc_val;
  bool car;
};
data_package data;
void reset()
{
  data.steer_val=90;
  data.acc_val=0;
  data.car=1;
}
void setup() {
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  car_steer.attach(6);
  car_steer.write(90);
  Serial.begin(9600);
  reset();
  delay(1000);

}
void loop()
{
  if (radio.available())
  {    radio.read(&data, sizeof(data_package)); // Read the whole data and store it into the 'data' structure
lastReceiveTime = millis();
    
  }
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    reset();
  
      if (data.car==1) //Forward
      {
        digitalWrite(5,0);
        analogWrite(3,data.acc_val);
        car_steer.write(data.steer_val);
      }
      else
      {
        digitalWrite(3,0);
        analogWrite(5,data.acc_val);
        car_steer.write(data.steer_val);
      }
      Serial.println(data.acc_val);  
}
