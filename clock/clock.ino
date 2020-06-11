#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);
float t=1.5;

const char *ssid     = "Vimal";
const char *password = "9821088672";

const long utcOffsetInSeconds = 19800;

String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  Serial.begin(115200);

  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  display.setTextSize(t);
  display.setTextColor(WHITE);
  
  // Clear the buffer.
  display.clearDisplay();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() 
{
  display.clearDisplay();
  
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();

  String days=daysOfTheWeek[timeClient.getDay()];
  int hrs=timeClient.getHours();
  
  String ampm;
  if(hrs>12)
  {
      hrs=hrs-12;
      ampm="PM";      
  }

  else
  {
    ampm="AM";
  }
  
  int mins=timeClient.getMinutes();
  int secs=timeClient.getSeconds();

  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;

  int currentMonth = ptm->tm_mon+1;
  
  String currentMonthName = months[currentMonth-1];

  int currentYear = ptm->tm_year+1900;

  String currentTime = String(hrs) + ":" + String(mins) + " " +ampm;

  String currentDate = String(monthDay) + "/" + String(currentMonth) + "/" +String(currentYear) ;

  Serial.print(days);
  Serial.print(", ");
  Serial.print(hrs);
  Serial.print(":");
  Serial.print(mins);
  Serial.print(":");
  Serial.println(secs);
  Serial.println(currentDate);
  //Serial.println(timeClient.getFormattedTime());

  display.setTextSize(2);
  display.setCursor(17*t,2*t);
  display.println(currentTime);
  //display.display();

  display.setTextSize(1.5);
  display.setCursor(23*t,15*t);
  display.println(currentDate);
  //display.display();
  
  display.setCursor(24*t,25*t);
  display.println(days);
  //display.display();
  
  display.setCursor(26*t,35*t);
  display.println("Mumbai");
  
  display.setCursor(54*t,35*t);
  display.write(3);  
  display.display();

  delay(1000);
}
