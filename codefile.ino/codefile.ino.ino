// include the SD library:

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"

 

// set up variables using the SD utility library functions:

Sd2Card card;

SdVolume volume;

SdFile root;

int count = 0;
char c;
String id;
DS1307 rtc;
LiquidCrystal lcd(2, 3, A3, A2, A1, A0);
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


void setup() {

  // Open serial communications and wait for port to open:

  Serial.begin(9600);

  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }
  lcd.begin(16, 2);
  
  if (! rtc.begin()) 
  {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) 
  {
    lcd.print("RTC is NOT running!");
  }
  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time

  Serial.println("Please scan your RFID TAG");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0)
  {
    id = Serial.readString();  // read the incoming data as string
    if (id == "AB123456789A" || id== "AB423156769C" || id=="BC123456789D" || id=="CD123456789A") {
      Serial.print("Tag id: "); Serial.println(id);
      Serial.println("Valid Tag");
      lcd.setCursor(0, 0);
      lcd.println("Attendance done");
      delay(1500);
      // lcd.setCursor(0, 1);
      // lcd.print("Recorded!");

    }
    else{
      Serial.print("Tag id: "); Serial.println(id);
      Serial.println("Invalid Tag");
      lcd.setCursor(0, 0);
      lcd.println("Id not found!");
    }
  }
  id="";
  delay(500);
  DateTime now = rtc.now();
    
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print("   ");

    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[now.dayOfWeek()]);
    lcd.print(" ,");
    lcd.print(now.day());
    lcd.print('/');
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.year());
}
