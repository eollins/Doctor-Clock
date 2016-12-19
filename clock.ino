
void(* resetFunc) (void) = 0;
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_RA8875.h"
#include "Sodaq_DS3231.h"
#include <Adafruit_NeoPixel.h>
//#include <Adafruit_STMPE610.h>
#define sd_cs 47                          // uding ethernet shield sd
int prev = 0;
int alarmPrev = -1;
unsigned long subtract = 0;
unsigned long first = 0;
//unsigned long multiplier = 1;
//int lastHour = 0;
//boolean add = true;
//String lastMin = String(prev);
//boolean updateHour = false;
//boolean updateMinuteOne = false;
//boolean updateMinuteTwo = false;
boolean firstDraw = true;
boolean setting = true;
String lastPrev = "";
unsigned long start;
uint16_t tx, ty;
boolean inAlarm = false;

int numb1 = 0;
int numb2 = 0;
int numb3 = 0;

double dow = 0;
double mon = 0;
double day = 0;
double year = 0;

#define RA8875_INT 48
#define RA8875_CS 53
#define RA8875_RESET 49
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 2, NEO_GRB + NEO_KHZ800);
void setup () {
  Wire.begin();
  rtc.begin();
  Serial.begin(9600);
  randomSeed(analogRead(A3));
  analogWrite(9, 255);
  digitalWrite(9, LOW);
  analogWrite(10, 0);
  //Serial.println(analogRead(A0));
  pixels.begin();

  int red = random(0, 256);
  int green = random(0, 256);
  int blue = random(0, 256);

  for (int i = 0; i < 12; i++) {
    pixels.setPixelColor(i, red, green, blue);
    pixels.show();
    delay(25);
  }

  if (!SD.begin(sd_cs))
  {
    Serial.println("initialization failed!");

    return;
  }
  Serial.println("initialization done.");
  Serial.println("RA8875 start");
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }


  //  for (int i = 0; i < 12; i++) {
  //    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
  //    pixels.show();
  //  }

  Serial.println("Found RA8875");
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  //  Serial.print("(");
  //  Serial.print(tft.width());
  //  Serial.print(", ");
  //  Serial.print(tft.height());
  //  Serial.println(")");
  tft.graphicsMode();
  tft.touchEnable(true);
  tft.fillRect(0, 0, 200, 750, RA8875_RED);
  tft.fillRect(200, 0, 200, 750, RA8875_GREEN);
  tft.fillRect(400, 0, 200, 750, RA8875_CYAN);
  tft.fillRect(600, 0, 200, 750, RA8875_YELLOW);
  tft.fillRect(0, 510, 200, 240, RA8875_RED);
  tft.fillRect(200, 510, 200, 240, RA8875_GREEN);
  tft.fillRect(400, 510, 200, 240, RA8875_CYAN);
  tft.fillRect(600, 510, 200, 240, RA8875_YELLOW);
  //  tft.fillRect(559.3, 0, 400, 480, RA8875_MAGENTA);
  //
  //
  //  tft.fillRect(559.3, 0, 400, 750, RA8875_BLUE);
  //  tft.fillRect(559.3, 0, 300, 120, RA8875_BLACK);

  numb1 = rtc.now().hour();
  numb2 = String((String(rtc.now().minute())[0])).toInt();
  numb3 = String((String(rtc.now().minute())[1])).toInt();

  tft.textMode();
  char string[4] = "12 ";
  tft.textSetCursor(65, 75);

  if (rtc.now().hour() < 12) {
    if (rtc.now().hour() == 0) {
      string[0] = '1';
      string[1] = '2';
    }
    else {
      if (rtc.now().hour() < 10) {
        string[0] = ' ';
        string[1] = String(rtc.now().hour())[0];
        tft.textSetCursor(50, 75);
      }
      else {
        string[0] = String(rtc.now().hour())[0];
        string[1] = String(rtc.now().hour())[1];
      }
    }
  }
  else {
    string[0] = String(rtc.now().hour() % 12)[0];
    string[1] = String(rtc.now().hour() % 12)[1];
    tft.textSetCursor(80, 75);
  }
  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(5);


  tft.textWrite(string);
  char string2[3] = " 0";

  string2[0] = ' ';

  if (rtc.now().minute() < 10) {
    string2[1] = '0';
  }
  else {
    string2[1] = String(rtc.now().minute())[0];
  }

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(5);
  tft.textSetCursor(250, 75);
  tft.textWrite(string2);


  char string3[3] = " 0";

  string3[0] = ' ';

  if (rtc.now().minute() < 10) {
    string3[1] = '0';
  }
  else {
    string3[1] = String(rtc.now().minute())[1];
  }

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(5);
  tft.textSetCursor(450, 75);
  tft.textWrite(string3);



  char string4[4] = "AM ";

  if (rtc.now().hour() > 11) {
    string4[0] = 'P';
    string4[1] = 'M';
  }

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(5);
  tft.textSetCursor(665, 75);
  tft.textWrite(string4);




  //  char string5[4] = "Set";
  //
  //  tft.textTransparent(RA8875_WHITE);
  //  tft.textEnlarge(4);
  //  tft.textSetCursor(635, 10);
  //  tft.textWrite(string5);
  //
  //  char string7[6] = "Alarm";
  //
  //  tft.textTransparent(RA8875_WHITE);
  //  tft.textEnlarge(4);
  //  tft.textSetCursor(600, 150);
  //  tft.textWrite(string7);

  char string9[6] = "Alarm";

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(35, 400);
  tft.textWrite(string9);



  char string8[5] = "None";

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(250, 400);
  tft.textWrite(string8);


  char string7[4] = "Set";

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(465, 400);
  tft.textWrite(string7);

  char string6[5] = "Load";

  tft.textTransparent(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(650, 400);
  tft.textWrite(string6);

  tft.graphicsMode();

  lastPrev = toPrev(rtc.now());

  dow = rtc.now().dayOfWeek();
  mon = rtc.now().month();
  day = rtc.now().date();
  year = rtc.now().year();

  //file = SD.open("log.txt", FILE_WRITE);
}
int hour = 0;
int minuteDigitOne = 0;
int minuteDigitTwo = 0;
unsigned long start2 = 0;
unsigned long end2 = 0;
boolean first2 = true;
DateTime dt;

String toPrev(DateTime now) {
  int theHour = now.hour() * 60;
  return String(theHour + now.minute());
}

String dowToWord(double dow3) {
  if (dow3 == 0 || dow3 == 0.5) {
    return "Sunday";
  }
  else if (dow3 == 1 || dow3 == 1.5) {
    return "Monday";
  }
  else if (dow3 == 2 || dow3 == 2.5) {
    return "Tuesday";
  }
  else if (dow3 == 3 || dow == 3.5) {
    return "Wednesday";
  }
  else if (dow3 == 4 || dow == 4.5) {
    return "Thursday";
  }
  else if (dow3 == 5 || dow == 5.5) {
    return "Friday";
  }
  else if (dow3 == 6 || dow == 6.5) {
    return "Saturday";
  }
  else {
    return "";
  }
}

String monToWord(int mon3) {
  if (mon3 == 1 || mon3 == 1.5) {
    return "January";
  }
  else if (mon3 == 2 || mon3 == 2.5) {
    return "February";
  }
  else if (mon3 == 3 || mon3 == 3.5) {
    return "March";
  }
  else if (mon3 == 4 || mon3 == 4.5) {
    return "April";
  }
  else if (mon3 == 5 || mon3 == 5.5) {
    return "May";
  }
  else if (mon3 == 6 || mon3 == 6.5) {
    return "June";
  }
  else if (mon3 == 7 || mon3 == 7.5) {
    return "July";
  }
  else if (mon3 == 8 || mon3 == 8.5) {
    return "August";
  }
  else if (mon3 == 9 || mon3 == 9.5) {
    return "September";
  }
  else if (mon3 == 10 || mon3 == 10.5) {
    return "October";
  }
  else if (mon3 == 11 || mon3 == 11.5) {
    return "November";
  }
  else if (mon3 == 12 || mon3 == 12.5) {
    return "December";
  }
}

void changeText() {
  String dow2 = dowToWord(dow);
  String mon2 = monToWord(mon);
  String plusMinus = "+ -";

  tft.fillScreen(RA8875_BLACK);
  tft.fillRect(10, 10, 135, 75, RA8875_WHITE);

  char * str = new char[dow2.length() + 1];
  Serial.println(dow2);
  for (int i = 0; i < dow2.length(); i++) {
    str[i] = dow2[i];
  }
  str[dow2.length()] = '\0';

  char * str2 = new char[mon2.length() + 1];
  Serial.println(mon2);
  for (int i = 0; i < mon2.length(); i++) {
    str2[i] = mon2[i];
  }
  str2[mon2.length()] = '\0';

  char * str3 = new char[String((int)day).length()];
  Serial.println(String((int)day));
  for (int i = 0; i < String((int)day).length(); i++) {
    str3[i] = String((int)day)[i];
  }
  str3[String((int)day).length()] = '\0';

  char * str4 = new char[String((int)year).length()];
  Serial.println(String((int)year));
  for (int i = 0; i < String((int)year).length(); i++) {
    str4[i] = String((int)year)[i];
  }
  str4[String((int)year).length()] = '\0';

  char * str5 = new char[plusMinus.length()];
  for (int i = 0; i < plusMinus.length(); i++) {
    str5[i] = String(plusMinus)[i];
  }
  str5[String(plusMinus).length()] = '\0';

  char save[] = "Save";
  tft.textMode();
  tft.textEnlarge(2);
  tft.textTransparent(RA8875_BLACK);
  tft.textSetCursor(31, 22);
  tft.textWrite(save);
  tft.graphicsMode();

  tft.textMode();
  tft.textEnlarge(6);
  tft.textTransparent(RA8875_WHITE);

  int dowX = (400 - ((dow2.length() * 35)) / 2) - 5;
  int monX = (400 - ((mon2.length() * 35)) / 2) - 5;
  int dayX = (400 - (String((int)day).length() * 35) / 2) - 5;
  int yearX = (400 - (String((int)year).length() * 35) / 2) - 5;
  int pmX = (400 - (plusMinus.length() * 35) / 2) - 5;

  tft.textSetCursor(dowX, 40);
  tft.textWrite(str);
  Serial.println();
  Serial.println(dow2.length());

  tft.textSetCursor(monX, 120);
  tft.textWrite(str2);

  tft.textSetCursor(dayX, 200);
  tft.textWrite(str3);

  tft.fillRect((dayX + (String(day).length() * 35)) - 112, 200, 100, 65, RA8875_BLACK);

  tft.textTransparent(RA8875_WHITE);
  tft.textSetCursor(yearX, 280);
  tft.textWrite(str4);

  tft.fillRect((dayX + (String(year).length() * 35)) - 156, 275, 100, 65, RA8875_BLACK);

  tft.textSetCursor(pmX, 335);
  tft.textTransparent(RA8875_CYAN);
  tft.textWrite(str5);

  tft.graphicsMode();
  //delete str;
  //delete str2;
  //delete str3;
  //delete str4;
}

boolean dateSetting = false;
int mode = 0;
void loop()
{
  //Serial.println(dateSetting);
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  if (setting == true) {
    if (first2) {
      hour = numb1;
      minuteDigitOne = numb2;
      minuteDigitTwo = numb3;
      first2 = false;
    }

    //Serial.println("Setting");
    if (!digitalRead(RA8875_INT))
    {
      //      setting = false;
      //      return;
      if (tft.touched())
      {
        Serial.println("Setting");
        delay(250);
        tft.touchRead(&tx, &ty);
        //        Serial.print(tx); Serial.print(", "); Serial.println(ty);
        /* Draw a circle */
        //tft.fillCircle((uint16_t)(tx/xScale), (uint16_t)(ty/yScale), 4, RA8875_WHITE);

        if (dateSetting) {
          if ((tx / xScale) < 140 && (ty / yScale) < 80) {
            DateTime dt = rtc.now();
            DateTime newDt((int)year, (int)mon, (int)day, rtc.now().hour(), rtc.now().minute(), rtc.now().second(), (int)dow);
            rtc.setDateTime(newDt);

            tft.fillRect(10, 10, 135, 75, RA8875_BLACK);
            delay(100);
            tft.fillRect(10, 10, 135, 75, RA8875_WHITE);
            char save[] = "Save";
            tft.textMode();
            tft.textEnlarge(2);
            tft.textTransparent(RA8875_BLACK);
            tft.textSetCursor(31, 22);
            tft.textWrite(save);
            tft.graphicsMode();

            tft.fillScreen(RA8875_BLACK);
            setting = false;
            return;
          }

          if ((ty / yScale) < 135 && (tx / xScale) > 145) {
            if (dow >= 6.5) {
              dow = 0;
            }
            else {
              dow += 0.5;
            }

            delay(250);
            changeText();
          }
          else if ((ty / yScale) > 135 && (ty / yScale) < 205) {
            if (mon >= 12) {
              mon = 1;
            }
            else {
              mon++;
            }

            delay(250);
            changeText();
          }
          else if ((ty / yScale) > 205 && (ty / yScale) < 275) {
            if (fmod(mon, 2) == 0 && mon != 2 && mon != 8) {
              if (day >= 31) {
                day = 1;
              }
              else {
                day++;
              }
            }
            else if (fmod(mon, 2) != 0 && mon != 2 || mon == 8) {
              if (day >= 32) {
                day = 1;
              }
              else {
                day++;
              }
            }
            else if (mon == 2) {
              if (fmod(mon, 4) == 0) {
                if (fmod(mon, 100) == 0) {
                  if (day >= 29) {
                    day = 1;
                  }
                  else {
                    day++;
                  }
                }
                else if (fmod(mon, 400) == 0) {
                  if (day >= 30) {
                    day = 1;
                  }
                  else {
                    day++;
                  }
                }
                else {
                  if (day >= 29) {
                    day = 1;
                  }
                  else {
                    day++;
                  }
                }
              }
            }

            delay(150);
            changeText();
          }
          else if ((ty / yScale) > 275 && (tx / xScale) < 400) {
            year += 0.5;
            delay(150);
            changeText();
          }
          else if ((ty / yScale) > 275 && (tx / xScale) > 400) {
            year -= 0.5;
            delay(150);
            changeText();
          }
        }
      }

      if (!dateSetting) {
        Serial.println("datesetting is false");
        if (tx >= 0 && tx <= 250) {
          if (ty < 500) {
            if (hour >= 23) {
              hour = 0;
            }
            else {
              hour += 1;
            }
            //Serial.println("add hour: " + String(hour));
            //Serial.print(tx); Serial.print(", "); Serial.println(ty);
            tft.fillRect(0, 0, 200, 750, RA8875_RED);

            tft.textMode();
            String hour2 = (String)hour;

            if (hour2 == "0") {
              hour2 = 12;
            }
            if (hour > 12) {
              hour2 = String(hour % 12);
            }
            if (hour2.length() == 1) {
              hour2 = " " + hour2;
            }

            char string[hour2.length() - 3];

            for (int i = 0; i < hour2.length(); i++) {
              string[i] = hour2[i];
            }


            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(5);

            if (hour2[0] == ' ') {
              tft.textSetCursor(50, 75);
            }
            else {
              tft.textSetCursor(65, 75);
            }

            tft.textWrite(string);

            tft.graphicsMode();
          }

          if (ty > 500) {
            alarmPrev = (hour * 60) + ((minuteDigitOne * 10) + minuteDigitTwo);
            Serial.println(String(alarmPrev));

            tft.fillRect(0, 500, 195, 750, RA8875_RED);
            delay(100);

            tft.textMode();
            char string9[6] = "Alarm";

            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(2);
            tft.textSetCursor(35, 400);
            tft.textWrite(string9);

            tft.graphicsMode();
            tft.textMode();
            tft.textEnlarge(1);
            char string10[] = "";
            int theHour = hour;
            boolean am = true;
            int num = 0;

            if (hour < 12) {
              am = true;
            }
            else {
              am = false;
            }

            if (theHour > 10) {
              string10[0] = String(theHour)[0];
              string10[1] = String(theHour)[1];
              string10[2] = ':';
              string10[3] = String(minuteDigitOne)[0];
              string10[4] = String(minuteDigitTwo)[0];
              num = 5;

              tft.textSetCursor(54, 370);
            }
            //            else if (theHour == 0) {
            //              string10[0] = '1';
            //              string10[1] = '2';
            //              string10[2] = ':';
            //              string10[3] = String(minuteDigitOne)[0];
            //              string10[4] = String(minuteDigitTwo)[0];
            //              num = 5;
            //
            //              tft.textSetCursor(54, 370);
            //            }
            else {
              string10[0] = String(theHour)[0];
              string10[1] = ':';
              string10[2] = String(minuteDigitOne)[0];
              string10[3] = String(minuteDigitTwo)[0];
              string10[4] = ' ';
              num = 4;

              tft.textSetCursor(65, 370);
            }

            tft.textWrite(string10);
            tft.graphicsMode();

            //            tft.textMode();
            //            tft.textEnlarge(1);
            //
            //            if ((hour % 12) < 12 && hour != 0) {
            //              char string[4];
            //
            //              string[0] = String(hour)[0];
            //              string[1] = String(hour)[1];
            //              string[2] = ':';
            //              string[3] = String(minuteDigitOne)[0];
            //              string[4] = String(minuteDigitTwo)[0];
            //              tft.textSetCursor(57, 370);
            //              tft.textWrite(string);
            //            }
            //            else if (hour == 0) {
            //              char string[4];
            //
            //              string[0] = '1';
            //              string[1] = '2';
            //              string[2] = ':';
            //              string[3] = String(minuteDigitOne)[0];
            //              string[4] = String(minuteDigitTwo)[0];
            //              tft.textSetCursor(57, 370);
            //              tft.textWrite(string);
            //            }
            //            else {
            //              char string1[4];
            //
            //              string1[0] = String(hour)[0];
            //              string1[1] = ':';
            //              string1[2] = String(minuteDigitOne)[0];
            //              string1[3] = String(minuteDigitTwo)[0];
            //              tft.textSetCursor(57, 370);
            //              tft.textWrite(string1);
            //
            //            }
            //
            //            tft.graphicsMode();
          }
        }
        else if (tx >= 250 && tx <= 500) {
          if (ty < 500) {
            if (minuteDigitOne >= 5) {
              minuteDigitOne = 0;
            }
            else {
              minuteDigitOne += 1;
            }
            //Serial.println("add minute digit (1): " + String(minuteDigitOne));
            tft.fillRect(200, 0, 200, 750, RA8875_GREEN);


            tft.textMode();
            String min1 = (String)minuteDigitOne;
            min1 = " " + min1;
            char string[min1.length() - 3];
            for (int i = 0; i < min1.length(); i++) {
              string[i] = min1[i];
            }


            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(5);
            tft.textSetCursor(250, 75);
            tft.textWrite(string);

            tft.graphicsMode();

          }

          if (ty > 500) {
            alarmPrev = -1;
            Serial.println(String(alarmPrev));

            tft.fillRect(200, 500, 200, 750, RA8875_GREEN);
            delay(100);

            tft.textMode();
            char string9[5] = "None";

            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(2);
            tft.textSetCursor(250, 400);
            tft.textWrite(string9);

            tft.graphicsMode();

            tft.fillRect(0, 500, 195, 750, RA8875_RED);
            delay(100);

            tft.textMode();
            char string2[6] = "Alarm";

            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(2);
            tft.textSetCursor(35, 400);
            tft.textWrite(string2);

            tft.graphicsMode();
          }

          return;
        }
        else if (tx >= 500 && tx <= 750) {
          if (ty < 500) {
            if (minuteDigitTwo >= 9) {
              minuteDigitTwo = 0;
            }
            else {
              minuteDigitTwo += 1;
            }
            //Serial.println("add minute digit (2): " + String(minuteDigitTwo));
            //Serial.print(tx); Serial.print(", "); Serial.println(ty);
            tft.fillRect(400, 0, 200, 750, RA8875_CYAN);

            tft.textMode();
            String min1 = (String)minuteDigitTwo;
            min1 = " " + min1;
            char string[min1.length() - 3];

            for (int i = 0; i < min1.length(); i++) {
              string[i] = min1[i];
            }


            tft.textTransparent(RA8875_BLACK);
            tft.textEnlarge(5);
            tft.textSetCursor(450, 75);
            tft.textWrite(string);

            tft.graphicsMode();
          }

          if (ty > 500) {
            int m = rtc.now().month();
            int d = rtc.now().date();
            int y = rtc.now().year();
            int w = rtc.now().dayOfWeek();

            DateTime dt(y, m, d, hour, (String(minuteDigitOne) + String(minuteDigitTwo)).toInt(), 0, w);
            rtc.setDateTime(dt);

            dateSetting = true;
            tft.fillScreen(RA8875_BLACK);
            changeText();
            return;
          }

          return;
        }
        else if (tx >= 750 && tx <= 1000) {
          if (ty < 500) {
            if (hour > 11) {
              hour -= 12;
            }
            else {
              hour += 12;
            }
            tft.fillRect(600, 0, 200, 750, RA8875_YELLOW);


            tft.textMode();
            if (hour < 12) {
              char string[4] = "AM ";

              tft.textTransparent(RA8875_BLACK);
              tft.textEnlarge(5);
              tft.textSetCursor(665, 75);
              tft.textWrite(string);

              tft.graphicsMode();
            }
            else {
              char string[4] = "PM ";

              tft.textTransparent(RA8875_BLACK);
              tft.textEnlarge(5);
              tft.textSetCursor(665, 75);
              tft.textWrite(string);

              tft.graphicsMode();
            }
          }

          if (ty > 500) {
            setting = false;
            tft.fillScreen(RA8875_BLACK);
            bmpDraw("son1.bmp", -10, 10);
            delay(100);
          }
        }
      }
    }
  }
  else {
    Serial.println("setting is false");
    dt = rtc.now();
    start = millis();
    first = millis();

    //alarmPrev = toPrev(rtc.now()).toInt();

    updateSerial(toPrev(rtc.now()).toInt());

    while (rtc.now().second() != 0) {
      Serial.println("Not zero: " + String(rtc.now().hour()) + ":" + String(rtc.now().minute()) + ":" + String(rtc.now().second()));
      if (tft.touched()) {
        tft.touchRead(&tx, &ty);
        Serial.println(String(tx) + ", " + String(ty));

        if (tft.touched()) {
          tft.touchRead(&tx, & ty);

          if ((tx / (1024.0F / tft.width())) < 200 && (ty / (1024.0F / tft.height())) < 200) {
            delay(10);
            resetFunc();
          }
        }

        //        if (tx / (1024.0F / tft.width()) < 200 && ty / (1024.0F / tft.height()) < 200) {
        //          setting = true;
        //          delay(100);
        //
        //          tft.fillRect(0, 0, 200, 750, RA8875_RED);
        //          tft.fillRect(200, 0, 200, 750, RA8875_GREEN);
        //          tft.fillRect(400, 0, 200, 750, RA8875_CYAN);
        //          tft.fillRect(600, 0, 200, 750, RA8875_YELLOW);
        //          tft.fillRect(0, 510, 200, 240, RA8875_RED);
        //          tft.fillRect(200, 510, 200, 240, RA8875_GREEN);
        //          tft.fillRect(400, 510, 200, 240, RA8875_CYAN);
        //          tft.fillRect(600, 510, 200, 240, RA8875_YELLOW);
        //
        //          tft.textMode();
        //          char string[4] = "12 ";
        //          tft.textSetCursor(65, 75);
        //
        //          if (rtc.now().hour() < 12) {
        //            if (rtc.now().hour() == 0) {
        //              string[0] = '1';
        //              string[1] = '2';
        //            }
        //            else {
        //              string[0] = ' ';
        //              string[1] = String(rtc.now().hour())[0];
        //              tft.textSetCursor(50, 75);
        //            }
        //          }
        //          else {
        //            string[0] = String(rtc.now().hour() % 12)[0];
        //            string[1] = String(rtc.now().hour() % 12)[1];
        //            tft.textSetCursor(80, 75);
        //          }
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(5);
        //
        //
        //          tft.textWrite(string);
        //          char string2[3] = " 0";
        //
        //          string2[0] = ' ';
        //
        //          if (rtc.now().minute() < 10) {
        //            string2[1] = '0';
        //          }
        //          else {
        //            string2[1] = String(rtc.now().minute())[0];
        //          }
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(5);
        //          tft.textSetCursor(250, 75);
        //          tft.textWrite(string2);
        //
        //
        //          char string3[3] = " 0";
        //
        //          string3[0] = ' ';
        //
        //          if (rtc.now().minute() < 10) {
        //            string3[1] = '0';
        //          }
        //          else {
        //            string3[1] = String(rtc.now().minute())[1];
        //          }
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(5);
        //          tft.textSetCursor(450, 75);
        //          tft.textWrite(string3);
        //
        //
        //
        //          char string4[4] = "AM ";
        //
        //          if (rtc.now().hour() > 11) {
        //            string4[0] = 'P';
        //            string4[1] = 'M';
        //          }
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(5);
        //          tft.textSetCursor(665, 75);
        //          tft.textWrite(string4);
        //
        //
        //
        //
        //          //  char string5[4] = "Set";
        //          //
        //          //  tft.textTransparent(RA8875_WHITE);
        //          //  tft.textEnlarge(4);
        //          //  tft.textSetCursor(635, 10);
        //          //  tft.textWrite(string5);
        //          //
        //          //  char string7[6] = "Alarm";
        //          //
        //          //  tft.textTransparent(RA8875_WHITE);
        //          //  tft.textEnlarge(4);
        //          //  tft.textSetCursor(600, 150);
        //          //  tft.textWrite(string7);
        //
        //          char string9[6] = "Alarm";
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(2);
        //          tft.textSetCursor(35, 400);
        //          tft.textWrite(string9);
        //
        //
        //
        //          char string8[5] = "None";
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(2);
        //          tft.textSetCursor(250, 400);
        //          tft.textWrite(string8);
        //
        //
        //          char string7[4] = "Set";
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(2);
        //          tft.textSetCursor(465, 400);
        //          tft.textWrite(string7);
        //
        //          char string6[5] = "Load";
        //
        //          tft.textTransparent(RA8875_BLACK);
        //          tft.textEnlarge(2);
        //          tft.textSetCursor(650, 400);
        //          tft.textWrite(string6);
        //
        //          tft.graphicsMode();
        //
        //          delay(250);
        //        }
        //      }
        //      rtc.convertTemperature();
        //      Serial.print(rtc.getTemperature()* 9 / 5 + 32); //read registers and display the temperature
        //      Serial.println("°F");
      }
    }
  }
}
#define BUFFPIXEL 20
void bmpDraw(char *filename, int x, int y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.println(F("File size: "));
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: "));
    Serial.println(bmpImageoffset, DEC);

    // Read DIB header
    Serial.print(F("Header size: "));
    Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: "));
      Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds

        for (row = 0; row < h; row++) { // For each scanline...
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if (lcdidx > 0) {
                tft.drawPixel(col + x, row + y, lcdbuffer[lcdidx]);
                lcdidx = 0;
                first  = false;
              }

              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx] = color565(r, g, b);
            tft.drawPixel(col + x, row + y, lcdbuffer[lcdidx]);


          } // end pixel
          if (tft.touched()) {
            tft.touchRead(&tx, & ty);

            if ((tx / (1024.0F / tft.width())) < 200 && (ty / (1024.0F / tft.height())) < 200) {
              delay(10);
              resetFunc();
            }
          }
        } // end scanline

        // Write any remaining data to LCD
        if (lcdidx > 0) {
          tft.drawPixel(col + x, row + y, lcdbuffer[lcdidx]);
        }

        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");

      } // end goodBmp
    }
  }
  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));

}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

byte decToBcd(byte val) {
  // Convert normal decimal numbers to binary coded decimal
  return ( (val / 10 * 16) + (val % 10) );
}
boolean out = false;
void updateSerial(int previous) {
  Serial.println(prev);
  //tft.fillRect(0, 350, 800, 150, RA8875_BLACK);
  analogWrite(3, 0);
  boolean dim = false;
  boolean on = false;

  digitalWrite(5, LOW);
  if (toPrev(rtc.now()).toInt() == alarmPrev) {
    analogWrite(5, 170);


    tft.fillScreen(RA8875_CYAN);
    tft.fillRect(0, 0, 400, 480, RA8875_BLUE);

    tft.textMode();
    char string2[10] = "5 minutes";
    tft.textEnlarge(1);
    tft.textSetCursor(120, 125);
    tft.textTransparent(RA8875_WHITE);
    tft.textWrite(string2);
    char string[8] = "Dismiss";
    tft.textEnlarge(6);
    tft.textSetCursor(500, 65);
    tft.textTransparent(RA8875_BLACK);
    tft.textWrite(string);
    char string1[7] = "Snooze";
    tft.textSetCursor(100, 65);
    tft.textTransparent(RA8875_WHITE);
    tft.textWrite(string1);
    tft.graphicsMode();

    inAlarm = true;




    analogWrite(7, 0);

    while (inAlarm) {
      analogWrite(3, 255);
      //delay(200);
      while (!out) {
        if (digitalRead(8) == 0) {
          digitalWrite(3, HIGH);
          delay(10);
          digitalWrite(3, LOW);
        }

        if (tft.touched()) {
          tft.touchRead(&tx, &ty);
          Serial.println(String(tx));
          Serial.println(String(ty));
          analogWrite(9, 255);

          if (tx < 500) {
            alarmPrev = toPrev(rtc.now()).toInt() + 5;
            Serial.println(alarmPrev);
            digitalWrite(3, LOW);
            out = true;
          }
          else {
            alarmPrev = -1;
            Serial.println(alarmPrev);
            delay(50);
            digitalWrite(3, LOW);
            //analogWrite(8, 255);
            out = true;
          }

          analogWrite(10, 100);
          delay(10);
          analogWrite(10, 0);
        }


        int red = random(0, 256);
        int green = random(0, 256);
        int blue = random(0, 256);
        for (int i = 0, e = 0; i < 256; i++, e++) {
          analogWrite(4, i);

          if (e < 12) {
            pixels.setPixelColor(e, pixels.Color(red, green, blue));
            pixels.show();
          }

          delay(3);
        }

        red = random(0, 256);
        green = random(0, 256);
        blue = random(0, 256);
        for (int i = 255, e = 0; i >= 0; i--, e++) {
          analogWrite(4, i);

          if (e < 12) {
            pixels.setPixelColor(e, pixels.Color(red, green, blue));
            pixels.show();
          }

          delay(3);
        }
      }

      digitalWrite(5, LOW);

      inAlarm = false;
      out = false;
    }

    tft.fillScreen(RA8875_BLACK);
  }

  String day = "";
  if (rtc.now().dayOfWeek() == 0) {
    day = "Sunday";
  }
  else if (rtc.now().dayOfWeek() == 1) {
    day = "Monday";
  }
  else if (rtc.now().dayOfWeek() == 2) {
    day = "Tuesday";
  }
  else if (rtc.now().dayOfWeek() == 3) {
    day = "Wednesday";
  }
  else if (rtc.now().dayOfWeek() == 4) {
    day = "Thursday";
  }
  else if (rtc.now().dayOfWeek() == 5) {
    day = "Friday";
  }
  else if (rtc.now().dayOfWeek() == 6) {
    day = "Saturday";
  }

  String month1 = "";
  if (rtc.now().month() == 1) {
    month1 = "January";
  }
  else if (rtc.now().month() == 2) {
    month1 = "February";
  }
  else if (rtc.now().month() == 3) {
    month1 = "March";
  }
  else if (rtc.now().month() == 4) {
    month1 = "April";
  }
  else if (rtc.now().month() == 5) {
    month1 = "May";
  }
  else if (rtc.now().month() == 6) {
    month1 = "June";
  }
  else if (rtc.now().month() == 7) {
    month1 = "July";
  }
  else if (rtc.now().month() == 8) {
    month1 = "August";
  }
  else if (rtc.now().month() == 9) {
    month1 = "September";
  }
  else if (rtc.now().month() == 10) {
    month1 = "October";
  }
  else if (rtc.now().month() == 11) {
    month1 = "November";
  }
  else if (rtc.now().month() == 12) {
    month1 = "December";
  }

  String full = day + ", " + month1 + " " + String(rtc.now().date()) + "  " + String(rtc.now().year());
  String ctrFull = day + ", " + month1 + " " + String(rtc.now().date());
  Serial.println(full);

  char string[full.length() - 15];

  for (int i = 0; i < full.length(); i++) {
    string[i] = full[i];
  }


  tft.textMode();
  tft.textEnlarge(2);
  tft.textTransparent(RA8875_WHITE);

  int num = ctrFull.length() * 20;

  int one = 400 - (num / 2);
  int two = one / 2;
  int three = (one + two) / 2;

  tft.textSetCursor(one - 30, 340);

  tft.fillRect(0, 350, 800, -380, RA8875_BLACK);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(string);
  tft.graphicsMode();

  tft.fillRect((one + num) + 35, 350, 200, 35, RA8875_BLACK);

  //tft.fillRect(300, 450, 500, 950, RA8875_BLACK);

  //  if (full.length() == 8) {
  //    char one = String(rtc.now().month())[0];
  //    char two = '/';
  //    char three = String(rtc.now().date())[0];
  //    char four = '/';
  //    char five = String(rtc.now().year())[0];
  //    char six = String(rtc.now().year())[1];
  //    char seven = String(rtc.now().year())[2];
  //    char eight = String(rtc.now().year())[3];
  //
  //    char full2[7];
  //
  //    full2[0] = one;
  //    full2[1] = two;
  //    full2[2] = three;
  //    full2[3] = four;
  //    full2[4] = five;
  //    full2[5] = six;
  //    full2[6] = seven;
  //    full2[7] = eight;
  //    full2[8] = ' ';
  //    full2[9] = ' ';
  //
  //    tft.textMode();
  //
  //    tft.textEnlarge(2);
  //    tft.textTransparent(RA8875_WHITE);
  //    tft.textSetCursor(310, 900);
  //
  //    tft.textWrite(full2);
  //    tft.graphicsMode();
  //  }
  //  else if (full.length() == 9 && String(rtc.now().month()).length() == 2) {
  //    char one = String(rtc.now().month())[0];
  //    char two = String(rtc.now().month())[1];
  //    char three = '/';
  //    char four = String(rtc.now().date())[0];
  //    char five = '/';
  //    char six = String(rtc.now().year())[0];
  //    char seven = String(rtc.now().year())[1];
  //    char eight = String(rtc.now().year())[2];
  //    char nine = String(rtc.now().year())[3];
  //
  //    char full2[7];
  //
  //    full2[0] = one;
  //    full2[1] = two;
  //    full2[2] = three;
  //    full2[3] = four;
  //    full2[4] = five;
  //    full2[5] = six;
  //    full2[6] = seven;
  //    full2[7] = eight;
  //    full2[8] = nine;
  //    full2[9] = ' ';
  //
  //    tft.textMode();
  //
  //    tft.textEnlarge(2);
  //    tft.textTransparent(RA8875_WHITE);
  //    tft.textSetCursor(300, 900);
  //
  //    tft.textWrite(full2);
  //    tft.graphicsMode();
  //  }
  //  else if (full.length() == 9 && String(rtc.now().date()).length() == 2) {
  //    char one = String(rtc.now().month())[0];
  //    char two = '/';
  //    char three = String(rtc.now().date())[0];
  //    char four = String(rtc.now().date())[1];
  //    char five = '/';
  //    char six = String(rtc.now().year())[0];
  //    char seven = String(rtc.now().year())[1];
  //    char eight = String(rtc.now().year())[2];
  //    char nine = String(rtc.now().year())[3];
  //
  //    char full2[7];
  //
  //    full2[0] = one;
  //    full2[1] = two;
  //    full2[2] = three;
  //    full2[3] = four;
  //    full2[4] = five;
  //    full2[5] = six;
  //    full2[6] = seven;
  //    full2[7] = eight;
  //    full2[8] = nine;
  //    full2[9] = ' ';
  //
  //    tft.textMode();
  //
  //    tft.textEnlarge(2);
  //    tft.textTransparent(RA8875_WHITE);
  //    tft.textSetCursor(300, 900);
  //
  //    tft.textWrite(full2);
  //    tft.graphicsMode();
  //  }
  //  else {
  //    char one = String(rtc.now().month())[0];
  //    char two = String(rtc.now().month())[1];
  //    char three = '/';
  //    char four = String(rtc.now().date())[0];
  //    char five = String(rtc.now().date())[1];
  //    char six = '/';
  //    char seven = String(rtc.now().year())[0];
  //    char eight = String(rtc.now().year())[1];
  //    char nine = String(rtc.now().year())[2];
  //    char ten = String(rtc.now().year())[3];
  //
  //    char full2[7];
  //
  //    full2[0] = one;
  //    full2[1] = two;
  //    full2[2] = three;
  //    full2[3] = four;
  //    full2[4] = five;
  //    full2[5] = six;
  //    full2[6] = seven;
  //    full2[7] = eight;
  //    full2[8] = nine;
  //    full2[9] = ten;
  //
  //    tft.textMode();
  //
  //    tft.textEnlarge(2);
  //    tft.textTransparent(RA8875_WHITE);
  //    tft.textSetCursor(290, 900);
  //
  //    tft.textWrite(full2);
  //    tft.graphicsMode();
  //  }

  //tft.fillRect(750, 450, 800, 950, RA8875_BLACK);

  String Hour = "";
  String Minute = "";
  if (previous >= 0 && previous < 60) {
    Hour = "12";
    Minute = String(previous);
  }
  else if (previous >= 60 && previous < 120) {
    Hour = "1";
    Minute = String(previous - 60);
  }
  else if (previous >= 120 && previous < 180) {
    Hour = "2";
    Minute = String(previous - 140);
  }
  else if (previous >= 180 && previous < 240) {
    Hour = "3";
    Minute = String(previous - 180);
  }
  else if (previous >= 240 && previous < 300) {
    Hour = "4";
    Minute = String(previous - 240);
  }
  else if (previous >= 300 && previous < 360) {
    Hour = "5";
    Minute = String(previous - 300);
  }
  else if (previous >= 360 && previous < 420) {
    Hour = "6";
    Minute = String(previous - 360);
  }
  else if (previous >= 420 && previous < 480) {
    Hour = "7";
    Minute = String(previous - 420);
  }
  else if (previous >= 480 && previous < 540) {
    Hour = "8";
    Minute = String(previous - 480);
  }
  else if (previous >= 540 && previous < 600) {
    Hour = "9";
    Minute = String(previous - 540);
  }
  else if (previous >= 600 && previous < 660) {
    Hour = "10";
    Minute = String(previous - 600);
  }
  else if (previous >= 660 && previous < 720) {
    Hour = "11";
    Minute = String(previous - 660);
  }
  else if (previous >= 720 && previous < 780) {
    Hour = "12";
    Minute = String(previous - 720);
  }
  else if (previous >= 780 && previous < 840) {
    Hour = "13";
    Minute = String(previous - 780);
  }
  else if (previous >= 840 && previous < 900) {
    Hour = "14";
    Minute = String(previous - 840);
  }
  else if (previous >= 900 && previous < 960) {
    Hour = "15";
    Minute = String(previous - 900);
  }
  else if (previous >= 960 && previous < 1020) {
    Hour = "16";
    Minute = String(previous - 960);
  }
  else if (previous >= 1020 && previous < 1080) {
    Hour = "17";
    Minute = String(previous - 1020);
  }
  else if (previous >= 1080 && previous < 1140) {
    Hour = "18";
    Minute = String(previous - 1080);
  }
  else if (previous >= 1140 && previous < 1200) {
    Hour = "19";
    Minute = String(previous - 1140);
  }
  else if (previous >= 1200 && previous < 1260) {
    Hour = "20";
    Minute = String(previous - 1200);
  }
  else if (previous >= 1260 && previous < 1320) {
    Hour = "21";
    Minute = String(previous - 1260);
  }
  else if (previous >= 1320 && previous < 1380) {
    Hour = "22";
    Minute = String(previous - 1320);
  }
  else if (previous >= 1380 && previous < 1440) {
    Hour = "23";
    Minute = String(previous - 1380);
  }

  if (Minute.length() == 1) {
    String newNum = "0" + (String)Minute;
    Minute = newNum;
  }

  Serial.println(Hour + ':' + Minute);

  if (alarmPrev != -1) {
    tft.fillCircle(35, 35, 20, RA8875_RED);
  }
  else {
    tft.fillCircle(35, 35, 20, RA8875_BLACK);
  }

  if (((rtc.now().hour() * 60) + rtc.now().minute()) >= 0 && ((rtc.now().hour() * 60) + rtc.now().minute()) < 720) {
    analogWrite(4, 63.75);
    //Serial.println("AM");
  }
  else {
    analogWrite(4, 0);
    //Serial.println("PM");
  }
  //  tft.fillRect(0, 0, 800, 155, RA8875_BLACK);
  //  String one;
  //  String two;
  //  one = String(Hour.toInt() % 12) + "F.bmp";
  //  two = String(Hour.toInt() % 12) + "L.bmp";
  //Serial.println("two: " + two);

  if (Minute == "59") {
    dim = true;
    on = false;

    //    if (Hour.toInt() < 10) {
    //      char array[7];
    //      two.toCharArray(array, 7);
    //      bmpDraw(array, 20, 20);
    //      //Serial.println(array);
    //    }
    //    else {
    //      char array[8];
    //      two.toCharArray(array, 8);
    //      bmpDraw(array, 20, 20);
    //    }
  }
  else if (Minute == "00") {
    dim = false;
    on = true;

    for (int i = 0; i < 12; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 102, 0));
      pixels.show();
      delay(20);
    }
  }

  //
  //    if (Hour.toInt() < 10) {
  //      char array[7];
  //      one.toCharArray(array, 7);
  //      bmpDraw(array, 20, 20);
  //      //Serial.println(array);
  //    }
  //    else {
  //      char array[8];
  //      one.toCharArray(array, 8);
  //      bmpDraw(array, 20, 20);
  //    }

  //    if (Hour == "0" || Hour == "12") {
  //      bmpDraw("12F.bmp", 40, 40);
  //    }
  //    if (Hour == "1" || Hour == "13") {
  //      bmpDraw("1F.bmp", 40, 40);
  //    }
  //    if (Hour == "2" || Hour == "14") {
  //      bmpDraw("2F.bmp", 40, 40);
  //    }
  //    if (Hour == "3" || Hour == "15") {
  //      bmpDraw("3F.bmp", 40, 40);
  //    }
  //    if (Hour == "4" || Hour == "16") {
  //      bmpDraw("4F.bmp", 40, 40);
  //    }
  //    if (Hour == "5" || Hour == "17") {
  //      bmpDraw("5F.bmp", 40, 40);
  //    }
  //    if (Hour == "6" || Hour == "18") {
  //      bmpDraw("6F.bmp", 40, 40);
  //    }
  //    if (Hour == "7" || Hour == "19") {
  //      bmpDraw("7F.bmp", 40, 40);
  //    }
  //    if (Hour == "8" || Hour == "40") {
  //      bmpDraw("8F.bmp", 40, 40);
  //    }
  //    if (Hour == "9" || Hour == "21") {
  //      bmpDraw("9F.bmp", 40, 40);
  //    }
  //    if (Hour == "10" || Hour == "22") {
  //      bmpDraw("10F.bmp", 40, 40);
  //    }
  //    if (Hour == "11" || Hour == "23") {
  //      bmpDraw("11F.bmp", 40, 40);
  //    }
  else {
    for (int i = 0; i < 12; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      pixels.show();
      delay(20);
    }

    //    if (Hour == "1") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(106, 106, 106));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: grey");
    //    }
    //    else if (Hour == "2") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(174, 160, 0));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: yellow");
    //    }
    //    else if (Hour == "3") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(154, 49, 169));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: purple");
    //    }
    //    else if (Hour == "4") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(8, 176, 0));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: green");
    //    }
    //    else if (Hour == "5") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(160, 92, 161));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: magenta");
    //    }
    //    else if (Hour == "6") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(7, 137, 150));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: light blue");
    //    }
    //    else if (Hour == "7") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(0, 43, 167));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: blue");
    //    }
    //    else if (Hour == "8") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(169, 21, 95));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: pink");
    //    }
    //    else if (Hour == "9") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(159, 0, 0));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: red");
    //    }
    //    else if (Hour == "10") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(0, 156, 166));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: slightly lighter blue");
    //    }
    //    else if (Hour == "11") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(89, 161, 171));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: even lighter blue");
    //    }
    //    else if (Hour == "12") {
    //      for (int i = 0; i < 12; i++) {
    //        pixels.setPixelColor(i, pixels.Color(166, 28, 28));
    //        pixels.show();
    //      }
    //
    //      Serial.println("Color on inside: dark red");
    //    }


    dim = false;
    on = false;
  }


  //

  if (Minute[1] == '0') {
    bmpDraw("0.bmp", 542.5, 160);
  }
  else if (Minute[1] == '1') {
    bmpDraw("1.bmp", 542.5, 160);
  }
  else if (Minute[1] == '2') {
    bmpDraw("2.bmp", 542.5, 160);
  }
  else if (Minute[1] == '3') {
    bmpDraw("3.bmp", 542.5, 160);
  }
  else if (Minute[1] == '4') {
    bmpDraw("4.bmp", 542.5, 160);
  }
  else if (Minute[1] == '5') {
    bmpDraw("5.bmp", 542.5, 160);
  }
  else if (Minute[1] == '6') {
    bmpDraw("6.bmp", 542.5, 160);
  }
  else if (Minute[1] == '7') {
    bmpDraw("7.bmp", 542.5, 160);
  }
  else if (Minute[1] == '8') {
    bmpDraw("8.bmp", 542.5, 160);
  }
  else if (Minute[1] == '9') {
    bmpDraw("9.bmp", 542.5, 160);
  }

  if (Minute[0] == '0') {
    bmpDraw("0.bmp", 401.5, 160);
  }
  else if (Minute[0] == '1') {
    bmpDraw("1.bmp", 401.5, 160);
  }
  else if (Minute[0] == '2') {
    bmpDraw("2.bmp", 401.5, 160);
  }
  else if (Minute[0] == '3') {
    bmpDraw("3.bmp", 401.5, 160);
  }
  else if (Minute[0] == '4') {
    bmpDraw("4.bmp", 401.5, 160);
  }
  else if (Minute[0] == '5') {
    bmpDraw("5.bmp", 401.5, 160);
  }

  if (Hour == "1" || Hour == "13") {
    bmpDraw("1.bmp", 116, 160);
  }
  else if (Hour == "2" || Hour == "14") {
    bmpDraw("2.bmp", 116, 160);
  }
  else if (Hour == "3" || Hour == "15") {
    bmpDraw("3.bmp", 116, 160);
  }
  else if (Hour == "4" || Hour == "16") {
    bmpDraw("4.bmp", 116, 160);
  }
  else if (Hour == "5" || Hour == "17") {
    bmpDraw("5.bmp", 116, 160);
  }
  else if (Hour == "6" || Hour == "18") {
    bmpDraw("6.bmp", 116, 160);
  }
  else if (Hour == "7" || Hour == "19") {
    bmpDraw("7.bmp", 116, 160);
  }
  else if (Hour == "8" || Hour == "20") {
    bmpDraw("8.bmp", 116, 160);
  }
  else if (Hour == "9" || Hour == "21") {
    bmpDraw("9.bmp", 116, 160);
  }
  else if (Hour == "10" || Hour == "22") {
    bmpDraw("10.bmp", 116, 160);
  }
  else if (Hour == "11" || Hour == "23") {
    bmpDraw("11.bmp", 116, 160);
  }
  else if (Hour == "12" || Hour == "24") {
    bmpDraw("12.bmp", 116, 160);
  }


  bmpDraw("colon.bmp", 258.5, 160);
  //  if (on) {
  //    digitalWrite(7, LOW);
  //  }
  //
  //  if (analogRead(7) > 50) {
  //    digitalWrite(7, LOW);
  //    digitalWrite(6, HIGH);
  //  }
  //  Serial.println("");
  //  Serial.println(prev);
  subtract = millis() - first;
  //  Serial.println(subtract);
}
void convertToMinCount24(String theTime) {
  int newNum = 0;
  String hour;
  String minute;
  if (theTime.length() == 4) {
    hour = theTime[0];
    minute = theTime[2] + theTime[3];
    //Serial.println(minute);
  }
  else {
    hour = theTime[0] + theTime[1];
    minute = theTime[3] + theTime[4];
    //    Serial.println(minute);
  }
  //  Serial.println(theTime.length());
  newNum += hour.toInt() * 60;
  //  Serial.println(newNum);
  newNum += minute.toInt() - 27;
  //  Serial.println(newNum);
  prev = newNum;
  tft.fillScreen(RA8875_BLACK);
  setting = false;
}


