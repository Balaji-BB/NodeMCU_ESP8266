/*
 * Blynk Credentials
 */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3Vs3kd1oT"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "11QskSaWE4nsvu_xP-UusSW3VAKLbHs8"
//#define BLYNK_TEMPLATE_ID "TMPL389NBaJtN"
//#define BLYNK_TEMPLATE_NAME "Home Automation"
//#define BLYNK_AUTH_TOKEN "ARQfutgPDyiYNBUTN4wU7aJmXJzSn3wR"
/*
 * Macros for Timeout
 */
 #define WiFi_timeout_ms        3000UL //3 Seconds Timeout
 /*
  * List of user macros
  */
  #define WIFI_OK               1
  #define WIFI_NOT_OK           0
  #define BLYNK_OK              1
  #define BLYNK_NOT_OK          0
/*
 * List of Header Files
 */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
/*
 * WiFi Credentials
 */
char ssid[] = "Nagu_home"; // Your own WiFi SSID
char pass[] = "Nagu@2208Bala@0704"; // Your own WiFi Password
/*
 * Pin Assignment
 */
const uint8_t LIGHT = D2;
const uint8_t PIR = D7;
const uint8_t LDR = A0;
/*
 * Variable Declarations
 */
static uint32_t start_ms = 0;
uint32_t WiFi_wait_ms = 0;
uint16_t LDR_data = 0;
uint16_t light_intensity = 750;
uint8_t light_intensity_high_cnt = 0;
/*
 * Flag/State Declarations
 */
bool PIR_detection = false;
bool LIGHT_flag = false;
bool light_blynk_state = false;
bool blynk_update_flag = true;
bool ldr_blynk_state = false;

bool Blynk_Init(void)
{
  Serial.println("_____BLYNK CONNECTION BEGINS_____");
  Blynk.config(BLYNK_AUTH_TOKEN); 
  Blynk.connect(5000);
  if(Blynk.connected())
  {
    Serial.println("---BLYNK connected---");
//    Blynk.virtualWrite(V3, LDR_ADC);
    return BLYNK_OK;
  }
  else
  {
    Serial.println("XXXXX BLYNK not connected XXXXX");
    return BLYNK_NOT_OK;
  }
}  

bool WiFi_Init(void)
{
  WiFi.begin(ssid,pass);
  start_ms = millis();
  Serial.println("Connecting to WiFI");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    WiFi_wait_ms = millis();
    delay(500);
    if((WiFi_wait_ms - start_ms) > WiFi_timeout_ms)
    {
      Serial.println("");
      Serial.println("*****WiFi Connection Failure*****");
      break;  
    }
  }
  if(WiFi.status() == WL_CONNECTED)
  {
     Serial.println("");
    Serial.println("*****WiFi Connection Success*****");
    return WIFI_OK;  
  }
  else
  {
    return WIFI_NOT_OK;  
  } 
}
void setup() 
{
  // GPIO configurations
  pinMode(PIR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(LIGHT, OUTPUT);
  digitalWrite(LIGHT, LOW);
  // Serial UART configuration
  Serial.begin(9600);
//  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Blocks execution in case of not connecting to the WiFi Connection
  if(WiFi_Init() == WIFI_OK)
  {
    Blynk_Init();
    LDR_data = analogRead(LDR); 
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, LDR_data);
  }
  delay(1000);
}

void loop() 
{
  if(Blynk.connected())
  {
    Serial.println("...BLYNK connected...");
    Blynk.run();
  }
  else
  {
    Serial.println("...BLYNK not connected...");
     if(WiFi.status() != WL_CONNECTED)
     {
//      start_ms = millis();
      Serial.println("WiFi disconnected after sometime");
      if(WiFi_Init() == WIFI_OK)
      {
       Blynk_Init();
       LDR_data = analogRead(LDR); 
       Blynk.virtualWrite(V2, 0);
       Blynk.virtualWrite(V3, LDR_data);
       Blynk.run();
      }
     }
     else
     {
       Blynk_Init();
       LDR_data = analogRead(LDR); 
       Blynk.virtualWrite(V2, 0);
       Blynk.virtualWrite(V3, LDR_data);
       Blynk.run();
     }
     
  }
  light_control();
//  delay(3000);
}
void light_control(void)
{
  PIR_detection = digitalRead(PIR);  // Reads PIR Sensor
  Serial.print("PIR Status : ");
  Serial.println(PIR_detection);
  LDR_data = analogRead(LDR);        // Reads LDR Sensor analog value
  Serial.print("LDR Digital Value : ");
  Serial.println(LDR_data);
  if(light_blynk_state == HIGH)
  {
    
  } 
  else
  {
    if(PIR_detection == true)
    {
        if(LDR_data<light_intensity)
        {
          LIGHT_flag = true;
          light_intensity_high_cnt = 0;
          LIGHT_ON();
          Serial.println("Light ON through PIR & LDR");
        }
        else if(LDR_data>light_intensity)
        {
          if(ldr_blynk_state == false)
          {
            light_intensity_high_cnt++;
            if(light_intensity_high_cnt == 10)
            {
              light_intensity_high_cnt = 0;
              LIGHT_OFF();  
              Serial.println("Light OFF through PIR & LDR");
            }
          }
          else
          {
              ldr_blynk_state = false;
              LIGHT_OFF();  
              Serial.println("Light OFF through PIR & LDR");  
          }
        }
    }
    else
    {
      LIGHT_OFF();  
      Serial.println("Light OFF through PIR");
    }
  }
}
BLYNK_WRITE(V0)
{
  light_blynk_state = param.asInt();
  if(light_blynk_state == true)
  {
    LIGHT_ON();
    Serial.println("Light ON through blynk dashboard");
  }
  else if(LIGHT_flag == false)
  {
     LIGHT_OFF();
     Serial.println("Light OFF through blynk dashboard"); 
  }
}
BLYNK_WRITE(V1)
{
  light_intensity = param.asInt();
  ldr_blynk_state = true;
  LDR_data = analogRead(LDR);
  Blynk.virtualWrite(V3, LDR_data);
  Serial.print("Set Intensity Level through blynk :");
  Serial.println(light_intensity);
}
void LIGHT_ON(void)
{
  digitalWrite(LIGHT, HIGH);
  if(blynk_update_flag == true)
  {
    blynk_update_flag = false;
    Blynk.virtualWrite(V2, 127);
    Serial.println("Light ON blynk update");
  }
}
void LIGHT_OFF(void)
{     
  digitalWrite(LIGHT, LOW);
  if(blynk_update_flag == false)
  {
    blynk_update_flag = true;
    Blynk.virtualWrite(V2, 0);
    Serial.println("Light OFF blynk update");
  }
}
