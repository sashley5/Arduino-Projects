const int SwitchInputPin = 2;

void setup() {
  // initialize digital pin for the LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // and similarly the switch as an input
  pinMode(SwitchInputPin, INPUT);
}
//A variable to hold the value of the button - High (TRUE) or Low (FALSE)
boolean buttonValue;

void loop() {
  // First read the state of the button
  buttonValue = digitalRead (SwitchInputPin);
  // Now decide what to do
  if (buttonValue)
  {
    // Turn on the LED if the button is pushed down
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1000);
  }
  else
  {
    // Turn off the LED if the button is not pushed down
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
  }
}

