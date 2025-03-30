#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi Credentials
#define WIFI_SSID "Your own SSID"
#define WIFI_PASSWORD "Your own Password"

// SMTP Credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465 // SSL port
#define AUTHOR_EMAIL "SENDER MAIL ID"
#define AUTHOR_PASSWORD "SENDER MAIL APP PASSWORD"
#define RECIPIENT_EMAIL "RECEIVER MAIL ID"

// SMTP Session
SMTPSession smtp;

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    sendEmail();
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
    message.sender.name = "ESP8266 Alert";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP8266 Alert: Motion Detected";
    message.addRecipient("User", RECIPIENT_EMAIL);
    message.text.content = "This is an automated alert from your ESP8266 system.";
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

void loop() {
    // Add event triggers here (e.g., motion detection)
}
