#include <SoftwareSerial.h>        // 블루투스 시리얼 통신 라이브러리 추가
#define BT_RXD 8
#define BT_TXD 7
#include "Wire.h"
#include <MPU6050_light.h>
SoftwareSerial bluetooth(BT_RXD, BT_TXD);        // 블루투스 설정 BTSerial(Tx, Rx)
MPU6050 mpu(Wire);
long timer = 0;
int green = 10;
int red = 11;




const byte PulsesPerRevolution = 2;
const unsigned long ZeroTimeout = 100000;
const byte numReadings = 2;

volatile unsigned long LastTimeWeMeasured_m1;
volatile unsigned long LastTimeWeMeasured_m2;
volatile unsigned long LastTimeWeMeasured_m3;
volatile unsigned long LastTimeWeMeasured_m4;

volatile unsigned long PeriodBetweenPulses_m1 = ZeroTimeout + 1000;
volatile unsigned long PeriodBetweenPulses_m2 = ZeroTimeout + 1000;
volatile unsigned long PeriodBetweenPulses_m3 = ZeroTimeout + 1000;
volatile unsigned long PeriodBetweenPulses_m4 = ZeroTimeout + 1000;

volatile unsigned long PeriodAverage_m1 = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage_m2 = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage_m3 = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage_m4 = ZeroTimeout + 1000;

unsigned long FrequencyRaw1;
unsigned long FrequencyRaw2;
unsigned long FrequencyRaw3;
unsigned long FrequencyRaw4;

unsigned long FrequencyRealm1;
unsigned long FrequencyRealm2;
unsigned long FrequencyRealm3;
unsigned long FrequencyRealm4;

unsigned long RPMm1;
unsigned long RPMm2;
unsigned long RPMm3;
unsigned long RPMm4;
unsigned int PulseCounter_m1 = 1;
unsigned int PulseCounter_m2 = 1;
unsigned int PulseCounter_m3 = 1;
unsigned int PulseCounter_m4 = 1;

unsigned long PeriodSum_m1;
unsigned long PeriodSum_m2;
unsigned long PeriodSum_m3;
unsigned long PeriodSum_m4;

unsigned long LastTimeCycleMeasure_m1 = LastTimeWeMeasured_m1;
unsigned long LastTimeCycleMeasure_m2 = LastTimeWeMeasured_m2;
unsigned long LastTimeCycleMeasure_m3 = LastTimeWeMeasured_m3;
unsigned long LastTimeCycleMeasure_m4 = LastTimeWeMeasured_m4;

unsigned long CurrentMicros = micros();//100만분의 1초

unsigned int AmountOfReadings_m1 = 1;
unsigned int AmountOfReadings_m2 = 1;
unsigned int AmountOfReadings_m3 = 1;
unsigned int AmountOfReadings_m4 = 1;

unsigned int ZeroDebouncingExtra;

unsigned long readings1[numReadings];
unsigned long readings2[numReadings];
unsigned long readings3[numReadings];
unsigned long readings4[numReadings];

unsigned long readIndex;  

unsigned long total1; 
unsigned long total2;
unsigned long total3;
unsigned long total4;

unsigned long average;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);                         // 블루투스 통신 시작
  
  attachInterrupt(digitalPinToInterrupt(2), Pulse_Event_m1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), Pulse_Event_m2, RISING);
  attachInterrupt(digitalPinToInterrupt(19), Pulse_Event_m3, RISING);
  attachInterrupt(digitalPinToInterrupt(18), Pulse_Event_m4, RISING);



  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ }      // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true);   // gyro and accelero
  Serial.println("Done!\n");

  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT); 

}
void loop() {


  mpu.update();

   unsigned long previousVal = 0;
   boolean isFalling = false;

    if(9.81*mpu.getAccZ()<=9){
      previousVal = millis();
      isFalling = true;          // Could be falling 
      
      while(millis()-previousVal <= 100){
        mpu.update();
         if(9.81*mpu.getAccZ()<=9){
          delay(10);
         }
         else{
          isFalling = false;
          digitalWrite(green,LOW);
          break;
        }
      }
    }

    if(isFalling == false){                   //1초 후에도 7이하이면 불이 들어온다
        digitalWrite(green,LOW);
      }
    else if(isFalling == true) {
        digitalWrite(green,HIGH);
      } 
  
   if(millis() - timer > 500){                   // print data every 0.5 second
    Serial.print("\nACCELERO Z: ");Serial.println(9.81*mpu.getAccZ());
    timer = millis();
   }


  

  
LastTimeCycleMeasure_m1 = LastTimeWeMeasured_m1;
LastTimeCycleMeasure_m2 = LastTimeWeMeasured_m2;
LastTimeCycleMeasure_m3 = LastTimeWeMeasured_m3;
LastTimeCycleMeasure_m4 = LastTimeWeMeasured_m4;

  CurrentMicros = micros();
  
  
  
  
  if (CurrentMicros < LastTimeCycleMeasure_m1) {
    LastTimeCycleMeasure_m1 = CurrentMicros;
  }

  if(CurrentMicros < LastTimeCycleMeasure_m2){
    LastTimeCycleMeasure_m2 = CurrentMicros;
  }
  if(CurrentMicros < LastTimeCycleMeasure_m3){
    LastTimeCycleMeasure_m3 = CurrentMicros;
  }
  if(CurrentMicros < LastTimeCycleMeasure_m4){
    LastTimeCycleMeasure_m4 = CurrentMicros;
  }
  
  
  
  FrequencyRaw1 = 10000000000 / PeriodAverage_m1;
  FrequencyRaw2 = 10000000000 / PeriodAverage_m2;
  FrequencyRaw3 = 10000000000 / PeriodAverage_m3;
  FrequencyRaw4 = 10000000000 / PeriodAverage_m4;
  
  
  
  
  if (PeriodBetweenPulses_m1 > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure_m1 > ZeroTimeout - ZeroDebouncingExtra) {
    
    FrequencyRaw1 = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }




 if (PeriodBetweenPulses_m2 > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure_m2 > ZeroTimeout - ZeroDebouncingExtra) {
    
    FrequencyRaw2 = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }


 if (PeriodBetweenPulses_m3 > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure_m3 > ZeroTimeout - ZeroDebouncingExtra) {
    
    FrequencyRaw3 = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }  

 if (PeriodBetweenPulses_m4 > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure_m4 > ZeroTimeout - ZeroDebouncingExtra) {
    
    FrequencyRaw4 = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  
  FrequencyRealm1 = FrequencyRaw1 / 10000;
  FrequencyRealm2 = FrequencyRaw2 / 10000;
  FrequencyRealm3 = FrequencyRaw3 / 10000;
  FrequencyRealm4 = FrequencyRaw4 / 10000;

  RPMm1 = FrequencyRaw1 / PulsesPerRevolution * 60;
  RPMm2 = FrequencyRaw2 / PulsesPerRevolution * 60;
  RPMm3 = FrequencyRaw3 / PulsesPerRevolution * 60;
  RPMm4 = FrequencyRaw4 / PulsesPerRevolution * 60;

  RPMm1 = RPMm1 / 10000;
  RPMm2 = RPMm2 / 10000;
  RPMm3 = RPMm3 / 10000;
  RPMm4 = RPMm4 / 10000;
  
  total1 = total1 - readings1[readIndex];
  total2 = total2 - readings2[readIndex];
  total3 = total3 - readings3[readIndex];
  total4 = total4 - readings4[readIndex];
  
  readings1[readIndex] = RPMm1;
  readings2[readIndex] = RPMm2;
  readings3[readIndex] = RPMm3;
  readings4[readIndex] = RPMm4;
  
  total1 = total1 + readings1[readIndex];
  total2 = total2 + readings2[readIndex];
  total3 = total3 + readings3[readIndex];
  total4 = total4 + readings4[readIndex];
  
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  //average = total / numReadings;

  Serial.print("\nRPMm1: ");
  Serial.print(RPMm1/2);
  Serial.print("\tRPMm2: ");
  Serial.print(RPMm2/2);
  Serial.print("\tRPMm3: ");
  Serial.print(RPMm3/2);
  Serial.print("\tRPMm4: ");
  Serial.print(RPMm4/2);


  if(RPMm1 <=200 || RPMm2 <= 200 || RPMm3 <= 200 || RPMm4 <= 200){
    if(isFalling == true){
      digitalWrite(red,HIGH);
    }
   } 



   if (bluetooth.available()) {        // 블루투스에서 보낸 내용은 시리얼모니터로 전송
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {           // 시리얼모니터에서 보낸 내용은 블루투스로 전송
    bluetooth.write(Serial.read());
  }
  

  delay(500);
}

void Pulse_Event_m1() {
  PeriodBetweenPulses_m1 = micros() - LastTimeWeMeasured_m1;
  LastTimeWeMeasured_m1 = micros();
  if (PulseCounter_m1 >= AmountOfReadings_m1)  {
    PeriodAverage_m1 = PeriodSum_m1 / AmountOfReadings_m1;
    PulseCounter_m1 = 1;
    PeriodSum_m1 = PeriodBetweenPulses_m1;

    int RemapedAmountOfReadings_m1 = map(PeriodBetweenPulses_m1, 40000, 5000, 1, 10);
    RemapedAmountOfReadings_m1 = constrain(RemapedAmountOfReadings_m1, 1, 10);
    AmountOfReadings_m1 = RemapedAmountOfReadings_m1;
  } else {
    PulseCounter_m1++;
    PeriodSum_m1 = PeriodSum_m1 + PeriodBetweenPulses_m1;
  }
}

void Pulse_Event_m2() {
  PeriodBetweenPulses_m2 = micros() - LastTimeWeMeasured_m2;
  LastTimeWeMeasured_m2 = micros();
  if (PulseCounter_m2 >= AmountOfReadings_m2)  {
    PeriodAverage_m2 = PeriodSum_m2 / AmountOfReadings_m2;
    PulseCounter_m2 = 1;
    PeriodSum_m2 = PeriodBetweenPulses_m2;

    int RemapedAmountOfReadings_m2 = map(PeriodBetweenPulses_m2, 40000, 5000, 1, 10);
    RemapedAmountOfReadings_m2 = constrain(RemapedAmountOfReadings_m2, 1, 10);
    AmountOfReadings_m2 = RemapedAmountOfReadings_m2;
  } else {
    PulseCounter_m2++;
    PeriodSum_m2 = PeriodSum_m2 + PeriodBetweenPulses_m2;
  }
}

void Pulse_Event_m3() {
  PeriodBetweenPulses_m3 = micros() - LastTimeWeMeasured_m3;
  LastTimeWeMeasured_m3 = micros();
  if (PulseCounter_m3 >= AmountOfReadings_m3)  {
    PeriodAverage_m3 = PeriodSum_m3 / AmountOfReadings_m3;
    PulseCounter_m3 = 1;
    PeriodSum_m3 = PeriodBetweenPulses_m3;

    int RemapedAmountOfReadings_m3 = map(PeriodBetweenPulses_m3, 40000, 5000, 1, 10);
    RemapedAmountOfReadings_m3 = constrain(RemapedAmountOfReadings_m3, 1, 10);
    AmountOfReadings_m3 = RemapedAmountOfReadings_m3;
  } else {
    PulseCounter_m3++;
    PeriodSum_m3 = PeriodSum_m3 + PeriodBetweenPulses_m3;
  }
}

void Pulse_Event_m4() {
  PeriodBetweenPulses_m4 = micros() - LastTimeWeMeasured_m4;
  LastTimeWeMeasured_m4 = micros();
  if (PulseCounter_m4 >= AmountOfReadings_m4)  {
    PeriodAverage_m4 = PeriodSum_m4 / AmountOfReadings_m4;
    PulseCounter_m4 = 1;
    PeriodSum_m4 = PeriodBetweenPulses_m4;

    int RemapedAmountOfReadings_m4 = map(PeriodBetweenPulses_m4, 40000, 5000, 1, 10);
    RemapedAmountOfReadings_m4 = constrain(RemapedAmountOfReadings_m4, 1, 10);
    AmountOfReadings_m4 = RemapedAmountOfReadings_m4;
  } else {
    PulseCounter_m4++;
    PeriodSum_m4 = PeriodSum_m4 + PeriodBetweenPulses_m4;
  }
}
