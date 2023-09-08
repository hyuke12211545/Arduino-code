int raw_X;
int raw_Y;
int raw_Z;
double acc_X;
double acc_Y;
double acc_Z;
void setup() {
  Serial.begin(9600); // 시리얼 통신
}
void loop() {
  raw_X = analogRead(14); // 변수 val_X에 A0값 대입
  raw_Y = analogRead(15); // 변수 val_Y에 A1값 대입
  raw_Z = analogRead(16); // 변수 val_Z에 A2값 대입
  // x, y, z값 출력
  acc_X=(raw_X);
  acc_Y=(raw_Y);
  acc_Z=(raw_Z);
  Serial.print("X:");
  Serial.print(acc_X);
  Serial.print("  Y:");
  Serial.print(acc_Y);
  Serial.print("  Z:");
  Serial.println(acc_Z);
  Serial.println();
  delay(100);
}
