#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
 
#define SCL 8     // SCL 연결한 핀
#define SDA 9     // SDA 연결한 핀
#define CSB 10    // CSB 연결한 핀
#define SDO 11    // SDO 연결한 핀
 
Adafruit_BMP280 bmp(CSB,SDA,SDO,SCL);
 
void setup() {
  Serial.begin(9600);
  
  if (!bmp.begin()) {  // 센서가 인식되지 않는 경우
    Serial.println(F("센서가 인식되지 않습니다. 연결 상태를 확인해주세요."));
    while (1);
  }
}
 
void loop() {
  delay(100);
  Serial.println("================================");
  
  Serial.print(F("온도 = "));
  Serial.print(bmp.readTemperature());      // 센서로 온도 측정해서 출력
  Serial.println(" *C");
    
  Serial.print(F("기압 = "));
  Serial.print(bmp.readPressure());        // 센서로 기압 측정해서 출력
  Serial.println(" Pa");
 
  Serial.print(F("고도 = "));
  Serial.print(bmp.readAltitude(1006.4)); // 센서로 고도 측정해서 출력
  Serial.println(" m");
 
  Serial.println("================================\n");
  delay(2000);
}
