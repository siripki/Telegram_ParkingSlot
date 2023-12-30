void oledSetup(){
  display.begin(0x3c, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(27,18);  display.println(F("Smart Parking"));
  display.setCursor(32,35);  display.println(F("By sirip_ki"));
  display.display();
  delay(1000);
}
void oledPrint(String msg1, String msg2, String msg3) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(1,5);  display.println("Terisi   : "+msg1);
  display.setCursor(1,25); display.println("Tersedia : "+msg2);
  display.setCursor(1,45); display.println(msg3);
  display.display();
}