#include <Wire.h>

#define IMU_ADDR 0x6A

// Register mein value write karne ke liye
void writeRegister(byte reg, byte value) {
  Wire1.beginTransmission(IMU_ADDR);
  Wire1.write(reg);
  Wire1.write(value);
  Wire1.endTransmission();
}

// 2-byte sensor value read karne ke liye
int16_t read16(byte reg) {
  Wire1.beginTransmission(IMU_ADDR);
  Wire1.write(reg);
  Wire1.endTransmission(false);

  Wire1.requestFrom(IMU_ADDR, (byte)2);

  byte low = Wire1.read();
  byte high = Wire1.read();

  return (int16_t)(high << 8 | low);
}

void setup() {
  Serial.begin(115200);

  // UNO Q ke Qwiic/I2C bus ko start karo
  Wire1.begin();

  // LSM6DSOX ka WHO_AM_I register read karo
  Wire1.beginTransmission(IMU_ADDR);
  Wire1.write(0x0F);
  Wire1.endTransmission(false);

  Wire1.requestFrom(IMU_ADDR, (byte)1);

  byte whoAmI = Wire1.read();

  Serial.print("WHO_AM_I: 0x");
  Serial.println(whoAmI, HEX);

  // Accelerometer ON
  writeRegister(0x10, 0x40);

  Serial.println("Wildlife Motion Tracker Started!");
}

void loop() {

  // X, Y, Z acceleration read karo
  int16_t x = read16(0x28);
  int16_t y = read16(0x2A);
  int16_t z = read16(0x2C);

  Serial.print("X: ");
  Serial.print(x);

  Serial.print("  Y: ");
  Serial.print(y);

  Serial.print("  Z: ");
  Serial.println(z);

  delay(500);
}
