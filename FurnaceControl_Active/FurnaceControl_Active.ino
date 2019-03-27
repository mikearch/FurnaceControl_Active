
//--------------Libraries--------------------------------------------
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>                   //Time functions
#include <LiquidCrystal_I2C.h>         //For LCD display
#include <DS1307RTC.h>                 //For real time clock
#include "DHT.h"                       //For DHT22 Temp sensor
#include <OneWire.h>                   //For waterproof temp sensor
#include <DallasTemperature.h>         //For waterproof temp sensor
//--------------end of Libraries-------------------------------------

#define DHTPIN 2                       // digital pin for temperature data DHT22
#define DHTTYPE DHT22                  // DHT 22  (AM2302), AM2321
#define ONE_WIRE_BUS 45                // defines which pin the onewire bus operates on

//--------------Initiate Instances-----------------------------------
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,20,4);      // set the LCD address to 0x27 for a 20 chars and 4 line display
OneWire oneWire(ONE_WIRE_BUS);         // Initiates one wire bus for all devices
DallasTemperature sensors(&oneWire);   // Passes bus as reference to Dallas Temp
//--------------end of Instantiation---------------------------------


//--------------Temp Variables------------------------------------------------------------------
  float temp_in_celsius = 0;
  float dht_temp_sensor_adj = 1.2;
  float wp_temp_sensor_adj = .6;
  float T=0;                             //value of Temp Cent returned by temp function
//--------------end of Temp Variables-----------------------------------------------------------


//--------------Pin # Variables------------------------------------------------------------------
  const int furnacePin =6;               // pin controlling relay for furnace
  const int furnaceModePin = 49;          //pin to receive Home or Away signal from Internet Switch
  const int testTempPin = 47;            // pin to receive test mode signal
  const int pumpPin =5;                  // pin controlling relay for irrigation pump
//--------------end of Pin Variables--------------------------------------------------------------


//--------------Heating Variables-----------------------------------------------------------------
  int HeatingMode = 1;                    //Mode of Heating system 0 = Away, 1= Home
  unsigned long CurtimeFurnace;           //time of furnace at execution of furnace control function
  unsigned long FurnaceOnTime;            //time that the furnace was turned on
  unsigned long FurnaceRunningTime;        //length of time in seconds that the furnance has been on
  int furnaceStatus = LOW;                //sets status variable to LOW (furnance On)
  float HeatOnTemp = 6;                  //minimum temperature allowed when system is set to AWAY
  unsigned long HeatingOffDelay = 2700;    //furnace runs for 45 mins (2700 seconds) past satisfaction of minimum temp
  //unsigned long HeatingOffDelay = 60;    //furnace runs for 1 min (60 seconds) past satisfaction of minimum temp
  int HoldFurnace = 0;                    //allows mainbody to test if Furnace is running within the HeatingDelay time
  unsigned long FurnaceTimeRemaining = 0;  //the amount of seconds remaining from current time to furnace off for Heating Delay
//-------------end of Heating Variables-----------------------------------------------------------
//-------------Pump Variables------------------------------------------------------------------------
unsigned long Basetime;                //variable for time of current bootup
unsigned long CurtimePump;             //time of pump at snapshot
unsigned long PumpOnTime;              //Target time for turning on pump
unsigned long ActPumpOnTime;           //Actual time when the pump was turned on
unsigned long PumpRunningTime;         //Length in seconds that pumpPin has been LOW
unsigned long Timediff;                //seconds remaining until PumpOnTime
unsigned long PumpInterval = 86400;    //One day cycle for pump time
//unsigned long PumpInterval = 200;    //for testing purposes only should be commented out
unsigned long StartOfDayTime;          //midnight(start of day) of current day
unsigned long PumpOnHour = 28800;      //Seconds from midnight to 8:00AM
//unsigned long PumpOnHour = 66720;    //Seconds from midnight to test time.should be commented out
int pumpStatus = HIGH;
//------------- end of Pump Variables------------------------------------------------------------------------
float temp(void);



//******************************************************************************************************
//**********************Setup********************************************************************
//******************************************************************************************************

void setup() {

//-------------General Setup-----------------------------------------------------------------------
  Serial.begin(9600);
  delay(1500);                        // Allow the hardware to sort itself out
//-------------end of General Setup----------------------------------------------------------------


//-------------One Wire Dallas Temp Setup----------------------------------------------------------
 pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
 // Start up the library
 sensors.begin();
//------------end ofOne Wire Dallas Temp Setup----------------------------------------------------------


//----------LCD Setup-------------------------------------------------------------------------------
  lcd.init();                         // initialize the lcd
  lcd.init();                         //2nd init per example sketches
  lcd.backlight();


//---------DHT Setup-------------------------------------------------------------------------------
  lcd.setCursor(0,2);
  lcd.print("T:");
  lcd.setCursor(11,2);
  lcd.print("H:");
  dht.begin();


//---------RTC Setup--------------------------------------------------------------
 setSyncProvider(RTC.get);                  // the function to get the time from the RTC

//---------end of RTC Setup--------------------------------------------------------------

//---------Furnace Control Setup----------------------------------------------------
  pinMode (testTempPin, INPUT);              //initiates pin for testing home/away cycle
  pinMode(furnacePin, OUTPUT);               //initiates pin for furnace relay
  pinMode(furnaceModePin, INPUT);            //sets furnace mode pin to INPUT
  //digitalWrite(furnacePin, HIGH);            //turns furnace on HOME by default at startup--note relay wired Normally Closed
  lcd.setCursor(0,1);
  lcd.print("M.Tmp.: ");
  lcd.setCursor(0,3);                        //Heating Mode Label
  lcd.print("H Mode:");
  lcd.setCursor(11,3);
  lcd.print("HS:");

//---------end of Furnace Control Setup--------------------------------------------------------------

//----------Pump Control Setup---------------------------------------------------------
  StartOfDayTime = daystart();              //Gets the UNIX time for midnight of current day
  Basetime = RTC.get();                     //gets current time in seconds when system has started
  PumpOnTime = StartOfDayTime + PumpOnHour; //sets initial pump operation for current time plus interval
    if (Basetime > PumpOnTime) {              //checks to see if the day's pump has been missed
      PumpOnTime = PumpOnTime + PumpInterval; //if missed set it for the next pump on time
    }
  pinMode(pumpPin, OUTPUT);                 //initiates pin for pump relay
  //pinMode(pumpOff, INPUT);                //error in compiling
  digitalWrite(pumpPin,HIGH);               //sets pin to release relay
  //digitalWrite (pumpOff, HIGH);             //sets up pin for pump on-off button error in compliling

}

//******************************************************************************************************
//**********************end of Setup********************************************************************
//******************************************************************************************************




void loop() {


  lcd.setCursor(8,1);                      //displays minimum house temp target.
  lcd.print(HeatOnTemp);

//-----------------GETS Dallas Temp WP Sensor data------------------------------------
 Serial.print(" Requesting temperatures...");
 sensors.requestTemperatures(); // Send the command to get temperature readings
 Serial.println("DONE");


//-----------------GETS AND DIPLAYS CURRENT TIME AND DATE------------------------------


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
//------------------End of Time and Date Routine----------------------------

//------------------Test Implimentation Routine----------------------------
if (digitalRead (testTempPin) == HIGH) {     // If pin is high, wp sensor is used for sample (put in icewater)
 T =  (sensors.getTempCByIndex(0)-wp_temp_sensor_adj);
 Serial.print("WP Sensor Temperature is: ");
 Serial.print (T);
 delay (2000);
}

if (digitalRead (testTempPin) == LOW) {      // If pin is high, DHT sensor is used for sample (in air)
 T = temp();                                  //calls temperature reading and display function
}
//------------------end of Test Implimentation Routine----------------------------

//---Pump Control-------------------------------------------------------------------------
PumpControl();                        //calls function to control pump
//---Furnace Control----------------------------------------------------------------------

   if (digitalRead(furnaceModePin) == LOW) {      //Checks if power from remote switch is OFF (LOW) (HOME)

      digitalWrite(furnacePin, HIGH);    //Turns Furnace continuously ON by keeping relay low
      lcd.setCursor(8,3);
      lcd.print("H");
   }

   if (digitalRead(furnaceModePin) == HIGH){         //Controls furnace to maintain minimum temperature
      lcd.setCursor(8,3);
      lcd.print("A");

      if (T < HeatOnTemp){                          //If less than target T, pass control to function that turns furn
          FurnaceControl();
      }

      if (HoldFurnace == HIGH){                     // If furnace is in HOLD mode (minimum 2hr run time) pass control to function
          FurnaceControl();
      }

      if (HoldFurnace == LOW){                      //  If furncace is not in HOLD mode and T >= to target, turn furnace off
          if (T >= HeatOnTemp){
          digitalWrite(furnacePin, LOW);
          }
      }


   }


   lcd.setCursor(15,3);
   lcd.print(HoldFurnace);
}

//------------END OF VOID LOOP--------------
