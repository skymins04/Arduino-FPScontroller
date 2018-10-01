#include <Wire.h>
#include <SoftwareSerial.h>

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
unsigned long jumpLimit = millis();
boolean jumpOK = false;

SoftwareSerial BT(2,3);

//digital Keys
const uint8_t SHOT = 4;
const uint8_t ZOOM = 5;
const uint8_t RELOAD_AMA = 6;
const uint8_t RELOAD_PRO = 7;
const uint8_t RUN = 8;
const uint8_t SQUAT = 9;
const uint8_t CHANGE_M = 10;
const uint8_t CHANGE_P = 11;

//analog JoySticks
const uint8_t SIGHT_X = 6;
const uint8_t SIGHT_Y = 7;
const uint8_t MOV_X = 1;
const uint8_t MOV_Y = 0;
const uint8_t JUMP = 2;

void getGyro() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,14,true);
  
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  Tmp=Wire.read()<<8|Wire.read();
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();
}

boolean readBtn(uint8_t pin) {
  for(int i = 0; i < 100; i++) {
    if(analogRead(pin) > 80) return false;   
  }
  return true;
}

void setup() {
  Serial.begin(9600); //while(!Serial) {}
  BT.begin(9600);
  
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.println("System Ready!!");
}

void loop() {
  String SendData;
  char shotKey = '0', zoomKey = '0', reloadKey = '0', runKey = '0', squatKey = '0', changeKey = '0', movW = '0', movS = '0', movA = '0', movD = '0', jumpKey = '0', LeanKey = ' ';
  int16_t gyroX, gyroZ;
  
  getGyro();
  int Sensitivity = 100;
  gyroX = GyX / Sensitivity / 1.1 * -1;
  gyroZ = GyZ / Sensitivity * -1;

  if (analogRead(SIGHT_X) < 400) {
    gyroZ += map(analogRead(SIGHT_X),400,0,10,20);
  }
  if (analogRead(SIGHT_X) > 600) {
    gyroZ += map(analogRead(SIGHT_X),600,1023,10,20)*-1;
  }
  if (analogRead(SIGHT_Y) < 400) {
    gyroX += map(analogRead(SIGHT_Y),400,0,10,20)*-1;
  }
  if (analogRead(SIGHT_Y) > 600) {
    gyroX += map(analogRead(SIGHT_Y),600,1023,10,20);
  }

  if(digitalRead(SHOT) == 1) shotKey = '1';
  if(digitalRead(ZOOM) == 1) zoomKey = '1';
  if(digitalRead(RELOAD_AMA) == 1 || digitalRead(RELOAD_PRO) == 1) reloadKey = '1';
  if(digitalRead(RUN) == 1) runKey = '1';
  if(digitalRead(SQUAT) == 1) squatKey = '1';
  if(digitalRead(CHANGE_P) == 1) changeKey = '1';
  else if(digitalRead(CHANGE_M) == 1) changeKey = '2';

  if (analogRead(MOV_X) < 300) movW = '1';
  if (analogRead(MOV_X) > 700) movS = '1';
  if (analogRead(MOV_Y) < 300) movA = '1';
  if (analogRead(MOV_Y) > 700) movD = '1';
  if(readBtn(JUMP) == true) {
    if(jumpLimit+200 < millis()) jumpOK = true;
    if(jumpOK) {
      jumpKey = '1';
      jumpOK = false;
      jumpLimit = millis();
    }
  }

  if(AcZ < -4000) LeanKey = 'e';
  if(AcZ > 4000) LeanKey = 'q';

  SendData += shotKey;
  SendData += zoomKey;
  SendData += reloadKey;
  SendData += runKey;
  SendData += squatKey;
  SendData += changeKey;
  SendData += movW;
  SendData += movS;
  SendData += movA;
  SendData += movD;
  SendData += jumpKey;
  SendData += LeanKey;
  SendData += ',';
  SendData += String(gyroX);
  SendData += ',';
  SendData += String(gyroZ);

  Serial.println(SendData);
  BT.println(SendData);
  //delay(10);
}
