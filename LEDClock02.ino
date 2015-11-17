/* Sketch for LED clock mirror
 * Created by Andy Waplinger, 2015
 * http://andywaplinger.com
 * https://github.com/andywaplinger/ledclock05
 */

#include <FastLED.h>
#define HOUR_LEDS 60
#define MIN_LEDS 60
#define HOUR_PIN 4
#define MIN_PIN 5
#define CLOCK_PIN 6

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}


CRGB minArray[MIN_LEDS];
CRGB hourArray[HOUR_LEDS];

int pirPin = 3;


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, MIN_PIN, GRB>(minArray, MIN_LEDS);
  FastLED.addLeds<WS2812B, HOUR_PIN, GRB>(hourArray, HOUR_LEDS);

  pinMode(pirPin, INPUT);

  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setTime(17,6,9,1,15,11,15);
}

void loop() {
  displayTime();
  if (digitalRead(pirPin) == HIGH){
    displayHour();
    displayMin();
  }
  else {
    displayOff();
  }
}

void setTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

void displayHour()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  switch(hour%12){
    case 0:
      hourArray[0] = CRGB::Red;
      hourArray[1] = CRGB::Red;
      break;
    case 1:
      hourArray[2] = CRGB::Red;
      hourArray[3] = CRGB::Red;
      break;
    case 2:
      hourArray[4] = CRGB::Red;
      hourArray[5] = CRGB::Red;
      break;
    case 3:
      hourArray[6] = CRGB::Red;
      hourArray[7] = CRGB::Red;
      break;
    case 4:
      hourArray[8] = CRGB::Red;
      hourArray[9] = CRGB::Red;
      break;
    case 5:
      hourArray[10] = CRGB::Red;
      hourArray[11] = CRGB::Red;
      break;
    case 6:
      hourArray[12] = CRGB::Red;
      hourArray[13] = CRGB::Red;
      break;
    case 7:
      hourArray[14] = CRGB::Red;
      hourArray[15] = CRGB::Red;
      break;
    case 8:
      hourArray[16] = CRGB::Red;
      hourArray[17] = CRGB::Red;
      break;
    case 9:
      hourArray[18] = CRGB::Red;
      hourArray[19] = CRGB::Red;
      break;
    case 10:
      hourArray[20] = CRGB::Red;
      hourArray[21] = CRGB::Red;
      break;
    case 11:
      hourArray[22] = CRGB::Red;
      hourArray[23] = CRGB::Red;
      break;
  }
  FastLED.show();
}

void displayMin()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  for(int minLED = 0; minLED < minute; minLED++) { 
    if ((minLED + 1)%15 == 0){
      minArray[minLED] = CRGB::Orange;
    }
    else {
      minArray[minLED] = CRGB::Blue;
    }
    FastLED.show();
    delay(20);
    // clear this led for the next time around the loop
    //leds[dot] = CRGB::Black;
  }
}

void displayOff()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  
  // retrieve data from DS3231
  readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  for (int offLED = 59; offLED >= 0; offLED--){
    minArray[offLED] = CRGB::Black;
    hourArray[offLED] = CRGB::Black;
    delay(20);
  }
  FastLED.show();
}
