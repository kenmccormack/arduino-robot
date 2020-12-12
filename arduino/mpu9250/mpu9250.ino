
/*
Basic_I2C.ino
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MPU9250.h"
#include <SoftwareSerial.h>
#include "SimpleTimer.h"

SoftwareSerial sserial_in(10, 11);

unsigned char last_rx = 0;
SimpleTimer timer;


// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
// Pin 2 is Sclk
// Pin 3 is SDA
MPU9250 IMU(Wire,0x68);
int status;

void setup() {
  // serial to display data
  sserial_in.begin(19200);
  while(!sserial_in) {}
  
  Serial.begin(115200);
  while(!Serial) {}


  // want to listen only to the input
  sserial_in.listen();
  

  
  
 
 

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  status = IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  if (status < 0 ) {
    Serial.println("Can't set accel range");
  }
  status = IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  if (status < 0 ) {
    Serial.println("Can't set gyro range");
  }
  status = IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_10HZ);
  

  timer.setInterval(50, readIMU);
}

void loop()
{
  timer.run();
}

void readIMU() {
  // read the sensor
  IMU.readSensor();
  // display the data
  Serial.print(IMU.getAccelX_mss(),6);
  Serial.print(",");
  Serial.print(IMU.getAccelY_mss(),6);
  Serial.print(",");
  Serial.print(IMU.getAccelZ_mss(),6);
  Serial.print(",");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print(",");
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print(",");
  Serial.print(IMU.getGyroZ_rads(),6);
  Serial.print(",");
  Serial.print(IMU.getMagX_uT(),6);
  Serial.print(",");
  Serial.print(IMU.getMagY_uT(),6);
  Serial.print(",");
  Serial.print(IMU.getMagZ_uT(),6);
  Serial.print(",");
  Serial.print(IMU.getTemperature_C(),6);
  Serial.print(",");
 
  bool header_found = false; 
  while (sserial_in.available())
  {
       unsigned char cur_rx = sserial_in.read();
       if (last_rx == 0xD3 && cur_rx == 0x00 )
       {
         header_found = true;
       }
       last_rx = cur_rx; 
  }
  if (header_found) {
     Serial.println("1.0");
  } else
  {
    Serial.println("0.0");
  }
}
