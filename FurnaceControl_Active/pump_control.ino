/*Controls the ON and OFF of the pump based upon proximity to Target ON and OFF times which are calculated
 * at the beginning during setup.  The targeted ON and OFF times for the NEXT cycle are calculated and updated
 * by this function when the pump has completed its cycle.  Current parameters are:
 * Pump ON once per day at 8:00AM, max running time 2 minutes.  Pump is turned off outside of this program
 * via a float switch in the water tank.
 */


void PumpControl() {
 CurtimePump =RTC.get();
 pumpStatus = digitalRead(pumpPin);              //read current status of pump, HIGH is off, LOW is on
 Timediff = PumpOnTime - CurtimePump;           //seconds left until the pump is scheduled to be on
 if (pumpStatus == HIGH) {                       //evaluates circumstances if pump is OFF


  if (Timediff < 30) {                            //Checks if current time is within 30 seconds of target on time.
    digitalWrite(pumpPin,LOW);                    //Turns pump on
    ActPumpOnTime = RTC.get();                    //Records time the pump actually went on vs. the target
    PumpOnTime = PumpOnTime + PumpInterval;       //Sets the new target for the next pump on cycle
    }
  }

if (pumpStatus == LOW) {                          //Checks if pump is ON

  PumpRunningTime = CurtimePump - ActPumpOnTime;  //If pump in ON, update running time
  if (PumpRunningTime > maxPumpRun) {             //Check if pump has run for target time and turn off is yes
    digitalWrite(pumpPin, HIGH);                  //Turns pump off
    PumpRunningTime =0;                           //Clear the pump running time after turning pump off
  }
}
delay(1000);                                      //Used in original routine to avoid running function more than once per second.probably not needed now

return;
}
