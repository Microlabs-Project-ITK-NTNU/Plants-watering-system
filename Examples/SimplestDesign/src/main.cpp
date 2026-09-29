// Includes the Arduino library, so you can use their functions
#include <Arduino.h>  
#include "esp_http_client.h"
#include <stdio.h>
#include "src/important_defines.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <string.h>



static EventGroupHandle_t s_wifi_event_group;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        printf("Wi-Fi disconnected, retrying...\n");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        printf("Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    printf("Connecting to Wi-Fi...\n");
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
}

esp_err_t send_moisture_reading(int raw_value, float percent)
{
    char post_data[128];
    int len = snprintf(post_data, sizeof(post_data),
        "moisture,device=esp32c6 raw=%d,percent=%.1f",
        raw_value, percent);

    esp_http_client_config_t config = {
        .url = "http://192.168.0.108:8086/api/v2/write?org=YOUR_ORG&bucket=YOUR_BUCKET&precision=s",
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
