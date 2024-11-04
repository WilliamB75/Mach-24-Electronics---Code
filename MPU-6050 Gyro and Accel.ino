#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_DEVICE "/dev/i2c-1"
#define MPU6050_ADDR 0x68

// Function to initialize I2C communication
int initI2C() {
    int file = open(I2C_DEVICE, O_RDWR);
    if (file < 0) {
        perror("Failed to open the I2C bus");
        return -1;
    }

    if (ioctl(file, I2C_SLAVE, MPU6050_ADDR) < 0) {
        perror("Failed to set I2C address");
        return -1;
    }

    return file;
}

// Function to read a 16-bit register value from MPU-6050
int16_t readMPU6050Register(int file, uint8_t reg) {
    write(file, &reg, 1);
    uint8_t data[2];
    read(file, data, 2);
    return (int16_t)((data[0] << 8) | data[1]);
}

int main() {
  int file;
  initI2C();
  if (file < 0) {
    return 1;
  }

  // Initialize MPU-6050
  // Power management register, wake up the sensor
  write(file, "\x6B\x00", 2);

  while (1) {
    // Read accelerometer data
    int16_t accelx = readMPU6050Register(file, 0x3B);
    int16_t accely = readMPU6050Register(file, 0x3D);
    int16_t accelz = readMPU6050Register(file, 0x3F);

    // Read gyroscope data
    int16_t gyrox = readMPU6050Register(file, 0x43);
    int16_t gyroy = readMPU6050Register(file, 0x45);
    int16_t gyroz = readMPU6050Register(file, 0x47);

    // Print data
    printf("Accelerometer: X=%d, Y=%d, Z=%d\n", accelx, accely, accelz);
    printf("Gyroscope: X=%d, Y=%d, Z=%d\n", gyrox, gyroy, gyroz);

    // Introduce a delay (adjust as needed)
    usleep(500000); // 500ms
  }

  close(file);

  return 0;
}
