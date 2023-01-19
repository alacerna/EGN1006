#include <Servo.h>
Servo myservo;
int servopin = 10;
int datalinepin = A7;
float datalinevalue;
float datavoltage;
int servologic;
int pos;
int mod = 1;
float basetest;
int basetestpin=A3;
float voltcal;
void setup() {
  // put your setup code here, to run once:
  pinMode(servopin,OUTPUT);
  pinMode(datalinepin,INPUT);
  myservo.attach(10);
  myservo.write(90);
  pos=90;
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  voltcal=analogRead(basetestpin);
  analogReference(INTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
basetest=analogRead(basetestpin);
if(basetest<300){
  digitalWrite(0,HIGH);
  digitalWrite(1,HIGH);
}


  
datalinevalue = analogRead(datalinepin);

//datavoltage = map((datalinevalue),0,1023,0,5);
if(datalinevalue<=100){ //LOW END
  servologic = 0;
}
if(datalinevalue>100 && datalinevalue<=240){//LOWER RANGE DIM BLUE
  servologic = 1;
}
if(datalinevalue>240 && datalinevalue<=744){//MID RANGE DIM ORANGE
  servologic = 0;
}
if(datalinevalue>744 && datalinevalue<=900){//HIGHER RANGE BRIGHT ORANGE
  servologic = 0;
}
if(datalinevalue>900){//HIGH END BRIGHT BLUE
  servologic = 0;
}


if(servologic==1){
  pos=pos+mod;
  myservo.write(pos);
  if(pos==180){
    mod=-1;
  }
  else if(pos==0){
    mod=1;
  }
}
delay(10);
}
