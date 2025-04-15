/* @Author : Monika , SJ
 * @Date : 15 April 2025
 * @Description :The Adafruit BNO08x 9-DOF IMU sensor with an ESP32 microcontroller to determine the real-time yaw (heading) angle and its corresponding compass direction (like North, South-East, West, etc.).
                 Compass Direction Detection:
                 Based on the yaw angle (0° to 360°), the code maps it to a compass direction like:
                  *N (North)
                  *NE (North-East)
                  *E (East)
                  *SE (South-East)
                  *S (South)
                  *SW (South-West)
                  *W (West)
                  *NW (North-West)
 * @Pin Connections :
                  ESP32  <---->  BNO085
                  3.3v    ---->  VIN
                  GND     ---->  GND
                  GPIO22  ---->  SCL
                  GPIO21  ---->  SDA
*/
/* Code */
#include <Adafruit_BNO08x.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BNO08x bno08x;
sh2_SensorValue_t sensorValue;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bno08x.begin_I2C()) {
    Serial.println("BNO08x not detected!");
    while (1);
  }

  Serial.println("BNO08x connected!");

  if (!bno08x.enableReport(SH2_ROTATION_VECTOR)) {
    Serial.println("Could not enable rotation vector!");
    while (1);
  }

  Serial.println("Yaw Output Initialized!");
}

void loop() {
  delay(100);

  if (!bno08x.getSensorEvent(&sensorValue)) return;

  if (sensorValue.sensorId == SH2_ROTATION_VECTOR) {
    float real = sensorValue.un.rotationVector.real;
    float i = sensorValue.un.rotationVector.i;
    float j = sensorValue.un.rotationVector.j;
    float k = sensorValue.un.rotationVector.k;

    // Convert quaternion to Euler angles
    float yaw = atan2(2.0 * (real * k + i * j),
                      1.0 - 2.0 * (j * j + k * k)) * 180.0 / PI;

    if (yaw < 0) yaw += 360.0;

    String direction = getDirectionFromYaw(yaw);

    Serial.print("Yaw: ");
    Serial.print(yaw, 2);
    Serial.print("° | Direction: ");
    Serial.println(direction);
  }
}

String getDirectionFromYaw(float yaw) {
  if (yaw >= 337.5 || yaw < 22.5) return "N";
  if (yaw >= 22.5 && yaw < 67.5) return "NW";
  if (yaw >= 67.5 && yaw < 112.5) return "W";
  if (yaw >= 112.5 && yaw < 157.5) return "SE";
  if (yaw >= 157.5 && yaw < 202.5) return "S";
  if (yaw >= 202.5 && yaw < 247.5) return "SE";
  if (yaw >= 247.5 && yaw < 292.5) return "E";
  if (yaw >= 292.5 && yaw < 337.5) return "NE";
  return "Unknown";
}
