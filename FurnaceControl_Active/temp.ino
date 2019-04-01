float temp(){

/*---------------GETS AND CALCULATES CELSIUS TEMPERATURE
 Wait a few seconds between measurements.
  Reading temperature or humidity takes about 250 milliseconds!
   Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  */
  delay(2000);


  float h = dht.readHumidity();
  float t = dht.readTemperature()-dht_temp_sensor_adj;             // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true)-(dht_temp_sensor_adj*9/5);    // Read temperature as Fahrenheit (isFahrenheit = true)

  if (isnan(h) || isnan(t) || isnan(f)) {           // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return -99.0;
  }


  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" *C ");


 //----------------END OF TEMPERATURE CALCULATION

 //----------------DISPLAYS TEMPERATURE
  lcd.setCursor(3,2);
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(14,2);
  lcd.print(h);
  lcd.print("%");
//  lcd.print(analogRead(0));
//  lcd.print(analogRead(1));
//-----------------END OF TEMPURATURE DISPLAY
return t;
}

void getTemp(){

  Serial.println("Chip = AM2320");
  switch(th.Read()) {
    case 2:
      Serial.println("  CRC failed");
      break;
    case 1:
      Serial.println("  Sensor offline");
      break;
    case 0:
      Serial.print("  Humidity = ");
      Serial.print(th.Humidity);
      hum = (th.Humidity);
      Serial.println("%");
      Serial.print("  Temperature = ");
      Serial.print(th.cTemp);
      temp = (th.cTemp);
      Serial.println("*C");
      Serial.println();
      break;
  }
  //delay(2000);
  return;
}
