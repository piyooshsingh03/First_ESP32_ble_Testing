#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int value=0;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("📲 Client Connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("🔌 Client Disconnected");
  }
};

void setup() {
  Serial.begin(115200);

  // Create BLE Device
  BLEDevice::init("ESP32_Temp_Sensor");

  // Create BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service and advertising
  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("🚀 BLE Temperature Sensor is now advertising...");
}

void loop() {
  if (deviceConnected) {
    // float tempC = 25.0 + (random(-50, 50) / 10.0); // simulate temp 20–30°C
    // char buffer[16];
    // std:: string message="hello";
    // snprintf(buffer, sizeof(buffer), "%.1f °C", tempC);

    pCharacteristic->setValue(value);
    pCharacteristic->notify();  // send notification
    Serial.print("📤 Sent: ");
    Serial.println(value);
    value++;
    delay(500); // send every 2 seconds
  } else {
    delay(500);
  }
}
