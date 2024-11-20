#include <Arduino.h>
//#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 27 // Digital pin connected to the DHT sensor
// Uncomment the type of sensor in use:
#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)
// #define const char index_html[]

DHT dht(DHTPIN, DHTTYPE);

void writeData(String str);

float Temperature;
String readDHTTemperature();
float Humidity;
String readDHTHumidity();

uint32_t delayMS = 5000;

void SensorsInit()
{
      
    pinMode(DHTPIN, INPUT);

    dht.begin();

   /*  // Print temperature sensor details.
   // sensor_t sensor;
    //dht.readTemperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
   // dht.readHumidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
    Serial.println(F("------------------------------------")); */
    // Set delay between sensor readings based on sensor details.
    //delayMS = sensor.min_delay / 1000;
}
String readDHTTemperature()
{
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    // Read temperature as Celsius (the default)
     Temperature = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    // float t = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(Temperature))
    {
        Serial.println("[Sensor]Failed to read T from DHT sensor!");
        return "--";
    }
    else
    {
        Serial.println(Temperature);
        return String(Temperature);
    }
}

String readDHTHumidity()
{
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    Humidity = dht.readHumidity();
    if (isnan(Humidity))
    {
        Serial.println("[Sensor]Failed to read H from DHT sensor!");
        return "--";
    }
    else
    {
        Serial.println(Humidity);
        return String(Humidity);
    }
}

void writeData(){

    const String ahora = getNiceTime(); 
/*     int ahoraLength = strlen(ahora);
    Serial.print(ahoraLength);
    char Buf[ahoraLength]; */
    const char *timeNow = ahora.c_str();

    const String dataEntry =  getNiceTime() + ", " + readDHTTemperature() + ", " + readDHTHumidity() + "\n";
       
    appendFile(LittleFS, "/data/data.txt", dataEntry.c_str());
   

}


void SensorLoop()
{
    // Delay between measurements.
 writeData();
    delay(delayMS);
       readFile(LittleFS, "/data/data.txt");

}