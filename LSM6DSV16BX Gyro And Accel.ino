#include <Wire.h>

#define LSM6DS3_ADDR 0x68 // Default I2C address for LSM6DS3

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize LSM6DS3
  initLSM6DS3();
}

void loop() {
  // Read accelerometer and gyroscope data
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;

  readAccelData(&accelX, &accelY, &accelZ);
  readGyroData(&gyroX, &gyroY, &gyroZ);

  // Print data to serial monitor
  Serial.print("Accel: ");
  Serial.print(accelX);
  Serial.print(",");
  Serial.print(accelY);
  Serial.print(",");
  Serial.print(accelZ);
 
  Serial.print(" | Gyro: ");
  Serial.print(gyroX);
  Serial.print(",");
  Serial.print(gyroY);
  Serial.print(",");
  Serial.println(gyroZ);

  delay(1000); // Adjust the delay based on your application requirements
}

void initLSM6DS3() {
// Enable accelerometer and gyroscope
writeRegister (LSM6DS3_ADDR, 0x10, 0b00111000);

// Set accelerometer and gyroscope scale (2g and 250 dps, respectively)
writeRegister (LSM6DS3_ADDR, 0x11, 0b10000000);

// Set data rate to 104 Hz
writeRegister(LSM6DS3_ADDR, 0x10, 0b00111000);
}

void readAccelData(int16_t* accelX, int16_t* accelY, int16_t* accelZ) {
  Wire.beginTransmission (LSM6DS3_ADDR);
  Wire.write(0x28);
  Wire.endTransmission(false);

  Wire.requestFrom(LSM6DS3_ADDR, 6, true);

  *accelX = Wire.read() | (Wire.read() << 8);
  *accelY = Wire.read() | (Wire.read() << 8);
  *accelZ = Wire.read() | (Wire.read() << 8);
}

void readGyroData(int16_t* gyroX, int16_t* gyroY, int16_t* gyroZ) {
  Wire.beginTransmission(LSM6DS3_ADDR);
  Wire.write(0x22);
  Wire.endTransmission(false);

  Wire.requestFrom(LSM6DS3_ADDR, 6, true);

  *gyroX = Wire.read() | (Wire.read() << 8);
  *gyroY = Wire.read() | (Wire.read() << 8);
  *gyroZ = Wire.read() | (Wire.read() << 8);
}

void writeRegister(uint8_t address, uint8_t subAddress, uint8_t data) {
  Wire.beginTransmission(address);
  Wire.write(subAddress);
  Wire.write(data);
  Wire.endTransmission(true);
}

