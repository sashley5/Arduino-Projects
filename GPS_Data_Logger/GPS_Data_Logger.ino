#include <SPI.h>
#include <SD.h>
#include <Arduino_MKRGPS.h>

const int chipSelect = SDCARD_SS_PIN;
const int SwitchInputPin = 2;
const bool debugMode = true;

// Variable to hold the write counter
int writeCounter;
// Variable to hold number of loops between writes
const int loopsBetweenWrites = 10;
// Variable to hold counter of number of loops
int numLoops;
//A variable to hold the value of the button - High (TRUE) or Low (FALSE)
bool buttonValue;
// Define the GPS variables
float latitude;
float longitude;
float altitude;
float speed;
int  satellites;
unsigned long epochTime;
bool GPSdataAvailable;

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
  writeCounter = 0;
  //Initalize the loop counter so it will write on the first time through
  numLoops = loopsBetweenWrites - 1;
   
   // see if the card is present and can be initialized:

  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1);
  }
  if (debugMode) Serial.println("card initialized.");

   // Initalize the GPS shield
   if (!GPS.begin(GPS_MODE_SHIELD)) {
     if (debugMode) Serial.println("Failed to initialize GPS!");
     while (1);

  }
   if (debugMode) Serial.println("GPS library initialized.");
}


void loop() {
  // Increment the loop counter
   numLoops++;
  
  // make a string for assembling the data to log:
   String dataString = "";

  // First read the state of the button
  // buttonValue = digitalRead (SwitchInputPin);
  buttonValue = true;
  // Now decide what to do
  if (buttonValue)
  {
    if (numLoops >= loopsBetweenWrites)
    {
      // Increment number of writes
      writeCounter ++;
      // Reset loop counter
      numLoops = 0;
      // Flash LED each time through
      digitalWrite(LED_BUILTIN,HIGH);
      // check if there is new GPS data available
      if (GPS.available()) {
         GPSdataAvailable=true;
         // read GPS values
          latitude   = GPS.latitude();
          longitude  = GPS.longitude();
          altitude   = GPS.altitude();
          speed      = GPS.speed();
          satellites = GPS.satellites();
          epochTime = GPS.getTime();
          if (debugMode) {
            // print GPS values
            Serial.print("Location: ");
            Serial.print(latitude, 7);
            Serial.print(", ");
            Serial.println(longitude, 7);
            Serial.print("Altitude: ");
            Serial.print(altitude);
            Serial.println("m");
            Serial.print("Ground speed: ");
            Serial.print(speed);
            Serial.println(" km/h");
            Serial.print("Number of satellites: ");
            Serial.println(satellites);
            Serial.println();
            Serial.print("Epoch time: ");
            Serial.println(epochTime);
          }
      }
     // open the file
     File dataFile = SD.open("datalog.txt", FILE_WRITE);
     // if the file is available, write to it:
     if (dataFile) {
        // Build up output string
       dataString = "loop ";
       dataString += String(writeCounter);
       // if GPS data is available then write that data to the SD card
        if (GPSdataAvailable) {
          dataString += " Location: ";
            dataString += String(latitude, 7);
            dataString += ", ";
            dataString += String(longitude, 7);
            dataString += "Altitude: ";
            dataString += String(altitude);
            dataString += "m, ";
            dataString += "Ground speed: ";
            dataString += String(speed);
            dataString += " km/h";
            dataString += "Number of satellites: ";
            dataString += String(satellites);
            dataString += "Epoch time: ";
            dataString += String(epochTime);
        } //end if GPS available
       dataFile.println(dataString);
       dataFile.close();
        // print to the serial port too:
         if (debugMode) Serial.println(dataString);
    } // end if dataFile
     // if the file isn't open, pop up an error:
     else
      {
       if (debugMode) Serial.println("error opening datalog.txt");
     }
    } // end if numLoops >= loopsBetweenWrites
  } // end if buttonValue
  else
  {
    digitalWrite(LED_BUILTIN,LOW);
  }
  delay(1000);
  // if GPS data is available leave he LED on
  if (!GPSdataAvailable) digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
} // end loop
