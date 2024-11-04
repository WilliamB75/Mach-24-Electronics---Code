#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h> //the ardunio IDE cant find this but this should work for the real thing? (I hope)
#include <linux/i2c-dev.h>

#define I2C_DEVICE "/dev/i2c-1"
#define ACCELEROMETER_ADDRESS 0x10

void initAccelerometer(int file) {
    // Initialize accelerometer settings if needed
    // You may need to refer to the accelerometer datasheet for configuration details
    // For example, you might set data rate, range, etc.
}

void readAccelerometerData(int file, int16_t* accelX, int16_t* accely, int16_t* accelZ) {
    // Request accelerometer data from the device
    write(file, "\x00", 1); // Send the register address to read
    read(file, accelX, 2);  // Read 2 bytes for X-axis
    read(file, accelY, 2);  // Read 2 bytes for Y-axis
    read(file, accelZ, 2);  // Read 2 bytes for Z-axis
}

int main() {
  int file;                                        

  // Open the I2C bus
  if ((file = open(I2C_DEVICE, O_RDWR)) < 0) {
    perror("Failed to open the I2C bus");
    return 1;
  }

  // Set the I2C device address
  if (ioctl(file, I2C_SLAVE, ACCELEROMETER_ADDRESS) < 0) {
    perror("Failed to set I2C address");
    return 1;
  }

  // Initialize accelerometer settings if needed
  initAccelerometer(file);

  // Loop to continuously read accelerometer data
  while (1) {
    int16_t accelX, accely, accelZ;

    // Read accelerometer data
    readAccelerometerData(file, &accelX, &accely, &accelZ);

    // Print the data
    printf("Accelerometer Data: X=%d, Y=%d, Z=%d\n", accelX, accely, accelZ);
    
    //Introduce a delay (adjust as needed)
    usleep(500000); // 500ms 
  }

  // Close the I2C bus
  close(file);

  return 0;
}