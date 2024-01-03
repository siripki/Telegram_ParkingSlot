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
  lcd.print("Isi: "+msg1);
  lcd.setCursor(8, 0);
  lcd.print("Sisa: "+msg2);
  lcd.setCursor(0, 1);
  lcd.print(msg3);
  delay(1000);
}