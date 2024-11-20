/*  expanded from Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
 */
#include <Arduino.h>
#include "time.h"
#include <string.h>

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
// char *timeNow = "";

// char *getTimeStamp();

void printLocalTime();
String getTimeStamp();
String getNiceTime();
void timeInit()
{
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("[Time] Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  /*  Serial.print("Day of week: ");
   Serial.println(&timeinfo, "%A");
   Serial.print("Month: ");
   Serial.println(&timeinfo, "%B");
   Serial.print("Day of Month: ");
   Serial.println(&timeinfo, "%d");
   Serial.print("Year: ");
   Serial.println(&timeinfo, "%Y");
   Serial.print("Hour: ");
   Serial.println(&timeinfo, "%H");
   Serial.print("Hour (12 hour format): ");
   Serial.println(&timeinfo, "%I");
   Serial.print("Minute: ");
   Serial.println(&timeinfo, "%M");
   Serial.print("Second: ");
   Serial.println(&timeinfo, "%S");

   Serial.println("Time variables");
   char timeHour[3];
   strftime(timeHour, 3, "%H", &timeinfo);
   Serial.println(timeHour);
   char timeWeekDay[10];
   strftime(timeWeekDay, 10, "%A", &timeinfo);
   Serial.println(timeWeekDay);
   Serial.println(); */
getTimeStamp();
  char buffer[30];

  // getLocalTime(&timeinfo, 5000);

  sprintf(buffer, "%d-%02d-%02d, %02d:%02d:%02d", (timeinfo.tm_year) + 1900, timeinfo.tm_mday, (timeinfo.tm_mon) + 1, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  Serial.println("[Time]");
  Serial.println(String(buffer));
  Serial.println("-_--__-_");
}
/* // Function to return a char*
const char* getString(const char* strang) {
    const char* str = strang;
    return str;
} */
String getTimeStamp()
{
//  char buffer[20];
  time_t now;
  time(&now);
  /*   Serial.print("Epoch time (UTC): ");
    Serial.println(now); */
//  buffer = now;


  return String(now);
}
String getNiceTime()
{
  struct tm timeinfo;
  char tiempo[40];
  getLocalTime(&timeinfo, 5000);
  sprintf(tiempo, "%d-%02d-%02d, %02d:%02d:%02d", (timeinfo.tm_year) + 1900, timeinfo.tm_mday, (timeinfo.tm_mon) + 1, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  //
/*   Serial.print("[Time-nice]");
  Serial.println(tiempo); */
  return String(tiempo);
}
// external var CurrentTime = getTimeStamp();
