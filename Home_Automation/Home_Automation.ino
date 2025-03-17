/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  NOTE: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LIGHT_VIRTUAL_PIN V0
/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"
#define BLYNK_TEMPLATE_ID "TMPL389NBaJtN"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "ARQfutgPDyiYNBUTN4wU7aJmXJzSn3wR"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Airtel_Embedded_home";
char pass[] = "Embedded@1298";

const uint8_t PIR = D8;
const uint8_t LDR = A0;
const uint8_t LIGHT = D4;
bool PIR_state = false;
bool LIGHT_flag = false;
uint16_t LDR_ADC = 0;
uint16_t LIGHT_INTENSITY = 50;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(LIGHT, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("MAIN");
   Blynk.run();
  PIR_state = digitalRead(PIR);
  LDR_ADC = analogRead(LDR);
  if(((LDR_ADC < LIGHT_INTENSITY) && (PIR_state == true)) || LIGHT_flag)
  {
    digitalWrite(LIGHT, HIGH);
    Blynk.virtualWrite(V2, 127);
  }
  else
  {
    digitalWrite(LIGHT, LOW);
    Blynk.virtualWrite(V2, 0);  
  }
  Serial.print("LDR_ADC");
  Serial.println(LDR_ADC);
  Serial.print("PIR_state");
  Serial.println(PIR_state);
  delay(1000);
}
BLYNK_WRITE(V0)
{
  bool LIGHT_STATE = param.asInt();
  if(LIGHT_STATE == 1)
  {
    LIGHT_flag = true;
  }
  else
  {
    LIGHT_flag = false;
  }
  Serial.print("Light pin state :");
  Serial.println(LIGHT_STATE);
}
BLYNK_WRITE(V1)
{
  LIGHT_INTENSITY = param.asInt();
  Serial.print("Set Intensity Level :");
  Serial.println(LIGHT_INTENSITY);
}
