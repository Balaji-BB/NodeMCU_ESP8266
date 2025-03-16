const uint8_t PIR = D1;
const uint8_t LIGHT = D0;
bool PIR_state = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIR, INPUT);
  pinMode(LIGHT, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  PIR_state = digitalRead(PIR);
  digitalWrite(LIGHT, PIR_state);
  Serial.println(PIR_state);
  delay(100);
}
