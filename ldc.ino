void lcdSetup(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    Telegram    ");
  lcd.setCursor(0, 1);
  lcd.print("  Parking Slot  ");
}
void lcdPrint(String msg1, String msg2, String msg3) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Terisi   : "+msg1);
  lcd.print("Tersedia : "+msg2);
  lcd.setCursor(0, 1);
  lcd.print(msg3);
}