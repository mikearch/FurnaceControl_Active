/*
 NOTES:
 This function requires a test in the main body for:
  a, temperature actual vs. temperature target
  b. Heating system mode

 
 This function is called only if:
  a. Heating System is AWAY ,and
  b. Temp Actual is Less than or Equal to LOW Temp, and

 The function then 
  a. records the start time of the function
  b. If the system is off, it records the ON time and turns the system ON
  c. It calculates the running time of the current ON cycle
  d. It determines wheter Hold time has been exceeded.
  It turns the system off 
 */
  
 void FurnaceControl() {
  CurtimeFurnace =RTC.get();                             //gets current time for start of this function
  //furnaceStatus = digitalRead(furnacePin);             //reads whether furnace is on or not
 
 
  if (digitalRead(furnacePin) == LOW){                   //check if furnace is off Pin is low, meaning activated relay, wired normally closed)
    FurnaceOnTime = CurtimeFurnace;                      //Record the time that the furnace starts
    digitalWrite(furnacePin, HIGH);                      //turns furnace ON by setting relay input HIGH (relay off, wired normally clsoed)
    HoldFurnace = HIGH;                                  //sets the first to HOLD mode after initial startup
  }
  FurnaceRunningTime = CurtimeFurnace  - FurnaceOnTime;  //calculates the current running time of the furnace
 
  if (FurnaceRunningTime > HeatingOffDelay) {
    furnaceStatus = LOW;
    digitalWrite(furnacePin, LOW);                       //Energizes relay which turns off furnace
    HoldFurnace = LOW;                                     //Sets furnace hold status to Off
  }

return;
}


