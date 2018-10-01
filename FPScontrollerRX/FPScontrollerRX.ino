#include <SoftwareSerial.h>
#include <Keyboard.h>
#include <Mouse.h>

SoftwareSerial BT(10,11);
String readData;
boolean DataEnd = false;
uint8_t weapon = 1;
char weaponKey;
unsigned long weaponTime = millis();

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  
  if(BT.available()) {
    char data = (char)BT.read();
    if(data == '\r') {
      char tmpData = BT.read();
      Serial.println("end");
      DataEnd = true;
    } else {
      readData += data;
    }
  }
  
  if(DataEnd) {
    Serial.println(readData+String("\n"));
    if(readData[1] == '1') Mouse.click(MOUSE_LEFT);
    if(readData[2] == '1') Mouse.click(MOUSE_RIGHT);
    if(readData[3] == '1') Keyboard.write('r');
    if(readData[4] == '1') Keyboard.write('z');
    if(readData[5] == '1') Keyboard.write(KEY_LEFT_CTRL);
    if(weaponTime + 200 < millis()) {
      switch(readData[6]) {
        case '1':
          weapon == 4 ? weapon = 1 : weapon += 1;
          weaponKey = weapon + 48;
          Keyboard.write(weaponKey);
          weaponTime = millis();
        break;
        case '2':
          weapon == 1 ? weapon = 4 : weapon -= 1;
          weaponKey = weapon + 48;
          Keyboard.write(weaponKey);
          weaponTime = millis();
        break;
      } 
    }
    if(readData[7] == '1') Keyboard.write('w');
    if(readData[8] == '1') Keyboard.write('s');
    if(readData[9] == '1') Keyboard.write('a');
    if(readData[10] == '1') Keyboard.write('d');
    if(readData[11] == '1') Keyboard.write(' ');
    if(readData[12] != ' ') Keyboard.write(readData[12]);

    // 단락 구분
    int first = readData.indexOf(",");
    int second = readData.indexOf(",",first+1);
    int strlength = readData.length();
    String str2 = readData.substring(first+1, second);
    String str3 = readData.substring(second+1,strlength);
   
   int16_t gyroX = str2.toInt();
   int16_t gyroZ = str3.toInt();

   Mouse.move(gyroX-0,gyroZ-1.5); // gyro - X 에서 X는 gyro 보정값
   
    DataEnd = false;
    readData = "";
  }
}
