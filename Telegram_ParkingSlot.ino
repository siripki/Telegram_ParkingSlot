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
#include <FS.h>
#include <SPI.h>
#include <MFRC522.h>
#include <AsyncTelegram2.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

BearSSL::WiFiClientSecure client;
BearSSL::Session   session;
BearSSL::X509List  certificate(telegram_cert);

AsyncTelegram2 teleBot(client);
TBMessage msg;

#define ssKanan 0 //D3
#define rstKanan 16 //D0
#define ssKiri 2 //D4
#define rstKiri 16 //D0
MFRC522 rfKiri(ssKiri, rstKiri);
MFRC522 rfKanan(ssKanan, rstKanan);
//SCK D5
//MOSI D6
//MISO D7

const char* APssid  =  "Parking Slot";     // SSID WiFi network
const char* APpass  =  "12345678";     // Password  WiFi network
const char* token =  "6325196881:AAE_7uXj3LFiCrpAvtPcTQvsXZehM8YX9pg";  // Telegram token

struct cardData {
  String id;
  bool status;
};

cardData cards[] = {
  {"1470439246", false}, //card1
  {"115010115014", false}, //card2
  {"5113199246", false}, //card3
  {"21101138014", false}, //card4
  {"9956181245", false}, //card5
  {"01186728", false}, //card6
  {"015613028", false}, //card7
};

int cardTotal = sizeof(cards) / sizeof(cards[0]);
int counter;
int tersedia;
int kapasitas = 5;

void setup() {
  Serial.begin(115200);
  lcdSetup();
  spiffSetup();

  WiFiManager WM;
  WM.setConnectTimeout(10);
  bool res;
  res = WM.autoConnect(APssid, APpass);
  if (res) {
    debugE("WiFi Terhubung");
    lcd.setCursor(0, 0);
    lcd.print("      WiFi      ");
    lcd.setCursor(0, 1);
    lcd.print("   Terhubung    ");
  }
  else {
    debugE("Gagal Terhubung ke WiFi !");
    lcd.setCursor(0, 0);
    lcd.print("      WiFi      ");
    lcd.setCursor(0, 1);
    lcd.print("Gagal Terhubung ");
    delay(2000);
    lcd.setCursor(0, 0);
    lcd.print("AP  :"+String(APssid));
    lcd.setCursor(0, 1);
    lcd.print("Pass:"+String(APpass));
  }

  debugE("Starting TelegramBot...");
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
  debugE("\nTest Telegram connection... ");
  if(teleBot.begin()){
    debugE("OK");
    lcd.setCursor(0, 0);
    lcd.print("  BOT Telegram  ");
    lcd.setCursor(0, 1);
    lcd.print("   Terhubung    ");
    delay(2000);
  }
  else {
    debugE("Not OK");
    lcd.setCursor(0, 0);
    lcd.print("  BOT Telegram  ");
    lcd.setCursor(0, 1);
    lcd.print("Gagal Terhubung ");
    delay(2000);
  }
  readData();
  cardCount(0);
  // debugE("cardTotal "+String(cardTotal));
  // debugE("tersedia "+String(tersedia));
}

void loop() { 
  // TBMessage msg;
  incomingMsgHandling(msg);
  cardDetect();
}

void incomingMsgHandling(TBMessage &msg) {
  if (teleBot.getNewMessage(msg)) {
    String msgText = msg.text;

    if (msgText.equals("/status")) {
      String reply;
      reply = "Kapasitas Parkir "+String(kapasitas)+"\n";
      reply += cardCount(0);
      teleBot.sendMessage(msg, reply);
    }

    else {
      String reply;
      reply = "Selamat Datang " ;
      reply += msg.sender.username;
      reply += "\nDaftar perintah :";
      reply += "\n/status";
      teleBot.sendMessage(msg, reply);                    // and send it
    }
  }
}
