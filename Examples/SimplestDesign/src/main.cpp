#include <Arduino.h>

// put function declarations here:
const int analogPin = 36;
const int relayPin = 32;

const int threshold = 1000;

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
  
  if (val < threshold) {
    // pump vann
    digitalWrite(relayPin, HIGH); // Writes to the relay pin
  } else if (val >= threshold) {
    digitalWrite(relayPin, LOW);
  }

  delay(200);
}
