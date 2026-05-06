#include <WiFi.h>
#include <esp_now.h>

// Joystick pins
#define X_PIN 34
#define Y_PIN 35

// Structure (must match receiver)
typedef struct struct_message {
  int xValue;
  int yValue;
} struct_message;

struct_message dataToSend;

// 👉 REPLACE with your receiver MAC address
uint8_t receiverMAC[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

// Callback (optional)
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // Set device as Wi-Fi station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Add peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read joystick
  dataToSend.xValue = analogRead(X_PIN);
  dataToSend.yValue = analogRead(Y_PIN);

  // Debug
  Serial.print("X: ");
  Serial.print(dataToSend.xValue);
  Serial.print("  Y: ");
  Serial.println(dataToSend.yValue);

  // Send data
  esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

  delay(50); // small delay for stability
}
