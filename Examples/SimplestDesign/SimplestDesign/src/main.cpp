#include <Arduino.h>

// put function declarations here:
const int analogPin = 36;
const int relayPin = 23;

int val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT); // Sets the digital relay pin as output
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(analogPin);
  Serial.println(val);
  
  if (val < 2000) {
    // pump vann
    digitalWrite(relayPin, HIGH); // Writes to the relay pin
  }

  delay(200);
}
