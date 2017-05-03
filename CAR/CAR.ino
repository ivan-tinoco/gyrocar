/*  ME106 Term Project
*  SJSU - Spring 2017
*  Hand Gesture Controlled Robotic Car*/

/*Code to control DC-Motors, RF Receiver, Unltasonic Sensor and Servo*/
//CAR-MODULE
//Code to control DC-Motors and RF Receiver

//DAC/ADC Library used for RF Transmission
#include <RH_ASK.h>         //(Uses pin 11 to read data)
#include <SPI.h>            //Not actually used but needed to compile (RH_ASK.h Library specified)

RH_ASK driver;              //Create radio-head object to control RF Transmission

//Ultrasonic sensor control
//ports:
int Echo = A4;
int Trig = A5;
//gloval variables:
int rightDistance = 0;
int leftDistance = 0;
int middleDistance = 0 ;

//Servo control
//port
int servo = 2;
//global variables
int angle;
int pwm;

//DC-motor driver control
//ports
int in1=9;
int in2=8;
int in3=7;
int in4=6;
int ENA=5;
int ENB=3;
//global variables
int ctrlSpeed1 = 150;
int ctrlSpeed2 = 120;

//TX Control counter
int count = 0;

//Car-movement control functions:
void _mForward()
{ 
  analogWrite(ENA,ctrlSpeed1);
  analogWrite(ENB,ctrlSpeed2);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("forward"); //debugging
}
void _mBack()
{
  analogWrite(ENA,ctrlSpeed2);
  analogWrite(ENB,ctrlSpeed2);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("back"); //debugging
}
void _mleft()
{
  analogWrite(ENA,ctrlSpeed2);
  analogWrite(ENB,ctrlSpeed2);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("left"); //debugging
}
void _mright()
{
  analogWrite(ENA,ctrlSpeed2);
  analogWrite(ENB,ctrlSpeed2);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("right");  //debugging
}
void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("STOP"); //debugging
}

//Ultrasonic distance measurement
int Distance_test()   
{
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance/58;       
  return (int)Fdistance;
} 

//Servo control sub-function 
void myServo(int servo, int angle)
{
  for(int i=0; i<10; i++)
  {
    pwm = (angle*11) + 500;      // Convert angle to microseconds
    digitalWrite(servo, HIGH);
    delayMicroseconds(pwm);
    digitalWrite(servo, LOW);
    delay(50);                   // Refresh cycle of servo
  }
}

//Initial setup I/O + Serial Comm + RF Comm
void setup()
{
  pinMode(servo, OUTPUT);

  //Ultrasonic sensor pins
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT); 

  //Motor-driver control pins
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);

  //Serial Comm use to debug
  Serial.begin(9600);
  
  _mStop();

  //RF module initialize:
  if (!driver.init()) Serial.println("init failed");
}

void loop()
{
  //Servo + obstacle checking

  
  //Radio-head library ADC processing:
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  //if RF signal recieved:
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    count = 0; //initialize counter
    Serial.println(buf[0]); //debugging
    
    if(buf[0]=='0')
    {
      _mForward();
      Serial.print("SERVO CHECK");  //debugging
      myServo(servo, 65);//set servo position
      middleDistance = Distance_test(); //get obstacle distance
      #ifdef send
      Serial.print("middleDistance=");  //debugging
      Serial.println(middleDistance);   //debugging
      #endif
      
      if(middleDistance<=50) // detects obstacle and decides where to turn
      {     
        _mStop();
        delay(500);                         
        myServo(servo, 0);          
        delay(1000);      
        rightDistance = Distance_test();

        #ifdef send
        Serial.print("rightDistance=");
        Serial.println(rightDistance);
        #endif
        
        delay(500);
        myServo(servo, 90);              
        delay(1000);                                                  
        myServo(servo, 180);              
        delay(1000); 
        leftDistance = Distance_test();
        
        #ifdef send
        Serial.print("leftDistance=");
        Serial.println(leftDistance);
        #endif
        
        delay(500);
        myServo(servo, 90);              
        delay(1000);
        if(rightDistance>leftDistance)  
        {
          _mright();
          delay(180);
        }
        else if(rightDistance<leftDistance)
        {
          _mleft();
          delay(180);
        }
        else if((rightDistance<=20)||(leftDistance<=20))
        {
          _mBack();
          delay(180);
        }
        else
        {
          _mForward();
        }
      }  
      else
        _mForward();  
    } 
    else if(buf[0]=='1')
    {
      _mBack();
    }  
    else if(buf[0]=='2')
    {
      _mright();
    }
    else if(buf[0]=='3')
    {
      _mleft();
    }   
    else if(buf[0]=='4')
    { 
      _mStop();
    }
    else
    {
      _mStop();
    }
  }
  else //signal not-received, increment counter
  {
    count++;
  }

//  Serial.println(count); //debugging
  if (count > 200 ) //Transmission lost
  {
    _mStop();
  }
} 

