#include <Arduino.h>
#include "credentials.h"
#if defined(ESP32)
#include <WiFi.h>
// #include <WebServer.h>
#include "ESPAsyncWebServer.h"
// #include <ArduinoOTA.h>

#endif

/**   WIFI credentials **/
const char *ssid = mySSID;
const char *password = myPASSWORD;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int status = WL_IDLE_STATUS;

void printWifiStatus();
void onConnected();
void WifiDisconnect();
String processor(const String &var);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
void WifiInit()
{

    // We start by connecting to a WiFi network
    // To debug, please enable Core Debug Level to Verbose

    Serial.println();
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    // Auto reconnect is set true as default
    // To set auto connect off, use the following function
    //    WiFi.setAutoReconnect(false);

    // Will try for about 10 seconds (20x 500ms)
    int tryDelay = 666;
    int numberOfTries = 20;

    // Wait for the WiFi event
    while (true)
    {

        switch (WiFi.status())
        {
        case WL_NO_SSID_AVAIL:
            Serial.println("[WiFi] SSID not found");
            break;
        case WL_CONNECT_FAILED:
            Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
            return;
            break;
        case WL_CONNECTION_LOST:
            Serial.println("[WiFi] Connection was lost");
            break;
        case WL_SCAN_COMPLETED:
            Serial.println("[WiFi] Scan is completed");
            break;
        case WL_DISCONNECTED:
            Serial.println("[WiFi] WiFi is disconnected");
            break;
        case WL_CONNECTED:
            onConnected();
            return;
            break;
        default:
            Serial.print("[WiFi] WiFi Status: ");
            Serial.println(WiFi.status());
            break;
        }
        delay(tryDelay);

        if (numberOfTries <= 0)
        {
            Serial.print("[WiFi] Failed to connect to WiFi!");
            // Use disconnect function to force stop trying to connect
            WifiDisconnect();
            return;
        }
        else
        {
            numberOfTries--;
        }
    }
}
void WifiLoop()
{
    // check for WiFi OTA updates
    //   ArduinoOTA.poll();
    //   ArduinoOTA.handle(); // Handles a code update request
    // All loop you're code goes here.
}
String processor(const String &var)
{
    // Serial.println(var);
    if (var == "TEMPERATURE")
    {
        return readDHTTemperature();
        // return "22";
    }
    else if (var == "HUMIDITY")
    {
        return readDHTHumidity();
        // return "66";
    }
    return String();
}
void handle_OnConnect()
{
    // LED1status = LOW;
    // LED2status = LOW;
    // Route for root / web page
    //         server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html, processor); });
    /*     server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });
        server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/plain", readDHTHumidity().c_str()); });
        Serial.println("[WIFI] onConnect"); */
} // Replaces placeholder with DHT values

void handle_NotFound()
{
    /*   server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", readDHTHumidity().c_str());
      });
        server.send(404, "text/plain", "Not found"); */
}
void onConnected()
{
    // handle_OnConnect();
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    /*     server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/plain", readDHTTemperature().c_str()); });
        server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send_P(200, "text/plain", readDHTHumidity().c_str()); });
        Serial.println("[WIFI] onConnect"); */

    // Start server
    server.begin();
    timeInit();

    Serial.println("[WiFi] WiFi is connected!");
    Serial.print("[WiFi] IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print(getNiceTime());

    const String ahora = getNiceTime();
    /*     int ahoraLength = strlen(ahora);
        Serial.print(ahoraLength);
        char Buf[ahoraLength]; */
    const char *timeNow = ahora.c_str();
    // getNiceTime().toCharArray(Buf, 50);
    //   Serial.print(String(getNiceTime()));
    Serial.print(timeNow);
    //    appendFile(LittleFS, "/data/log.txt", getNiceTime());
    appendFile(LittleFS, "/data/log.txt", "WiFi is connected\r\n");
    appendFile(LittleFS, "/data/log.txt", timeNow);
}

void WifiDisconnect()
{
    // disconnect WiFi as it's no longer needed

    const String ahora = getNiceTime();
    /*     int ahoraLength = strlen(ahora);
        Serial.print(ahoraLength);
        char Buf[ahoraLength]; */
    const char *timeNow = ahora.c_str();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[WiFi] WiFi disconnected!");
    // appendFile(LittleFS, "/data/log.txt", "WiFi disconnected");
    appendFile(LittleFS, "/data/log.txt", "WiFi disconnected");
    appendFile(LittleFS, "/data/log.txt", timeNow);
}