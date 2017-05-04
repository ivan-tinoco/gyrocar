/*  ME106 Term Project
   SJSU - Spring 2017
   Hand Gesture Controlled Robotic Car*/

#include <Servo.h> //servo library

Servo myservo; // create servo object to control servo

int Echo = A4;
int Trig = A5;
int in1 = 9;
int in2 = 8;
int in3 = 7;
int in4 = 6;
int ENA = 5;
int ENB = 3;
int ABS = 150;
int ctrlSpeed2 = 120;

int rightDistance = 0, leftDistance = 0, middleDistance = 0 ;
void _mForward()
{
  analogWrite(ENA, ABS);
  analogWrite(ENB, ctrlSpeed2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("go forward!");
}

void _mBack()
{
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("go back!");
}

void _mleft()
{
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("go left!");
}

void _mright()
{
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("go right!");
}
void _mStop()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}
/*Ultrasonic distance measurement Sub function*/
int Distance_test()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = Fdistance / 58;
  return (int)Fdistance;
}

void setup()
{
  myservo.attach(2);// attach servo on pin 2 to servo object
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  _mStop();
}

void loop()
{
  myservo.write(90);//setservo position according to scaled value
  //    delay(500);
  middleDistance = Distance_test();
  //    #ifdef send
  Serial.print("middleDistance=");
  Serial.println(middleDistance);
  //    #endif

  if (middleDistance <= 30)
  {
    _mStop();
    delay(500);
    myservo.write(5);
    delay(1000);
    rightDistance = Distance_test();

    //      #ifdef send
    Serial.print("rightDistance=");
    Serial.println(rightDistance);
    //      #endif

    delay(500);
    myservo.write(90);
    delay(1000);
    myservo.write(180);
    delay(1000);
    leftDistance = Distance_test();

    //      #ifdef send
    Serial.print("leftDistance=");
    Serial.println(leftDistance);
    //      #endif

    delay(500);
    myservo.write(90);
    delay(1000);
    if (rightDistance > leftDistance)
    {
      for (int i = 0; i < 5; i++)
      {
        _mright();
        delay(180);
      }
    }
    else if (rightDistance < leftDistance)
    {
      for (int i = 0; i < 5; i++)
      {
        _mleft();
        delay(180);
      }
    }
    else if ((rightDistance <= 20) || (leftDistance <= 20))
    {
      for (int i = 0; i < 2; i++)
      {
        _mBack();
        delay(180);
      }
      for (int i = 0; i < 8; i++)
      {
        _mright();
        delay(180);
      }
    }
    else
    {
      _mForward();
    }
  }
  else
    _mForward();
}

