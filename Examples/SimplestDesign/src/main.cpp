// Includes the Arduino library, so you can use their functions
#include <Arduino.h>  
#include "esp_http_client.h"
#include <stdio.h>

esp_err_t send_moisture_reading(int raw_value, float percent)
{
    char post_data[128];
    int len = snprintf(post_data, sizeof(post_data),
        "moisture,device=esp32c6 raw=%d,percent=%.1f",
        raw_value, percent);

    esp_http_client_config_t config = {
        .url = "http://100.112.39.3:8086/api/v2/write?org=YOUR_ORG&bucket=YOUR_BUCKET&precision=s",
        .method = HTTP_METHOD_POST,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Authorization", "Token YOUR_TOKEN");
    esp_http_client_set_header(client, "Content-Type", "text/plain; charset=utf-8");
    esp_http_client_set_post_field(client, post_data, len);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        printf("InfluxDB write status: %d\n", status);
    } else {
        printf("HTTP POST failed: %s\n", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return err;
}

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
