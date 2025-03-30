#define BLYNK_TEMPLATE_ID "BLYNK ID"
#define BLYNK_TEMPLATE_NAME "BLYNK NAME"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP_Mail_Client.h>

// WiFi Credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
char auth[] = "";  // Blynk Auth Token
char ssid[] = "";  // Your WiFi SSID
char pass[] = "";  // Your WiFi password
// SMTP Credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465 // SSL port
#define AUTHOR_EMAIL "" //SENDER MAIL ID
#define AUTHOR_PASSWORD "" //SENDER MAIL APP PASSWORD
#define RECIPIENT_EMAIL "" RECEIVER MAIL ID
#define firesensor D5  // Fire sensor connected to pin D5
#define alarm D6  // Alarm connected to pin D6
#define led D7  // LED connected to pin D7
// SMTP Session
SMTPSession smtp;

void setup() {
      pinMode(firesensor, INPUT_PULLUP);  // Set the fire sensor pin as input
    pinMode(alarm, OUTPUT);  // Set alarm pin as output
    pinMode(led, OUTPUT);  // Set LED pin as output
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
    Serial.println("\nConnected to WiFi!");

//    sendEmail();
}

void loop() {
  Blynk.run(); 
    // Add event triggers here (e.g., motion detection)
        if (digitalRead(firesensor) == LOW)  // Check if fire is detected (LOW indicates fire)
    {
        String b = "FIRE DETECTED";
        Blynk.virtualWrite(V0, b);  // Display on Blynk app
        
        digitalWrite(alarm, HIGH);  // Turn on alarm
        digitalWrite(led, HIGH);  // Turn on LED
        Serial.println("Fire Detected");
        sendEmail();  // Send email
    }
    else
    {
        String b = "FIRE NOT DETECTED";
        Blynk.virtualWrite(V0, b);  // Display on Blynk app
      Serial.println("Fire not Detected");
        digitalWrite(alarm, LOW);  // Turn off alarm
        digitalWrite(led, LOW);  // Turn off LED
    }
    delay(1000);
}
void sendEmail() {
    smtp.debug(1);
    smtp.callback(smtpCallback);

    ESP_Mail_Session session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";

    SMTP_Message message;
    message.sender.name = "Fire Detection Alert System";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "Emergency Alert: Fire Detected";
    message.addRecipient("User", RECIPIENT_EMAIL);
    message.text.content = "Fire has been detected";
    message.text.charSet = "utf-8";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    if (!smtp.connect(&session)) {
        Serial.println("SMTP Connection Failed!");
        return;
    }

    if (MailClient.sendMail(&smtp, &message)) {
        Serial.println("Email Sent Successfully!");
    } else {
        Serial.println("Error Sending Email: " + smtp.errorReason());
    }
}

void smtpCallback(SMTP_Status status) {
    Serial.println(status.info());
}
