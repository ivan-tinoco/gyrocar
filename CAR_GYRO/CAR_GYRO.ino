/*  ME106 Term Project
   SJSU - Spring 2017
   Hand Gesture Controlled Robotic Car*/

//CAR-MODULE
//Code to control DC-Motors and RF Receiver

//DAC/ADC Library used for RF Transmission
#include <RH_ASK.h>   // (Uses pin 11 to read data)
#include <SPI.h>      //Not actually used but needed to compile (RH_ASK.h Library specified)

RH_ASK driver;

//DC-motor driver control pins:
int in1 = 9;
int in2 = 8;
int in3 = 7;
int in4 = 6;
int ENA = 5;
int ENB = 3;
int ctrlSpeed1 = 150;
int ctrlSpeed2 = 160;

//TX Control counter
int count = 0;

//Car-movement control functions:
void _mForward()
{
  analogWrite(ENA, ctrlSpeed1);
  analogWrite(ENB, ctrlSpeed1);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("forward");
}
void _mBack()
{
  analogWrite(ENA, ctrlSpeed1);
  analogWrite(ENB, ctrlSpeed1);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("back");
}
void _mleft()
{
  analogWrite(ENA, ctrlSpeed2);
  analogWrite(ENB, ctrlSpeed2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("left");
}
void _mright()
{
  analogWrite(ENA, ctrlSpeed2);
  analogWrite(ENB, ctrlSpeed2);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  //Serial.println("right");
}
void _mStop()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP");
}

//Initial setup I/O + Serial Comm + RF Comm
void setup()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //Serial Comm use to debug
  Serial.begin(9600);

  _mStop();

  //RF module initialize:
  if (!driver.init()) Serial.println("init failed");
}

//Continuous signal reading/processing:
void loop()
{
  //RH_ASK.h library ADC processing:
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  //if RF signal recieved:
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    count = 0;
    Serial.println(buf[0]); //debugging
    if (buf[0] == '0')
    {
      _mForward();
    }
    else if (buf[0] == '1')
    {
      _mBack();
    }
    else if (buf[0] == '2')
    {
      _mright();
    }
    else if (buf[0] == '3')
    {
      _mleft();
    }
    else if (buf[0] == '4')
    {
      _mStop();
    }
    else
    {
      _mStop();
    }
  }
  else
  {
    count++;
  }

  Serial.println(count);
  if (count > 200 ) //Transmission lost
  {
    _mStop();
  }
}
