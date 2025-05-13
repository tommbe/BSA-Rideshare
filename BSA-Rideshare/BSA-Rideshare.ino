#include <SPI.h>       // needed for SD Card
#include <SD.h>        // Library for SD card
#include <Wire.h>        // library for I2C
#include "SparkFun_MMA8452Q.h"       // Accelerometer Library
#include <Adafruit_BMP085.h>         // Library for BMP180 pressure and temperature sensor
#include <Adafruit_HDC302x.h>     // Temp & Humidity Sensor Library

// Constants
const int POLE_DELAY = 5000;       // delay between sensor readings (milliseconds) min = 1
const char* FILENAME = "TEST.csv";
const int CHIP_SELECT = 4;
const int LED_PIN = LED_BUILTIN; // Use the built-in LED pin

// Global Objects
MMA8452Q accel;
Adafruit_BMP085 bmp;
Adafruit_HDC302x hdc = Adafruit_HDC302x();
File dataFile; //make the file global

// Function Declarations (Best Practice)
void setupSDCard();
void setupAccelerometer();
void setupBMP180();
void setupHDC302x();
void createDataFile();
void readAndLogData();
void printSensorData(float accX, float accY, float accZ, float tempA, float pres, double tempB, double RH); // Used for debuging and testing
void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(LED_PIN, OUTPUT);

  setupSDCard();
  setupAccelerometer();
  setupBMP180();
  setupHDC302x();
  createDataFile();
}

void loop() {
  delay(POLE_DELAY);
  readAndLogData();
}

void setupSDCard() {
  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("SD Card failed, or not present");
    while (1);
  }
}

void setupAccelerometer() {
  if (accel.begin() == false) {
    Serial.println("Could not find MMA8452Q (accelerometer) sensor");
    while (1);
  }
}

void setupBMP180() {
  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 (Preassure Sensor) sensor");
    while (1) {}
  }
}

void setupHDC302x() {
  if (!hdc.begin(0x44, &Wire)) {
    Serial.println("Could not find HDC302x (Temp & Humidity) sensor");
    while (1);
  }
}

void createDataFile() {
  dataFile = SD.open(FILENAME, FILE_WRITE);

  if (dataFile) {
    dataFile.println("AccX,AccY,AccZ,TempBMP180,Pressure,TempHDC302x,Humidity");
    dataFile.close();
    Serial.print("Created file: ");
    Serial.println(FILENAME);
  }
  else {
    Serial.println("Error opening data file for writing.");
  }
}


void readAndLogData() {
  digitalWrite(LED_PIN, LOW); // Removed:  Use meaningful names

  float accX = accel.getCalculatedX();
  float accY = accel.getCalculatedY();
  float accZ = accel.getCalculatedZ();

  float tempA = bmp.readTemperature();
  float pres = bmp.readPressure();

  double tempB = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(tempB, RH, TRIGGERMODE_LP0);

  dataFile = SD.open(FILENAME, FILE_WRITE);

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

    digitalWrite(LED_PIN, HIGH);
    printSensorData(accX, accY, accZ, tempA, pres, tempB, RH); //call print
  } else {
    Serial.println("Error opening data file for appending.");
  }
}

//function to print the sensor data
void printSensorData(float accX, float accY, float accZ, float tempA, float pres, double tempB, double RH) {
  Serial.println("Acceleration");
  Serial.print("AccX: ");
  Serial.println(accX, 3);
  Serial.print("AccY: ");
  Serial.println(accY, 3);
  Serial.print("AccZ: ");
  Serial.println(accZ, 3);
  Serial.println("Pressure & Temp");
  Serial.print("Pressure: ");
  Serial.println(pres);
  Serial.print("Temperature A: ");
  Serial.println(tempA);
  Serial.println("Humidity and Temp");
  Serial.print("Temperature B: ");
  Serial.println(tempB);
  Serial.print("Humidity: ");
  Serial.println(RH);
  Serial.println("");
}
