void lcdlabels() {
//----Current Temp----
  lcd.setCursor(0,2);
  lcd.print("T: ");
//----Minimum On Temp for Away
  lcd.setCursor(0,1);
  lcd.print("MTemp: ");
  //----Current Humidity---
  lcd.setCursor(11,2);
  lcd.print("H: ");
  //----Current Heating Mode----
  lcd.setCursor(0,3);
  lcd.print("HMode: ");
  //----Next Pump Run-----
  lcd.setCursor(11,3);
  lcd.print("PmpOn: ");

  return;
}

void lcddata(){
//----minimum house temp target
  lcd.setCursor(7,1);
  lcd.print(HeatOnTemp);
//----prints hold status of AWAY mode
  lcd.setCursor(15,3);
  lcd.print(HoldFurnace);
  return;
}
