#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16, 2);

const int interruptPin = 2;

volatile int pre_count, count;
volatile int 
unsigned long skip_micro_time = 100;
unsigned long cur_time = 0;
unsigned long pre_time = 0;

unsigned long skip_rpm_micro_time = 4000;
unsigned long cur_rpm_time = 0;
unsigned long pre_rpm_time = 0;

void setup() { 
  Serial.begin(9600); // 시리얼 통신, 속도 9600
  count = 0;
  pre_count = 0;
  attachInterrupt(digitalPinToInterrupt(interruptPin), CountServoRotation, FALLING);
  Serial.println("start");
}

void loop() {
  cur_rpm_time = micros();
  if((cur_rpm_time - pre_rpm_time) > skip_rpm_micro_time){
    Serial.print("Count is ");
    count = count/3;
    Serial.println(count);
    Serial.print("Present RPM #:\t");
    Serial.println(count*15);
    count = 0;
    pre_rpm_time = micros();
    delay(500);
  }
}

void CountServoRotation(){
  cur_time = micros();
  if((cur_time - pre_time)>skip_micro_time){
    count++;
  }
  pre_time == millis();
}
