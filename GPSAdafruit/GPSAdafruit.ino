#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);
const bool debugMode = false;

void setup()
{
  if (debugMode) Serial.begin(9600);
  Serial1.begin(115200);
  delay(5000);
  if (debugMode) Serial.println("Adafruit GPS library basic test!");

  // initialize digital pin for the LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  if (debugMode) Serial.println(PMTK_Q_RELEASE);
}

uint32_t timer = millis();
void loop()
{
  if (Serial1.available())  //test if GPS data is available
  {
    // Turn on LED
    digitalWrite(LED_BUILTIN,HIGH);
    char c = GPS.read();
    if ((c))
      if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA()))
          return;
      }

    if (millis() - timer > 2000) {
      timer = millis();

      if (GPS.fix) {
        if (debugMode) Serial.print(GPS.latitudeDegrees, 4);
        if (debugMode) Serial.print(", ");
        if (debugMode) Serial.println(GPS.longitudeDegrees, 4);

      }
    }
  }
  else {
    if (debugMode) Serial.println("Serial1 is unavailable"); 
    // Turn off LED
    digitalWrite(LED_BUILTIN,LOW);
    delay(2000);
  }
}
