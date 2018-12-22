// for printing arrays
//Float array variable transmision
/*
Array Index Keys
arrayBoolean[]
  0 status_Home = whether current status is Home or Away
arrayFloat[]
  0 temp_Bar = temperature in Bar
  1 temp_HtOn = set temperature degC for Away Heat On
arrayLongUs[]
  0 time_HeatCycle = set time in seconds for HeatOn during Away
  1 time_CycleRem = time running in seconds in curr HeatCycle
*/
void printFloat(){
  for (n=0; n <= arraySize_float - 1; n++ ) {
    Serial1.print (begOfLine);
    Serial1.print (arrayFloatVars);
    Serial1.print(delim);
    Serial1.print(n);
    Serial1.print(delim);
    Serial1.print(arrayFloat[n]);
    Serial1.print(endOfLine);
    Serial1.flush();
    delay(1000);
  }
}

//Boolean array variable transmision
void printBoolean(){
  for (n=0; n <= arraySize_boolean - 1; n++ ) {
    Serial1.print (begOfLine);
    Serial1.print (arrayBoolVars);
    Serial1.print(delim);
    Serial1.print(n);
    Serial1.print(delim);
    Serial1.print(arrayBoolean[n]);
    Serial1.print(endOfLine);
    Serial1.flush();
    delay(1000);
  }
}

//Unsigned Long array variable transmision
void printLongUsInt(){
  for (n=0; n <= arraySize_longUsInt - 1; n++ ) {
    Serial1.print (begOfLine);
    Serial1.print (arrayLongUSVars);
    Serial1.print(delim);
    Serial1.print(n);
    Serial1.print(delim);
    Serial1.print(arrayLongUs[n]);
    Serial1.print(endOfLine);
    Serial1.flush();
    delay(1000);
  }
}

//This function needs to have one line for each position in the above three arrays
void collectData(){
  //Boolean array
  arrayBoolean[0] = furnaceModePin;
  //Float array
  arrayFloat[0] = T;
  arrayFloat[1] = HeatOnTemp;
  //Long UnsInt array
  arrayLongUs[0] = HeatingOffDelay;
  arrayLongUs[1] = FurnaceRunningTime;


}
