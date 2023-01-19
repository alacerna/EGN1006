//Inlclude Libraries
#include <ESP32Servo.h>

//Define variables related to Servo
Servo gatorHead;
int servoPin;
int servoPos;
float kp;
float deadband;
int servoLogic;
int responseTime;

//Define variables related to Flashlight
int photoresistorDataPin;
int photoresistorData;
int nuclearDataPin;
int nuclearData;
int flashlightDataPin;
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
}

void loop() {
  // put your main code here, to run repeatedly:
  nuclearData = digitalRead(nuclearDataPin);
  flashlightData = analogRead(flashlightDataPin);
  photoresistorData = analogRead(photoresistorDataPin);
  deadbandData = analogRead(deadbandPotPin);
  sensitivityData = analogRead(sensitivityPotPin);

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
    if (flashlightData <= 100) {  //LOW END
      servoLogic = 0;
    }
    if (flashlightData > 100 && flashlightData <= 240) {  //LOWER RANGE DIM BLUE
      servoLogic = 0;
    }
    if (flashlightData > 240 && flashlightData <= 744) {  //MID RANGE DIM ORANGE
      servoLogic = 0;
    }
    if (flashlightData > 744 && flashlightData <= 900) {  //HIGHER RANGE BRIGHT ORANGE
      servoLogic = 1;
    }
    if (flashlightData > 900) {  //HIGH END BRIGHT BLUE
      servoLogic = -1;
    }

    //Servo Control Logic
    if (photoresistorData < ambientCal - deadband) {
      if (servoPos <= 180) {
        servoPos = servoPos + servoLogic*(floor((abs(photoresistorData - ambientCal) * kp)));
      }
    }
    if (photoresistorData > ambientCal + deadband) {
      if (servoPos >= 0) {
        servoPos = servoPos - servoLogic*(floor((abs(photoresistorData - ambientCal) * kp)));
      }
    }
    gatorHead.write(servoPos);
    delay(responseTime);
  }
}