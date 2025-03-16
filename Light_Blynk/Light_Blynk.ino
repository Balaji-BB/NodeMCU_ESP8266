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
#define LED_BUILTIN D2
#define LIGHT_VIRTUAL_PIN V0
#define LIGHT_PIN D0
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

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}
BLYNK_WRITE(V0)
{
  bool LIGHT_STATE = param.asInt();
  if(LIGHT_STATE == 1)
  {
    digitalWrite(LIGHT_PIN, HIGH);  
  }
  else
  {
    digitalWrite(LIGHT_PIN, LOW);  
  }
  Serial.print("Light pin state :");
  Serial.println(LIGHT_STATE);
}
