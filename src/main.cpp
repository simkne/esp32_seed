#include <Arduino.h>
#include "TimeHandling.h"
#include "LittleFSBase.h"
#include "SensorHandling.h"
#include "WifiCon.h"
// #include "LittleFSBase.h"

// put function declarations here:


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("HIYA");

  setupFS();
  WifiInit();

  delay(400);
  Serial.print("[main] WiFi Status: ");
  Serial.println(WiFi.status());
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
  SensorsInit();
  
  Serial.println("[main]setup complete");

  Serial.print("[main][main]");
  Serial.println(getTimeStamp());
  Serial.println(getNiceTime());


  readFile(LittleFS, "/data/log.txt");

//  appendFile(LittleFS, "/data/log.txt", getTimeStamp()); // Append some text to the previous file
/*   delay(2400);
  readFile(LittleFS, "/data/data.txt"); */
}

void loop()
{
  loopFS();
  WifiLoop();
  SensorLoop();

  delay(500);
}