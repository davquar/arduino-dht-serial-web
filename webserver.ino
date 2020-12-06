/**
 * This Arduino code reads temperature and relative humidity
 * from a DHT11 sensor, and prints the data to an LCD display
 * and the serial port, to make the data avilable to other applications.
 * //TODO: dressing + status leds
 * Author: Davide Quaranta
 * quaranta.1715742@studenti.uniroma1.it
 */

// import required libraries
#include <stdio.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 8                          // set the data pin used for the DHT sensr
#define DHTTYPE DHT11                     // set the type of DHT sensor

// set the backlight pins
#define CONTRAST_PIN 9
#define BRIGHTNESS_PIN 10

// initialize the libraries
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // begin using the LCD
  lcd.begin(16, 2);

  // mark the backlight pins as output
  pinMode(CONTRAST_PIN, OUTPUT);
  pinMode(BRIGHTNESS_PIN, OUTPUT);

  // set the backlight in such a way that characters are visible
  digitalWrite(CONTRAST_PIN, LOW);
  digitalWrite(BRIGHTNESS_PIN, HIGH);

  // begin using the DHT 
  dht.begin();
  lcd.print("Temperat");
  lcd.setCursor(0, 1);                    // prepare to print on the second row
  lcd.print("Humidity");

  // begin using the serial port with baudrate 9600
  Serial.begin(9600);
  while (!Serial) {
    ; // wait until the connection is estabilished
  }
}

void loop() {
  delay(2000);                            // add a delay needed to avoid DHT reading errors

  // read data from the sensor
  int hum = dht.readHumidity();
  float temp = dht.readTemperature();

  lcd.setCursor(9, 0);                    // set the cursor on row 0 and column 9
  lcd.print(temp);
  lcd.print((char)223);                   // print degree (°) character
  lcd.print("C");
  lcd.setCursor(9, 1);                    // set the cursor on row 1 and column 9
  lcd.print(hum);
  lcd.print("%");

  char json[50];                          // prepare a buffer for the JSON stirng
  
  // create the JSON string.
  // The *100%100 trick is used to get the decimals, because Arduino doesn't support %f
  snprintf(json, 50, "{\"temperature\":%d.%02d,\"humidity\":%d}\n", (int)temp, (int)(temp*100)%100, hum);

  Serial.write(json);                     // write the JSON string on the serial port
  Serial.flush();                         // wait for the transmission to finish
}
