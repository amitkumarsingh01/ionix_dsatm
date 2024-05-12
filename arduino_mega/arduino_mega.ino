#define CH1 8
#define CH2 9
#define CH3 10
#define CH4 11
#define CH5 12
#define CH6 13
#define M1 43
#define M2 45
#define M3 7
#define M4 41
#define L1 3 
#define L2 25
#define L3 27
#define L4 4
#define L5 31
#define L6 33
#define L7 35
#define L8 37
#define trigPin 6
#define echoPin 5
#include<Servo.h>
Servo servo,servo2;
int ch1Value;
int ch2Value;
int ch3Value;
int ch4Value; 
int ch5Value;
int h;
bool ch6Value;
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}
void setup(){
  Serial.begin(9600);
  servo.attach(44);
  servo2.attach(46);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(M3,OUTPUT);
  pinMode(M4,OUTPUT);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(L4,OUTPUT);
  pinMode(L5,OUTPUT);
  pinMode(L6,OUTPUT);
  pinMode(L7,OUTPUT);
  pinMode(L8,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(2,1);
}
long getDistance()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (distance >= 200 || distance <= 0) return -1;
  else  return distance;
}
void left()
{
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  digitalWrite(M3,HIGH);
  digitalWrite(M4,LOW);
}
void right()
{
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
  digitalWrite(M3,LOW);
  digitalWrite(M4,HIGH);
}
void forward()
{
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  digitalWrite(M3,LOW);
  digitalWrite(M4,HIGH);
}
void back()
{
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
  digitalWrite(M3,HIGH);
  digitalWrite(M4,LOW);
}
void stop()
{
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
  digitalWrite(M3,LOW);
  digitalWrite(M4,LOW);
}
void LUp()
{
  digitalWrite(25,1);
  digitalWrite(3,0);
  digitalWrite(27,0);
  digitalWrite(4,1);
  digitalWrite(31,0);
  digitalWrite(33,1);
  digitalWrite(35,0);
  digitalWrite(37,1);
}
void LDown()
{
  digitalWrite(25,0);
  digitalWrite(3,1);
  digitalWrite(27,1);
  digitalWrite(4,0);
  digitalWrite(31,1);
  digitalWrite(33,0);
  digitalWrite(35,1);
  digitalWrite(37,0);
}
void LStop()
{
  digitalWrite(25,0);
  digitalWrite(3,0);
  digitalWrite(27,0);
  digitalWrite(4,0);
  digitalWrite(31,0);
  digitalWrite(33,0);
  digitalWrite(35,0);
  digitalWrite(37,0);
}
void loop() {
  if(Serial.available())
  {
      h=Serial.readString().toInt();
      int mapVal=map(h,0,480,0,20);
      while (mapVal<getDistance())
      {
        LUp();
      }
      LStop();
  }
  ch1Value = readChannel(CH1, 0, 176, 88);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, -100);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch5Value = readChannel(CH5, -100, 100, 0);
  ch6Value = readSwitch(CH6, false);
  
  if(ch6Value==0)
  {
    stop();
    LStop();
    servo.write(88);
    servo2.write(88);
    if (ch2Value>=75)
    {
      forward();
    }
    else if(ch2Value<=-75)
    {
      back();
    }
    else if(ch1Value>=100)
    {
      right();
    }
    else if(ch1Value<=55)
    {
      left();
    }
    else
    { 
      stop();
    }
  }
  else
  {
    stop();
    LStop();
    servo.write(ch1Value);
    servo2.write(ch1Value);
    if(ch2Value>=75)
      LUp();
    else if(ch2Value<=-75)
      LDown();
    else
      LStop();
    if(ch3Value>=55)
      forward();
    else
      stop();
  }
}
