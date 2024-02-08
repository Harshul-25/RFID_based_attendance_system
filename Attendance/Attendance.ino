#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"

Sd2Card card;

SdVolume volume;

SdFile root;

int count = 0;
char c;
String id;
DS1307 rtc;
File dataLog;
boolean sd_ok = 0;  
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

LiquidCrystal lcd(2, 3, A3, A2, A1, A0);

void setup() {

  // Open serial communications and wait for port to open:

  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // put your setup code here, to run once:
  if ( !SD.begin() )
    Serial.println("initialization failed!");  // initialization error
 
  else {   // initialization OK
    sd_ok = 1;
    Serial.println("initialization done.");
    if( SD.exists("Log.txt") == 0 )   // test if file with name 'Log.txt' already exists
    {  // create a text file named 'Log.txt'
      Serial.print("\r\nCreate 'Log.txt' file ... ");
      dataLog = SD.open("Log.txt", FILE_WRITE);
      if(dataLog) {                               
        Serial.println("OK");
        
        dataLog.close();   // close the file
      }
      else
        Serial.println("error creating file.");
    }
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
  DateTime now = rtc.now();
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
      // write data to SD card
    if(sd_ok)
    {  // if the SD card was successfully initialized
      // open Log.txt file with write permission
      dataLog = SD.open("Log.txt", FILE_WRITE);
      dataLog.print( id ); dataLog.print("  ");
    dataLog.print(now.year());
    dataLog.print('/');
    dataLog.print(now.month());
    dataLog.print('/');
    dataLog.print(now.day());
    dataLog.print(',');
    dataLog.print(now.hour());
    dataLog.print(':');
    dataLog.println(now.minute());
      dataLog.close();   // close the file
    }

    }
    else{
      Serial.print("Tag id: "); Serial.println(id);
      Serial.println("Invalid Tag");
      lcd.setCursor(0, 0);
      lcd.println("Id not found!");
      lcd.println("Id not found!");

    }
  }
  id="";
  delay(500);
    
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
