// for printing arrays
//Float array variable transmision
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
