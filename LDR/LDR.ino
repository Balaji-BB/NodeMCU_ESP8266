#define LDR   A0
#define LIGHT_INTENSITY 50
//#define LED_BUILTIN D2
uint16_t LDR_ADC = 0;
void setup() {
  pinMode(LDR, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, LOW); 
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  LDR_ADC = analogRead(LDR);
  if(LDR_ADC < LIGHT_INTENSITY)
  {
    digitalWrite(LED_BUILTIN, LOW);  
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);  
  }
  Serial.println(LDR_ADC);
  delay(1000);
  
}
