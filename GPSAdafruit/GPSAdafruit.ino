#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);
const bool debugMode = true;

void setup()
{
  // initialize digital pin for the LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  if (debugMode) Serial.begin(9600);
  Serial1.begin(115200);
  delay(5000);
  if (debugMode) Serial.println("Adafruit GPS library basic test!");

  // Initialise the GPS
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);

  if (debugMode) {
    Serial.print("PMTK_Q_RELEASE: ");
    Serial.println(PMTK_Q_RELEASE);
  } 
}

uint32_t timer = millis();

void loop()
{
  if (Serial1.available())  //test if GPS data is available
  {
    // Read some GPS data
    char c = GPS.read();
    if (debugMode) {
      // Serial.print ("c is ");
      // Serial.print(c); 
    }  
    if ((c))
      if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA()))
          return;
      }

    // Output data every 2 seconds
    if (millis() - timer > 2000) {
      timer = millis();
    if (debugMode) Serial.println("Serial1 is available"); 
    if (debugMode) {
      // Serial.print("\nTime: ");
      // Serial.print(GPS.hour, DEC); Serial.print(':');
      // Serial.print(GPS.minute, DEC); Serial.print(':');
      // Serial.print(GPS.seconds, DEC); Serial.print('.');
      // Serial.println(GPS.milliseconds);
      // Serial.print("Date: ");
      // Serial.print(GPS.day, DEC); Serial.print('/');
      // Serial.print(GPS.month, DEC); Serial.print("/20");
      // Serial.println(GPS.year, DEC);
      // Serial.print("Fix: "); Serial.print((int)GPS.fix);
      // Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    }
    
      // Test GPS processing
      // if (debugMode) Serial.println("GPS processing");
      if (GPS.fix) {
        // Turn on LED
        digitalWrite(LED_BUILTIN,HIGH);
        if (debugMode) Serial.print(GPS.latitudeDegrees, 4);
        if (debugMode) Serial.print(", ");
        if (debugMode) Serial.println(GPS.longitudeDegrees, 4);

      } // end if GPS.fix
    } // end if timer is 2 seconds
  } // end if serial available
  else {
    if (debugMode) Serial.println("Serial1 is unavailable"); 
    // Turn off LED
    digitalWrite(LED_BUILTIN,LOW);
    delay(2000);
  }
}
