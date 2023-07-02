#include <SPI.h>
#include <SD.h>

const int chipSelect = SDCARD_SS_PIN;
const int SwitchInputPin = 2;
const boolean debugMode = false;

// Variable to hold the write counter
int writeCounter;
// Variable to hold number of loops between writes
const int loopsBetweenWrites = 30;
// Variable to hold counter of number of loops
int numLoops;

void setup() {
  if (debugMode) {
     // Open serial communications and wait for port to open:
     Serial.begin(9600);
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
        }
  }
   if (debugMode) Serial.print("Initializing SD card...");
  
  // initialize digital pin for the LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // and similarly the switch as an input
  pinMode(SwitchInputPin, INPUT);
  // Initialise the write counter
  writeCounter = 1000;
  //Initalize the loop counter so it will write on the first time through
  numLoops = loopsBetweenWrites - 1;
   
   // see if the card is present and can be initialized:

  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1);
  }
  if (debugMode) Serial.println("card initialized.");
}
//A variable to hold the value of the button - High (TRUE) or Low (FALSE)
boolean buttonValue;

void loop() {
  // Increment the loop counter
   numLoops++;
  // make a string for assembling the data to log:
    String dataString = "";
  // First read the state of the button
  buttonValue = digitalRead (SwitchInputPin);
  // Now decide what to do
  if (buttonValue)
  {
    if (numLoops >= loopsBetweenWrites)
    {
      // Increment number of writes
      writeCounter ++;
      // Reset loop counter
      numLoops = 0;
     // open the file. note that only one file can be open at a time,
     // so you have to close this one before opening another.
     File dataFile = SD.open("datalog.txt", FILE_WRITE);
     // if the file is available, write to it:
     if (dataFile) {
       dataString = "loop ";
       dataString += String(writeCounter);
       dataFile.println(dataString);
       dataFile.close();
        // print to the serial port too:
         if (debugMode) Serial.println(dataString);
    }
     // if the file isn't open, pop up an error:
     else
      {
       if (debugMode) Serial.println("error opening datalog.txt");
     }
  }
 
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
  }
  else
  {
    digitalWrite(LED_BUILTIN,LOW);
  }
}
