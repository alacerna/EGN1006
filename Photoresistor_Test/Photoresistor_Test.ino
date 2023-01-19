#include <Servo.h>
Servo myservo;
int val;
int valpin = A6;
int servopin = 10;
int cal;
int deadband=50;
float x;
int pos;
float kp=0.025;
int response_time = 20;

void setup() {
  // put your setup code here, to run once:
pinMode(valpin,INPUT);
pinMode(servopin,OUTPUT);
delay(200);
cal = analogRead(valpin);
delay(200);
myservo.attach(10);
myservo.write(90);
pos=90;

}

void loop() {
  // put your main code here, to run repeatedly:
val = analogRead(valpin);
if(val<cal-deadband){
  if(pos<=180){
    pos=pos+floor((abs(val-cal)*kp));
  }
}
if(val>cal+deadband){
  if(pos>=0){
    pos=pos-floor((abs(val-cal)*kp));
  }
}
myservo.write(pos);

delay(response_time);
}
