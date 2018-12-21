//time functions
//-----------------GETS AND DIPLAYS CURRENT TIME AND DATE------------------------------

void Time_Date(){
  tmElements_t tm;                        //Reads RTC

  if (RTC.read(tm)) {
      lcd.setCursor(15,0);
      if ((tm.Hour)<10)
         {
              lcd.print("0");
         }
      lcd.print(tm.Hour);
      lcd.print(":");

      if ((tm.Minute)<10)
         {
              lcd.print("0");
         }
      lcd.print(tm.Minute);
      lcd.setCursor(0,0);
      lcd.print(tm.Day);
      lcd.print("/");
      lcd.print(tm.Month);
      lcd.print("/");
      lcd.print(tmYearToCalendar(tm.Year));
    }

    else {
      lcd.setCursor(0,0);
      if (RTC.chipPresent())
        {
              lcd.print("Run the SetTime");
        }
      else
        {
              lcd.print("Chk Circuits");
        }
      }
    }
//------------------End of Time and Date Routine----------------------------
