#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <OneButton.h>

#include "html.h"

IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
AsyncWebServer httpServer(80);

String _ssid = "";
String _password = "";

String wifiList = "";
bool bootUp = false;
bool waitingWifi = false;

bool restartCountdown = false;
unsigned long checkMillis = 0;

int LED = 16;
int IH = 5;
int BUTTON = 4;
int RED_LED = 14;
int GREEN_LED = 12;
int BLUE_LED = 13;

//###Button stuff###
OneButton btn = OneButton(
    BUTTON, // Input pin for the button
    true,   // Button is active LOW
    true    // Enable internal pull-up resistor
);
unsigned long pressStartTime;
ICACHE_RAM_ATTR void checkTicks()
{
  // include all buttons here to be checked
  btn.tick(); // just call tick() to check the state.
}
// this function will be called when the button was pressed 1 time only.
void singleClick()
{
  Serial.println("singleClick() detected.");
} // singleClick

// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleClick()
{
  Serial.println("doubleClick() detected.");
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  //setUpVariables();
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("Induti");
  delay(100);
} // doubleClick

// this function will be called when the button was pressed multiple times in a short timeframe.
void multiClick()
{
  Serial.print("multiClick(");
  Serial.print(btn.getNumberClicks());
  Serial.println(") detected.");
  if (btn.getNumberClicks() == 3)
  {
    WiFi.disconnect(true);
    WiFi.persistent(false);
    WiFi.setAutoReconnect(false);
    ESP.restart();
  }
} // multiClick

// this function will be called when the button was held down for 1 second or more.
void pressStart()
{
  Serial.println("pressStart()");
  pressStartTime = millis() - 1000; // as set in setPressTicks()
  analogWrite(IH, 512);             // turn the IH on
} // pressStart()

// this function will be called when the button was released after a long hold.
void pressStop()
{
  Serial.print("pressStop(");
  Serial.print(millis() - pressStartTime);
  Serial.println(") detected.");
  analogWrite(IH, LOW); // turn the IH off
} // pressStop()

//###Setup Config Portal###
bool configMode = false;
void setupConfigPortal()
{

  setUpVariables();
  static char szSSID[12];
  sprintf(szSSID, "Induti %02d", ESP.getChipId() % 100);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //WIFI.wifi_sta_config_t config = {0};
  if ((WiFi.getMode() & WIFI_STA) != 0)
  {
    ETS_UART_INTR_DISABLE(); // @todo probably not needed
    wifi_station_disconnect();
    ETS_UART_INTR_ENABLE();

  } // disconnect before begin, in case anything is hung, this causes a 2 seconds delay for connect
  WiFi.mode(WIFI_STA);

  WiFi.disconnect();

  Serial.println(WiFi.getMode());
  delay(100);
  // Make sure the requested SSID is not being used
  bool exists = true;
  while (exists)
  {
    int n = WiFi.scanNetworks();
    Serial.print("Scanning Wifi Networks: ");
    Serial.println(n);
    exists = false;
    for (int i = 0; i < n; i++)
    {
      String ssid = WiFi.SSID(i);
      Serial.print("Found SSID ");
      Serial.println(ssid);
      wifiList += ssid;
      wifiList += "|"; //delimter

      if (strcmp(szSSID, ssid.c_str()) == 0)
        exists = true;
    }
    if (exists || n == -1 || n == 0)
    {
      delay(5000); // Wait before scanning again
      exists = true;
    }
  }
  //might need to wifi.disconnect
  // Set up the open AP with the given SSID
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(szSSID);
  delay(100);
  setupDNS();
  setupWebServer();
}
void setUpVariables()
{
  IPAddress apIP(192, 168, 4, 1);
  IPAddress netMsk(255, 255, 255, 0);
  DNSServer dnsServer;
  AsyncWebServer httpServer(80);
}
void setupDNS()
{
  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  configMode = true;
}

//## Access Point Server ##
PGM_P config_html = config_html_str;
void setupWebServer()
{

  httpServer.on("/config", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  Serial.println("Config Page Requested");
                  request->send_P(200, "text/html", config_html); //_p or send
                });
  httpServer.on("/config", HTTP_POST, [](AsyncWebServerRequest *request)
                {
                  Serial.println("Recieved Config Page Paramters");
                  _ssid = request->arg("ssid").c_str();
                  _password = request->arg("password").c_str();
                  Serial.println(_ssid + " | " + _password);
                  request->send(200, "text/plain", "Connecting to SSID"); //_p or send
                  configMode = false;
                  WiFi.softAPdisconnect();
                  WiFi.disconnect();
                  connectWifi(_ssid, _password);
                  Serial.println("Initiated WiFi Connection Process...");
                });
  httpServer.on("/wifiList", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  Serial.println("WifiList Requested");
                  request->send(200, "text/plain", wifiList); //_p or send
                });
  httpServer.onNotFound(handleNotFound);

  httpServer.begin();
  Serial.println("AP Server Begun.");
}
void handleNotFound(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
  response->addHeader("Location", String("http://") + toStringIp(request->client()->localIP()) + String("/config"));
  request->send(response);
}
String toStringIp(IPAddress ip)
{
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }

  res += String(((ip >> 8 * 3)) & 0xFF);

  return res;
}
String WiFi_SSID(bool persistent)
{

  struct station_config conf;
  if (persistent)
    wifi_station_get_config_default(&conf);
  else
    wifi_station_get_config(&conf);

  char tmp[33]; //ssid can be up to 32chars, => plus null term
  memcpy(tmp, conf.ssid, sizeof(conf.ssid));
  tmp[32] = 0; //nullterm in case of 32 char ssid
  return String(reinterpret_cast<char *>(tmp));
}

String WiFi_psk(bool persistent)
{

  struct station_config conf;

  if (persistent)
    wifi_station_get_config_default(&conf);
  else
    wifi_station_get_config(&conf);

  char tmp[65]; //psk is 64 bytes hex => plus null term
  memcpy(tmp, conf.password, sizeof(conf.password));
  tmp[64] = 0; //null term in case of 64 byte psk
  return String(reinterpret_cast<char *>(tmp));
}

bool connectWifi(String ssid, String pass)
{

  Serial.println("Attempting Connection");
  if ((WiFi.getMode() & WIFI_STA) != 0)
  {
    ETS_UART_INTR_DISABLE(); // @todo probably not needed
    wifi_station_disconnect();
    ETS_UART_INTR_ENABLE();

  } // disconnect before begin, in case anything is hung, this causes a 2 seconds delay for connect
  // @todo find out what status is when this is needed, can we detect it and handle it, say in between states or idle_status

  // if ssid argument provided connect to that
  if (ssid != "")
  {

    WiFiMode_t newMode;
    WiFiMode_t currentMode = WiFi.getMode();
    newMode = (WiFiMode_t)(currentMode | WIFI_STA);

    ETS_UART_INTR_DISABLE();
    wifi_set_opmode(newMode);

    ETS_UART_INTR_ENABLE();

    WiFi.persistent(true);
    WiFi.config(INADDR_ANY, INADDR_ANY, INADDR_ANY);
    WiFi.hostname("induti");
    WiFi.begin(ssid.c_str(), pass.c_str());
    waitingWifi = true;
  }
  else
  {
    // connect using saved ssid if there is one
    if (WiFi_SSID(true) != "")
    {

      WiFiMode_t newMode;
      WiFiMode_t currentMode = WiFi.getMode();
      newMode = (WiFiMode_t)(currentMode | WIFI_STA);

      ETS_UART_INTR_DISABLE();
      wifi_set_opmode(newMode);

      ETS_UART_INTR_ENABLE();
      WiFi.config(INADDR_ANY, INADDR_ANY, INADDR_ANY);
      WiFi.hostname("induti");
      WiFi.begin();
      waitingWifi = true;
    }
  }

  return true;
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

bool setUpControl = false;
PGM_P control_html = control_html_str;
void setUpInductionHeater()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());

    httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                    digitalWrite(LED, LOW); // turn the LED on
                    Serial.println("Index");
                    request->send_P(200, "text/html", control_html);
                  });
    httpServer.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                    digitalWrite(LED, LOW); // turn the LED on
                    analogWrite(IH, 512);   // turn the IH on
                    Serial.println("IH ON");
                    request->send(200, "text/plain", "Turning On the IH");
                  });
    httpServer.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                    digitalWrite(LED, HIGH); // turn the LED off
                    digitalWrite(IH, LOW);   // turn the IH off
                    Serial.println("IH OFF");
                    request->send(200, "text/plain", "Turning Off the IH");
                  });
  httpServer.on("/color", HTTP_POST, [](AsyncWebServerRequest *request)
                {
                  String valueStr = request->arg("intValue").c_str();
                  Serial.println(valueStr);
                  request->send(200, "text/plain", "Changing Color"); //_p or send
                  lightControl(valueStr.toInt());
                  //function testColor(hex){fetch("http://192.168.1.16/color?intValue="+parseInt(hex, 16).toString(),{method:"POST"})}
                  });
    httpServer.on("/resetWifi", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                    digitalWrite(LED, HIGH); // turn the LED off
                    digitalWrite(IH, LOW);   // turn the IH off
                    Serial.println("Reset Wifi");
                    request->send(200, "text/plain", "Resetting Wifi");
                    delay(500);
                    WiFi.disconnect();
                    ESP.restart();
                  });

    httpServer.onNotFound(notFound);

    httpServer.begin();

    setUpControl = true;
  }
  delay(3000);
}
int connectionAttempt = 0;
bool handleWaitingConnection()
{
  if (WiFi.status() == 7) // == 7
  {
    Serial.println("Still Waiting");
    lightControl(0xDB4402);
    delay(500);
    lightControl(0x000000);
    delay(500);
    //   wifiConnectionAttempts++;
  }
  else
  {
    waitingWifi = false;
    Serial.println(WiFi.status());
    if (WiFi.status() == 3)
    {
      Serial.println("Done Conecting");
      connectionAttempt = 0;
    lightControl(0x00FF00);

    }
    else
    {
      connectionAttempt++;
      Serial.println("Something messed up");
      if (WiFi.status() == 1 && connectionAttempt < 5)
      {
        Serial.println("Attempting Connection Again");
        connectWifi(_ssid, _password);
      }
      if (!bootUp)
      {

        setupConfigPortal();
      }
      else
      {

        delay(500);

        checkMillis = millis();
        restartCountdown = true;
      }
    }
    delay(1000);
  }
  return true;
}

//Hex Value & brightness
void lightControl(int hex)
{
  analogWrite(RED_LED, (hex >> 16) );
  analogWrite(GREEN_LED, ((hex >> 8) & 0xFF) );
  analogWrite(BLUE_LED, (hex & 0xFF) );
}

void blinkLight(int hex)
{
  lightControl(hex);
  delay(500);
  lightControl(0x000000);
  delay(500);
  lightControl(hex);
  delay(500);
  lightControl(0x000000);
  delay(500);
}
void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
    // analogWriteRange(1023);
    // analogWriteFreq(2500);
  delay(200);
  pinMode(LED, OUTPUT);
  pinMode(IH, OUTPUT); //IH

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON), checkTicks, CHANGE);
  btn.attachClick(singleClick);
  btn.attachDoubleClick(doubleClick);
  btn.attachMultiClick(multiClick);
  btn.setPressTicks(1000); // that is the time when LongPressStart is called
  btn.attachLongPressStart(pressStart);
  btn.attachLongPressStop(pressStop);

  Serial.setDebugOutput(true);
  Serial.println();
  configMode = false;
  Serial.println(WiFi_SSID(true));
  _ssid = WiFi_SSID(true);
  Serial.println(WiFi_psk(true));
  _password = WiFi_psk(true);
  if (WiFi_SSID(true) != "")
  {
    Serial.println("Connecting from Saved WiFi");
    bootUp = true;
    connectWifi(_ssid, _password);
  }
  else
  {
    Serial.println("Starting Config Portal");
    setupConfigPortal();
  }
}
void loop()
{
  btn.tick();
  delay(500);
  //if it's config mode
  if (configMode)
  {
    dnsServer.processNextRequest();
  }
  if (!configMode && waitingWifi)
  {
    handleWaitingConnection();
  }
  if (!setUpControl && !configMode && !waitingWifi)
  {
    setUpInductionHeater();
  }

  if (restartCountdown)
  {
    if (millis() - checkMillis >= 3 * 60 * 1000UL)
    {
      //WiFi.disconnect(); // Don't want to disconnect bc it will delete wifi information
      ESP.restart();
    }
  }
}
