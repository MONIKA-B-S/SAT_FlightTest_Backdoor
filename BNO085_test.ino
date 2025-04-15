/*
 * @Author : Monika , SJ
 * @Date : 15 April 2025
 * @Description : Live Accelerometer Data Logging from BNO08x with ESP32.
                  > Using BNO085 reads real-time acceleration data (X, Y, Z axes) from the BNO085 sensor using I²C communication with an ESP32. 
                  > Outputs it in CSV format for easy logging.
                  > Displays it as live graphs in Arduino Serial Plotter.
 * @Pin Connection :
                 ESP32     <--->      BNO085
                 3.3v      ---->      VIN
                 GND       ---->      GND
                 GPIO22    ---->      SCL
                 GPIO21    ---->      SDA
 */

/* Code */

#include <Adafruit_BNO08x.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BNO08x bno08x;
sh2_SensorValue_t sensorValue;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bno08x.begin_I2C()) {
    while (1) {
      Serial.println("BNO08x not detected");
      delay(1000);
    }
  }

  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Failed to enable accelerometer");
  }

  // No headers here, to make Serial Plotter show a graph
  // If logging to a .csv manually, you can uncomment the line below
  // Serial.println("Accel_X,Accel_Y,Accel_Z");
}

void loop() {
  delay(100); // ~10Hz sample rate

  if (bno08x.getSensorEvent(&sensorValue)) {
    if (sensorValue.sensorId == SH2_ACCELEROMETER) {
      Serial.print(sensorValue.un.accelerometer.x, 4);
      Serial.print(",");
      Serial.print(sensorValue.un.accelerometer.y, 4);
      Serial.print(",");
      Serial.println(sensorValue.un.accelerometer.z, 4);
    }
  }
}
