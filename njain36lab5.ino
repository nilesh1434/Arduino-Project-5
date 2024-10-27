// Nilesh Jain - 653929943
// Lab 5 - Communication
// 1. This code is supposed to allow the user to type in a specific time and date into
// the Serial Monitor command prompt. This will be a special Epoch Timestamp.
// Epoch Timestamp wrbsite - (https://www.epochconverter.com/)
// Remember: Always put T before every timestamp serial number (eg: T1357041600)
// After this the LCD will display and run the time and date from the requested timestamp.
// Youtube link - https://youtu.be/w9x_ZqxAzqI

// LCD library
#include <LiquidCrystal.h>
// Time and Date libraary
#include <TimeLib.h>

// Add 'T' before every Epoch Timestamp
#define TIME_HEADER "T"
#define TIME_REQUEST 7

// This defines the LCD wiring to the DIGITALpins
// LCD points
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;

// library
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  while (!Serial);
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);
  // Beofre input
  Serial.println("Waiting to sync");
  lcd.print("Loading...");
}

void loop() {
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus() != timeNotSet) {
    digitalClockDisplay();
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
  delay(1000);
  lcd.setCursor(0, 0);
}

void digitalClockDisplay() {
  // Time (hh/mm/ss)
  lcd.print("Time: ");
  lcd.print(hour());
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  lcd.setCursor(0, 1);
  
  // Date (mm/dd/yyyy)
  lcd.print("Date: ");
  lcd.print(month());
  lcd.print("/");
  lcd.print(day());
  lcd.print("/");
  lcd.print(year());

  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  lcd.print(":");
  // if its single digit, add 0 in front of that number
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

// Sync message
void processSyncMessage() {
  unsigned long userTime;
  const unsigned long DEFAULT_TIME = 1357041600;

  if (Serial.find(TIME_HEADER)) {
    userTime = Serial.parseInt();
    if (userTime >= DEFAULT_TIME) {
      setTime(userTime);
    }
  }
}

time_t requestSync() {
Serial.write(TIME_REQUEST);
return 0;
}
