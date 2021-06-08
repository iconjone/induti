// /****************************************************************************************************************************
//   Async_AutoConnect_ESP8266_minimal.ino
//   For ESP8266 / ESP32 boards
//   Built by Khoi Hoang https://gifrib.com/khoih-prog/ESPAsync_WiFiManager
//   Licensed under MIT license
//  *****************************************************************************************************************************/
// #include <ESPAsync_WiFiManager.h>              //https://gifrib.com/khoih-prog/ESPAsync_WiFiManager
// #define _ESPASYNC_WIFIMGR_LOGLEVEL_ 4

// AsyncWebServer server(80);
// DNSServer dnsServer;

// int LED = 16;
// int IH = 5;

// const char * PARAM_MESSAGE = "message";

// #include <LittleFS.h>
// FS* filesystem =      &LittleFS;
// #define FileFS        LittleFS
// #define FS_Name       "LittleFS"
// #define FORMAT_FILESYSTEM         true
// #define  CONFIG_FILENAME       F("/wifi_cred.dat") 
// void notFound(AsyncWebServerRequest * request) {
//   request -> send(404, "text/plain", "Not found");
// }

// void setup() {
//   Serial.begin(115200);
//   while (!Serial);
//   delay(200);

//   Serial.println("Please work buddy");
//     Serial.println("Please work buddy");


//   if( !FileFS.begin() ) {
//     Serial.println("LittleFS not mounted correctly, retrying...");
//     delay(1000);
//     if( !FileFS.begin() ) {
//       Serial.println("mounting failed twice, formatting and then restarting");
//       FileFS.format();
//       ESP.restart();
//     } else {
//       Serial.println("LittleFS mounted at second try, proceeding as usual");
//     }
//   }
//   else{
//     Serial.println("LittleFS mounted correctly");
//   }
// Serial.println(WiFi.SSID());
// Serial.println(WiFi.psk());

//   pinMode(LED, OUTPUT);
//   pinMode(IH, OUTPUT); //IH
//   digitalWrite(LED, HIGH); // turn the LED off
//   // put your setup code here, to run once:

//   Serial.print("\nStarting Async_AutoConnect_ESP8266_minimal on " + String(ARDUINO_BOARD));
//   Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
//   ESPAsync_WiFiManager ESPAsync_wifiManager( & server, & dnsServer, "Induti");
//   //ESPAsync_wifiManager.resetSettings();   //reset saved settings
//   //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192,168,186,1), IPAddress(192,168,186,1), IPAddress(255,255,255,0));
//   String Router_SSID;
//   String Router_Pass;
//   Router_SSID = ESPAsync_wifiManager.WiFi_SSID();
//   Router_Pass = ESPAsync_wifiManager.WiFi_Pass();

//   //Remove this line if you do not want to see WiFi password printed
//   Serial.println("ESP Self-Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);
//   //if()
//   ESPAsync_wifiManager.setDebugOutput(true);
//   ESPAsync_wifiManager.autoConnect("Induti");
//   delay(500);
//   while (WiFi.status() != WL_CONNECTED);
//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.print(F("Connected. Local IP: "));
//     Serial.println(WiFi.localIP());
//     Serial.println("Connect");
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
//       digitalWrite(LED, LOW); // turn the LED on
//       Serial.println("oooh");
//       request -> send(200, "text/plain", "Hello, world");
//     });
//     server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
//       digitalWrite(LED, LOW); // turn the LED on
//       digitalWrite(IH, HIGH); // turn the IH on
//       Serial.println("ih on");
//       request -> send(200, "text/plain", "Turning On the IH");
//     });
//     server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
//       digitalWrite(LED, HIGH); // turn the LED off
//       digitalWrite(IH, LOW); // turn the IH off
//       Serial.println("ih off");
//       request -> send(200, "text/plain", "Turning Off the IH");
//     });
//     // Send a GET request to <IP>/get?message=<message>
//     server.on("/get", HTTP_GET, [](AsyncWebServerRequest * request) {
//       String message;
//       if (request -> hasParam(PARAM_MESSAGE)) {
//         message = request -> getParam(PARAM_MESSAGE) -> value();
//       } else {
//         message = "No message sent";
//       }
//       request -> send(200, "text/plain", "Hello, GET: " + message);
//     });

//     // Send a POST request to <IP>/post with a form field message set to <message>
//     server.on("/post", HTTP_POST, [](AsyncWebServerRequest * request) {
//       String message;
//       if (request -> hasParam(PARAM_MESSAGE, true)) {
//         message = request -> getParam(PARAM_MESSAGE, true) -> value();
//       } else {
//         message = "No message sent";
//       }
//       request -> send(200, "text/plain", "Hello, POST: " + message);
//     });

//     server.onNotFound(notFound);

//     server.begin();
//   } else {
//     Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
//   }
// }

// void loop() {}
