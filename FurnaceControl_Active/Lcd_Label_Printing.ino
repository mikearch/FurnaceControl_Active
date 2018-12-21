//LCD Label printing
void LCD_Label_Print(){
  lcd.setCursor(0,2);
  lcd.print("T:");

  lcd.setCursor(11,2);
  lcd.print("H:");

  lcd.setCursor(0,1);
  lcd.print("M.Tmp.: ");

  lcd.setCursor(0,3);                        //Heating Mode Label
  lcd.print("H Mode:");

  lcd.setCursor(11,3);
  lcd.print("HS:");
}
