
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
  const int furnaceModePin = 49;           //pin to receive Home or Away signal from Internet Switch
  const int testTempPin = 47;            // pin to receive test mode signal
//--------------end of Pin Variables--------------------------------------------------------------


//--------------Heating Variables-----------------------------------------------------------------
  int HeatingMode = 1;                    //Mode of Heating system 0 = Away, 1= Home
  unsigned long CurtimeFurnace;           //time of furnace at execution of furnace control function
  unsigned long FurnaceOnTime;            //time that the furnace was turned on
  unsigned long FurnaceRunningTime;        //length of time in seconds that the furnance has been on
  int furnaceStatus = LOW;                //sets status variable to LOW (furnance On)
  float HeatOnTemp = 10;                  //minimum temperature allowed when system is set to AWAY
  unsigned long HeatingOffDelay = 7200;    //furnace runs for 2hrs (7200 seconds) past satisfaction of minimum temp
  //unsigned long HeatingOffDelay = 60;    //furnace runs for 2hrs (7200 seconds) past satisfaction of minimum temp
  int HoldFurnace = 0;                    //allows mainbody to test if Furnace is running within the HeatingDelay time
  unsigned long FurnaceTimeRemaining = 0;  //the amount of seconds remaining from current time to furnace off for Heating Delay
//-------------end of Heating Variables-----------------------------------------------------------

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
 sensors.begin();   // Start up the library
//------------end ofOne Wire Dallas Temp Setup----------------------------------------------------------

//----------LCD Setup-------------------------------------------------------------------------------
  lcd.init();                         // initialize the lcd
  lcd.init();                         //2nd init per example sketches
  lcd.backlight();

//---------DHT Setup-------------------------------------------------------------------------------
  dht.begin();

//---------RTC Setup--------------------------------------------------------------
 setSyncProvider(RTC.get);                  // the function to get the time from the RTC
//---------end of RTC Setup--------------------------------------------------------------

//---------Furnace Control Setup----------------------------------------------------
  pinMode (testTempPin, INPUT);              //initiates pin for testing home/away cycle
  pinMode(furnacePin, OUTPUT);               //initiates pin for furnace relay
  pinMode(furnaceModePin, INPUT);            //sets furnace mode pin to INPUT

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


Time_Date(); //Gets and Displays current time and date

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

//---Furnace Control----------------------------------------------------------------------

   if (digitalRead(furnaceModePin) == LOW) {      //Checks if power from remote switch is OFF (LOW) (HOME)

      digitalWrite(furnacePin, HIGH);             //Turns Furnace continuously ON by keeping relay low
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
