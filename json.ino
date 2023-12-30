#include <ArduinoJson.h>
StaticJsonDocument<256> doc;

void decodeJson(String &data) {
  DeserializationError error = deserializeJson(doc, data);
  
  for (int i = 0; i < cardTotal; i++) {
    String key = "stat" + String(i + 1);
    cards[i].status = doc[key];
  }
}

void codeJson(String &data) {
  for (int i = 0; i < cardTotal; i++) {
    String key = "stat" + String(i + 1);
    doc[key] = cards[i].status;
  }
  
  serializeJson(doc, data);
}