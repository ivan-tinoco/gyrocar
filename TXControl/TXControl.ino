/*  ME106 Term Project
   SJSU - Spring 2017
   Hand Gesture Controlled Robotic Car*/

//CONTROL-MODULE
//Code to read MPU6050 (Gyroscope Sensor) data and RF Transmitter

//Libraris used to communicate with MPU6050 module:
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

//DAC/ADC Library used for RF Transmission:
#include <RH_ASK.h>   // (Uses pin 12 to send data)
#include <SPI.h>      // Not actually used but needed to compile

RH_ASK driver;

//LED pins used for debugging:
//#define pinA 4
//#define pinB 5
//#define pinC 6
//#define pinD 7

//Initial setup I/O + Serial Comm + RF Comm
void setup()
{
  //Debug only LED pins
  //  pinMode(pinA, OUTPUT);
  //  pinMode(pinB, OUTPUT);
  //  pinMode(pinC, OUTPUT);
  //  pinMode(pinD, OUTPUT);

  Serial.begin(9600);

  //RF module initialize:
  if (!driver.init()) Serial.println("init failed");

  //Initialize MPU6050:
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

//Continuous data reading/processing, signal send:
void loop()
{
  //MPU6050 data read
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

//    Used to read raw values and debugging
//    Serial.print(" Xraw = ");
//    Serial.println(rawAccel.XAxis);
//    Serial.print(" Yraw = ");
//    Serial.println(rawAccel.YAxis);
//    Serial.println();
//    delay(500);
  //  Serial.print(" Zraw = ");
  //  Serial.println(rawAccel.ZAxis);

  if (rawAccel.XAxis < -7000 && rawAccel.XAxis > -16000 && rawAccel.YAxis < 5999  && rawAccel.YAxis > -5999) //forward
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, LOW);
    //    digitalWrite(pinB, HIGH);
    //    digitalWrite(pinC, HIGH);
    //    digitalWrite(pinD, LOW);

    //Control Signal
    sendSignal("0");
  }
  else if (rawAccel.XAxis > 7000 && rawAccel.XAxis < 17000 && rawAccel.YAxis < 5999  && rawAccel.YAxis > -5999) //backward
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, HIGH);
    //    digitalWrite(pinB, LOW);
    //    digitalWrite(pinC, LOW);
    //    digitalWrite(pinD, HIGH);

    //Control Signal
    sendSignal("1");
  }
  else if (rawAccel.YAxis > 7000 && rawAccel.YAxis < 16000 && rawAccel.XAxis < 6999 && rawAccel.XAxis > -6999) //right
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, LOW);
    //    digitalWrite(pinB, LOW);
    //    digitalWrite(pinC, HIGH);
    //    digitalWrite(pinD, HIGH);

    //Control Signal
    sendSignal("2");
  }
  else if (rawAccel.YAxis < -7000 && rawAccel.YAxis > -16000 && rawAccel.XAxis < 5999 && rawAccel.XAxis > -2999) //left
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, HIGH);
    //    digitalWrite(pinB, HIGH);
    //    digitalWrite(pinC, LOW);
    //    digitalWrite(pinD, LOW);

    //Control Signal
    sendSignal("3");
  }
  else if (rawAccel.XAxis < 6999 && rawAccel.XAxis > -6999 && rawAccel.YAxis < 5999  && rawAccel.YAxis > -5999) //stop
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, LOW);
    //    digitalWrite(pinB, HIGH);
    //    digitalWrite(pinC, LOW);
    //    digitalWrite(pinD, HIGH);

    //Control Signal
    sendSignal("4");
  }
  else
  {
    //LED pins - Debug only
    //    digitalWrite(pinA, LOW);
    //    digitalWrite(pinB, LOW);
    //    digitalWrite(pinC, LOW);
    //    digitalWrite(pinD, LOW);

    //Control Signal
    sendSignal("5");
  }
}

//Function to send a signal using RF module
void sendSignal(const char *controller)
{
  Serial.print("Sending => "); //debugging
  Serial.println(controller); //debugging

  driver.send((uint8_t *)controller, strlen(controller));
  driver.waitPacketSent();
}


