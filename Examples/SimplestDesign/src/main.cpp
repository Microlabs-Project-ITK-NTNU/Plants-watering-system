// Includes the Arduino library, so you can use their functions
#include <Arduino.h>  

// Set pin numbers for IO:
const int analogPin = 36;
const int relayPin = 32;

// Set threshold for relay activation
const int threshold = 1000;

// Initialize the value variable outside the loop, to save performance 
int val = 0;

// Setup runs automatically, when using the Arduino framework
void setup() {
  Serial.begin(9600); // Baudrate for the serial monitor
  pinMode(relayPin, OUTPUT); // Sets the digital relay pin as output
}

// Loop will run again and again, when using the Arduino framework
void loop() {
  val = analogRead(analogPin); // Reads analog value of sensor, using arduino function for ADC
  Serial.println(val); // Prints the value to the serial monitor
  
  if (val < threshold) {
    digitalWrite(relayPin, HIGH); // Writes relay pin high, starts pump
  } else if (val >= threshold) {
    digitalWrite(relayPin, LOW); // Writes relay pin low, stops pump
  }

  delay(200); // Waits .2 seconds, mostly to not overflow the serial monitor
}
