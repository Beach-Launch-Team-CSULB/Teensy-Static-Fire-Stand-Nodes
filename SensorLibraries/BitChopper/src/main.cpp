#include <Arduino.h>
#include "BitChopper.h"
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  BitChopper bitChopper;
  uint32_t data = 3723476863;//110 1110 11110 111110 1111110 1111111 TESTING
  
  config testConfig;
  testConfig.resolution =16;
  testConfig.offset = 0;

  Serial.println(data, BIN);
  data = bitChopper.compress(testConfig, data );  
  data = bitChopper.extract(testConfig, data);
  Serial.println(data, BIN);

  delay(3000);
}