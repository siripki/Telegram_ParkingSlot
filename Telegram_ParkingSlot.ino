/*
Note :
- Based on ESP8266 V2.6.3
*/
#define debugMode 1 //debugMode 1 enable common print serial, 0 only print error
#if debugMode == 1
#define debugP(x) Serial.println(x) //printSerial
#define debugE(y) Serial.println(y) //printError
#else
#define debugP(x)
#define debugE(y) Serial.println(y)
#endif

#define USE_CLIENTSSL true 
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

#include <time.h>
#include "SPIFFS.h"
#include "FS.h"
#include <SPI.h>
#include <MFRC522.h>
#include <AsyncTelegram2.h>
#include <Adafruit_ADS1X15.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
BearSSL::WiFiClientSecure client;
BearSSL::Session   session;
BearSSL::X509List  certificate(telegram_cert);

AsyncTelegram2 teleBot(client);
Adafruit_ADS1115 ads;

#define ssKanan 32
#define rstKanan 17
#define ssKiri 33
#define rstKiri 16
MFRC522 rfKiri(ssKiri, rstKiri);
MFRC522 rfKanan(ssKanan, rstKanan);

const char* APssid  =  "Parking Slot";     // SSID WiFi network
const char* APpass  =  "12345678";     // Password  WiFi network
const char* token =  "6302352184:AAFl_8lq3vgNN2BemJRIObhqC0Zu3Aav79w";  // Telegram token

struct cardData {
  String id;
  bool status;
};

cardData cards[] = {
  {"16511018173", 0}, //card1
  {"2023817644", 0}, //card2
  {"6801522644", 0}, //card3
  {"18024522744", 0}, //card4
  {"0424922944", 0}, //card5
  {"18012223744", 0}, //card6
  {"5211422844", 0}, //card7
  {"048323444", 0}, //card8
  {"1969323344", 0}, //card9
  {"2125120744", 0}, //card10
  {"2129023344", 0} //card11
};

int cardTotal = sizeof(cards) / sizeof(cards[0]);
int counter;
int tersedia;

void setup() {
  Serial.begin(115200);
  spiffSetup();
  readData();

  WiFiManager WM;
  WM.setConnectTimeout(10);
  if (WM.autoConnect(APssid, APpass)) {
    Serial.println("WiFi Terhubung");
  }
  else {
    Serial.println("Gagal Terhubung ke WiFi !");
  }

  Serial.println("Starting TelegramBot...");
  // Sync time with NTP, to check properly Telegram certificate
  configTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  //Set certficate, session and some other base client properies
  client.setSession(&session);
  client.setTrustAnchors(&certificate);
  client.setBufferSizes(1024, 1024);
  // Set the Telegram bot properies
  teleBot.setUpdateTime(2000);
  teleBot.setTelegramToken(token);
  // Check if all things are ok
  Serial.print("\nTest Telegram connection... ");
  teleBot.begin() ? Serial.println("OK") : Serial.println("NOK");
}

void loop() { 
  TBMessage msg;
  incomingMsgHandling(msg);
  cardDetect();
}

void incomingMsgHandling(TBMessage &msg) {
  if (teleBot.getNewMessage(msg)) {
    String msgText = msg.text;

    if (msgText.equals("/status")) {
      String slamp1Stat = "OFF";
      String slamp2Stat = "OFF";
      if (lamp1Stat == 0) slamp1Stat = "ON";
      if (lamp2Stat == 0) slamp2Stat = "ON";
      
      String reply;
      reply = "Informasi" ;
      reply += slamp1Stat;
      teleBot.sendMessage(msg, reply);    
    }

    else {
      String reply;
      reply = "Selamat Datang " ;
      reply += msg.sender.username;
      reply += "\nDaftar perintah :";
      reply += "\n/lampu1_on";
      reply += "\n/lampu2_on";
      reply += "\n/lampu1_off";
      reply += "\n/lampu2_off";
      reply += "\n/info";
      teleBot.sendMessage(msg, reply);                    // and send it
    }
  }
}
