//Inlclude Libraries
#include <Servo.h>

//Define variables related to Servo
Servo gatorHead;
int servoPin = 9;
int servoPos;
float kp = 0.01;
float deadband = 40;
int servoLogic;
int responseTime = 20;

//Define variables related to Flashlight
int photoresistorDataPin = A2;
int photoresistorData;
int nuclearDataPin = 6;
int nuclearData = 0;
int flashlightDataPin = A6;
int flashlightData;
int ambientCal;

//Define variables related to Poteniometers
int deadbandPotPin;
int deadbandData;
int sensitivityPotPin;
int sensitivityData;

void setup() {
  //Define pinModes
  pinMode(servoPin, OUTPUT);

  pinMode(photoresistorDataPin, INPUT);
  pinMode(nuclearDataPin, INPUT);
  pinMode(flashlightDataPin, INPUT);

  pinMode(deadbandPotPin, INPUT);
  pinMode(sensitivityPotPin, INPUT);

  //Attach Servo
  gatorHead.attach(servoPin);

  //Calibrate all tools
  gatorHead.write(90);
  servoPos = 90;
  delay(500);
  ambientCal = analogRead(photoresistorDataPin);
  delay(500);

  //FOR DEBUGGING
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  nuclearData = digitalRead(nuclearDataPin);
  flashlightData = analogRead(flashlightDataPin);
  photoresistorData = analogRead(photoresistorDataPin);
  deadbandData = analogRead(deadbandPotPin);
  sensitivityData = analogRead(sensitivityPotPin);

  Serial.println(photoresistorData);

  handleGatorHead(nuclearData, flashlightData, photoresistorData, deadbandData, sensitivityData, responseTime);
}

void handleGatorHead(int nuclearData, int flashlightData, int photoresistorData, int deadbandData, int sensitivityData, int responseTime) {
  if (nuclearData == HIGH) {
    //gatorHead goes crazy
    servoPos = 0;
    gatorHead.write(servoPos);
    delay(500);
    servoPos = 180;
    gatorHead.write(servoPos);
    delay(500);

  } else {
    //gatorHead does normal things
    //Light Control Logic
    if (flashlightData <= 30) {  //LOW END
      servoLogic = 0;
    }
    if (flashlightData > 30 && flashlightData <= 55.5) {  //LOWER RANGE DIM BLUE
      servoLogic = 0;
    }
    if (flashlightData > 55.5 && flashlightData <= 200) {  //MID RANGE DIM ORANGE
      servoLogic = 0;
    }
    if (flashlightData > 200 && flashlightData <= 234) {  //HIGHER RANGE BRIGHT ORANGE
      servoLogic = 1;
    }
    if (flashlightData > 234) {  //HIGH END BRIGHT BLUE
      servoLogic = -1;
    }
    

    //Servo Control Logic
    if (photoresistorData < ambientCal - deadband) {
      
        servoPos = servoPos + servoLogic*(floor((abs(photoresistorData - ambientCal) * kp)));
        if (servoPos >=180){
          servoPos = 180;
        }
      
    }
    if (photoresistorData > ambientCal + deadband) {
      
        servoPos = servoPos - servoLogic*(floor((abs(photoresistorData - ambientCal) * kp)));
        if (servoPos <= 0){
          servoPos = 0;
        }
      
    }
    Serial.println(servoPos);
    gatorHead.write(servoPos);
    delay(responseTime);
  }
}
