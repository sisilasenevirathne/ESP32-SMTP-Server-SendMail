#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// Wi-Fi and Email credentials
#define WIFI_SSID "SJAY-5G"
#define WIFI_PASSWORD "12345678"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "sltmnitiot@gmail.com"
#define AUTHOR_PASSWORD "eeii qxas rmko fykd"
#define RECIPIENT_EMAIL "sjaycreation1@gmail.com"

// SMTPSession object for SMTP transport
SMTPSession smtp;
Session_Config config;
SMTP_Message message;

void connectWiFi();
void configureMailServer();
void createMailBody();
bool sendMail();
void smtpCallback(SMTP_Status status);

void setup() {
    Serial.begin(115200);
    connectWiFi();
    configureMailServer();
    createMailBody();
    sendMail();
}

void loop() {}

void connectWiFi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println("\nConnected to Wi-Fi");
}

void configureMailServer() {
    MailClient.networkReconnect(true);
    smtp.debug(1);
    smtp.callback(smtpCallback);
    
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;
    config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    config.time.gmt_offset = 5.5;
    config.time.day_light_offset = 0;
}

void createMailBody() {
    message.sender.name = F("ESP32");
    message.sender.email = AUTHOR_EMAIL;
    message.subject = F("ESP32 Test Email");
    message.addRecipient(F("Sara"), RECIPIENT_EMAIL);
    message.text.content = "Hello World! - Sent from ESP32 board";
    message.text.charSet = "us-ascii";
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
}

bool sendMail() {
    if (!smtp.connect(&config)) {
        Serial.println("SMTP Connection Failed!");
        return false;
    }
    if (!smtp.isLoggedIn()) {
        Serial.println("Not Logged In!");
        return false;
    }
    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Failed to Send Email!");
        return false;
    }
    Serial.println("Email Sent Successfully!");
    return true;
}

void smtpCallback(SMTP_Status status) {
    Serial.println(status.info());
    if (status.success()) {
        Serial.println("Email Sent Successfully!");
    }
}
