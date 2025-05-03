#include <SPI.h>                  // needed for SD Card
#include <SD.h>                   // Library for SD card
#include <Wire.h>                 // library for I2C
#include "SparkFun_MMA8452Q.h"    // Accelerometer Library
#include <Adafruit_BMP085.h>      // Library for BMP180 preassure and temperature sensor
#include <Adafruit_HDC302x.h>     // Temp & Humidity Sensor Libary

int PoleDelay = 1000;               // delay between sensor readings (mili sec) min =1
const char* filename = ".csv";


const int chipSelect = 4;

MMA8452Q accel; 
Adafruit_BMP085 bmp;
Adafruit_HDC302x hdc = Adafruit_HDC302x();

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Basic Reading Code!");
  Wire.begin();

  pinMode(LED_BUILTIN, OUTPUT);

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

  // Humidity and Temp Sensor
  if (! hdc.begin(0x44, &Wire)) {
  Serial.println("Could not find sensor?");
  while (1);
  }

  // Create a new file
  File dataFile = SD.open(filename, FILE_WRITE);

  if (dataFile) {
    dataFile.println("AccX,AccY,AccZ,TempBMP180,Presure,TempHDC302x,Humidity");
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

  digitalWrite(LED_BUILTIN, LOW);

  float accX = accel.getCalculatedX();
  float accY = accel.getCalculatedY();
  float accZ = accel.getCalculatedZ();

  float tempA = bmp.readTemperature();
  float pres = bmp.readPressure();

  double tempB = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(tempB, RH, TRIGGERMODE_LP0);

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
    dataFile.print(tempA);
    dataFile.print(",");
    dataFile.print(pres);
    dataFile.print(",");
    dataFile.print(tempB);
    dataFile.print(",");
    dataFile.print(RH);
    dataFile.println(",");
    dataFile.flush();

    dataFile.close();
    Serial.println("Data logged to SD card.");
  
  digitalWrite(LED_BUILTIN, HIGH);

  // Testing Print Outs
  Serial.println("Acceleration");
  Serial.print("AccX: ");
  Serial.println(accX, 3);
  Serial.print("AccY: ");
  Serial.println(accY, 3);
  Serial.print("AccZ: ");
  Serial.println(accZ, 3);
  Serial.println("Preassure & Temp");
  Serial.print("Preassure: ");
  Serial.println(pres);
  Serial.print("Temperature A: ");
  Serial.println(tempA);
  Serial.println("Humidity and Temp");
  Serial.print("Temperature B: ");
  Serial.println(tempB);
  Serial.print("Humidity: ");
  Serial.println(RH);


  Serial.println("");

  } else {
    Serial.println("Error opening data file for appending.");
  }

}
