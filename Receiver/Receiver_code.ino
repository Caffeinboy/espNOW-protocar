#include <WiFi.h>
#include <esp_now.h>

// Motor pins
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// PWM settings
#define PWM_FREQ 1000
#define PWM_RES 8
#define PWM_DUTY 191   // 75%

typedef struct struct_message {
  int xValue;
  int yValue;
} struct_message;

struct_message incomingData;

// ================= MOTOR CONTROL =================

void stopCar() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, 0);
}

void forward() {
  ledcWrite(IN1, PWM_DUTY);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, PWM_DUTY);
  ledcWrite(IN4, 0);
}

void backward() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, PWM_DUTY);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, PWM_DUTY);
}

void left() {
  ledcWrite(IN1, 0);
  ledcWrite(IN2, PWM_DUTY);
  ledcWrite(IN3, PWM_DUTY);
  ledcWrite(IN4, 0);
}

void right() {
  ledcWrite(IN1, PWM_DUTY);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, PWM_DUTY);
}

// ================= ESP-NOW RECEIVE =================

void OnDataRecv(const esp_now_recv_info * info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  int x = incomingData.xValue;
  int y = incomingData.yValue;

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("  Y: ");
  Serial.println(y);

  if (y > 3000) forward();
  else if (y < 1000) backward();
  else if (x > 3000) right();
  else if (x < 1000) left();
  else stopCar();
}

void setup() {
  Serial.begin(115200);

  // Attach PWM (ESP32 Core 3.x)
  ledcAttach(IN1, PWM_FREQ, PWM_RES);
  ledcAttach(IN2, PWM_FREQ, PWM_RES);
  ledcAttach(IN3, PWM_FREQ, PWM_RES);
  ledcAttach(IN4, PWM_FREQ, PWM_RES);

  stopCar();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {}
