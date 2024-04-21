#include <bluefruit.h>

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEUart bleuart;  // BLE UART service

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning Setup");
  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check the datasheet for the value to use
  Bluefruit.setName("BME280_ESP32");

  // Configure and Start Device Information Service
  bledis.setManufacturer("Nordic Semiconductor");
  bledis.setModel("nRF52");
  bledis.begin();

  // Start the BLE UART service
  bleuart.begin();

  // Set up and start advertising
  startAdvertising();
}

void startAdvertising() {
  Serial.println("Starting advertising...");
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)
     
     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html   
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop() {
  // Send a single message when connected
  if (Bluefruit.connected()) {
    bleuart.print("Hello, this is a single message!");
    delay(1000);  // Wait for a moment
    //uint16_t conn_handle = Bluefruit.connHandle(); // Get the connection handle
    //Bluefruit.disconnect(conn_handle);
  }
}
