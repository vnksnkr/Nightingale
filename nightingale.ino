#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <ctype.h>
#include <EEPROM.h>
#include <string.h>
RTC_DS1307 rtc;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // (rs, e, d4, d5, d6, d7)
Servo servo1;
Servo servo2;
String text;
int index;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char times[5][5];
char med[5][10];
int weekday[7][5],commas=0;
int daycount = 0;
char buffer1[100];
char buffer2[10];
String buffer3 = "";
char dot = '.';
String currenttime;
int count = 0, debug=0,i;
void setup () 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  EEPROM.write(0,0);
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
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));// to set the time manualy 
  servo1.attach(A0);
  servo2.attach(A1);
  i=0;
  //Serial.print("hello");
}

void loop () 
{
  DateTime now = rtc.now();
  int dayno = now.dayOfTheWeek();
  int hour = now.hour();
  int minute = now.minute(); 
  count = EEPROM.read(0);
  //delay(1000);
  commas = 0;
  int i=0;
  if(Serial.available())
  count++;
  EEPROM.write(0, count);
  while(Serial.available())
    {
      delay(10);
      char c =  Serial.read();
      buffer1[i++] = c;
    }
    buffer1[i]='\0';
    int k =0;
    for(int j = 0; j<i ;j++) {
      buffer2[k] = buffer1[j];  
      if(buffer2[k] == ',')
      {
        if(commas == 0)
        {
        buffer2[k]='\0' ;
        strcpy(times[count],buffer2);
        k = 0;
        commas += 1;
        }
        else if(commas == 1)
        {
          buffer2[k]='\0';
          strcpy(med[count],buffer2);
          delay(1000);
          k = 0;
          commas +=1;
        }
      }
         else if(commas == 2)
        {
          if(buffer2[k]!= '8')
          {
          //lcd.print("reading");
          //lcd.print(buffer2[k]);
          delay(1000);
          lcd.clear();
          if(buffer2[k] == '6')
          index = 0;
          else
          index = (int)(buffer2[k]-'0') + 1;
          weekday[index][daycount] = count;
          }
          k++;
        }
        else
        k++;
      }
   for(int j=0;j<count;j++)
    {
      if((float)hour+((float)minute/100) == atof(times[weekday[dayno][j]]))
      {
      lcd.clear();  
      lcd.print(times[weekday[dayno][j]]);
      lcd.print("   ");  
      lcd.print(med[weekday[dayno][j]]);
      for(int m=5;m <= 180;m+=60)
      {
        servo1.write(m);
        servo2.write(m);
        delay(15);
      }
      delay(6000);
      }
    }
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print("   ");
    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.print(" ,");
    lcd.print(now.day());
    lcd.print('/');
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.year());
    }
    
