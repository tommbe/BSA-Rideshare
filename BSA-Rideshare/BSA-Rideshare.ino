#include <SPI.h>                  // needed for SD Card
#include <SD.h>                   // Library for SD card
#include <Wire.h>                 // library for I2C
#include "SparkFun_MMA8452Q.h"    // Accelerometer Library
#include <Adafruit_BMP085.h>      // Library for BMP180 preassure and temperature sensor

int PoleDelay = 1000;               // delay between sensor readings (mili sec) min =1
const char* filename = "Test.csv";


const int chipSelect = 4;

MMA8452Q accel; 
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Basic Reading Code!");
  Wire.begin();

// Initialise SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);                      // Stops code if 
  }

  // Accelerotmeter Setup
  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }

  // Preassure and Temp Setup
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  // Create a new file
  File dataFile = SD.open(filename, FILE_WRITE);

  if (dataFile) {
    dataFile.println("AccX,AccY,AccZ,Temp,P");
    dataFile.close();
    Serial.print("Created file: ");
    Serial.println(filename);
  } 
  else {
    Serial.println("Error opening data file for writing.");
  }
}

void loop() {
  delay(PoleDelay);

  float accX = accel.getCalculatedX();
  float accY = accel.getCalculatedY();
  float accZ = accel.getCalculatedZ();

  float temp = bmp.readTemperature();
  float pres = bmp.readPressure();

   // Open the file for appending
  File dataFile = SD.open(filename, FILE_WRITE);

  // Write the sensor data to the file
  if (dataFile) {
    dataFile.print(accX, 3);
    dataFile.print(",");
    dataFile.print(accY, 3);
    dataFile.print(",");
    dataFile.print(accZ, 3);
    dataFile.print(",");
    dataFile.print(temp);
    dataFile.print(",");
    dataFile.print(pres);
    dataFile.println(",");
    dataFile.flush();

    dataFile.close();
    Serial.println("Data logged to SD card.");
  } else {
    Serial.println("Error opening data file for appending.");
  }

}
