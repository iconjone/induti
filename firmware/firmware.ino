#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "config_html.h"


IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
AsyncWebServer httpServer(80);
String wifiList = "";
bool configMode = false;

int LED = 16;
int IH = 5;

//###Setup Access Point###
void setupAccessPoint() {
setUpVariables();
  static char szSSID[12];
  sprintf(szSSID, "Induti %02d", ESP.getChipId() % 100);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //WIFI.wifi_sta_config_t config = {0};
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // Make sure the requested SSID is not being used
  bool exists = true;
  while(exists) {
    int n = WiFi.scanNetworks();
    exists = false;
    for (int i=0; i<n; i++) {
      String ssid = WiFi.SSID(i);
      Serial.print("Found SSID ");
      Serial.println(ssid);
      wifiList += ssid;
      wifiList += "|"; //delimter

      if(strcmp(szSSID, ssid.c_str())==0)
        exists = true;
      }
    if(exists) {
      delay(5000); // Wait before scanning again
      }
    }

  // Set up the open AP with the given SSID
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(szSSID);
  delay(100);
  setupDNS();
  setupWebServer(true);
  }
void setUpVariables(){
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
AsyncWebServer httpServer(80);

}
void setupDNS() {
  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  
  }

//## Access Point Server ##
PGM_P s = config_html_str;
void setupWebServer(bool withForm) {
Serial.println("Lets see if run");
  //httpServer.on("/config", handleConfig);
    httpServer.on("/config", HTTP_GET, [](AsyncWebServerRequest * request) {
      Serial.println("oooh");
      request -> send_P(200, "text/html", s); //_p or send
    });
        httpServer.on("/config", HTTP_POST, [](AsyncWebServerRequest * request) {
      Serial.println("oooh i GOT IT yuh");
     String _ssid = request->arg("ssid").c_str();
     String _password = request->arg("password").c_str();
    Serial.println(_ssid + " | " + _password);
     request -> send(200, "text/plain", "WOW GOOD JOB"); //_p or send
     configMode = false;
  WiFi.softAPdisconnect();
WiFi.disconnect();

             Serial.print("f1");
    //  delay(5000);
             Serial.print("f2");


             Serial.print("f3");

//     dnsServer.stop();
             Serial.print("f4");

    // delay(500);
  //   httpServer.reset();
             Serial.print("f5");
     //delay(500);
    // delay(500);
connectWifi(_ssid,_password);
         


//while (WiFi.status() != WL_CONNECTED);
      Serial.print("f10");
  Serial.println();

  Serial.print("Connected, IP address: ");
  //Serial.println(WiFi.localIP());

  //delay(120000);


    });
        httpServer.on("/wifiList", HTTP_GET, [](AsyncWebServerRequest * request) {
      Serial.println("wifiList");
      request -> send(200, "text/plain", wifiList); //_p or send
    });
    httpServer.onNotFound(handleNotFound);
//   if(withForm) {
//     httpServer.onNotFound(handleDefault);
//     }
//   else
//     httpServer.onNotFound(handleNotFound);
  httpServer.begin();
  Serial.println("Lets see if run2");
  }
void handleNotFound(AsyncWebServerRequest *request) {
    //  request -> send(200, "text/html", s); //_p or send
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



  //verify connectrivity



//html to string  var string = document.documentElement.innerHTML;
//www.nousphere.net/cleanspecial.php Cleaning up html
//https://ajusa.github.io/lit/docs/lit.html?

// typedef enum {
//     WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//     WL_IDLE_STATUS      = 0,
//     WL_NO_SSID_AVAIL    = 1,
//     WL_SCAN_COMPLETED   = 2,
//     WL_CONNECTED        = 3,
//     WL_CONNECT_FAILED   = 4,
//     WL_CONNECTION_LOST  = 5,
//     WL_WRONG_PASSWORD   = 6,
//     WL_DISCONNECTED     = 7
// } wl_status_t;
bool waitingWifi = false;
bool waitForConnectResult(uint16_t timeout) {

waitingWifi = true;
  unsigned long timeoutmillis = millis() + timeout;
Serial.println("Waiting For connect..");

  Serial.println(WiFi.status());
  Serial.println(timeoutmillis);
  Serial.println(millis());
  // while(WiFi.status() != 3 || WiFi.status() != 4){
  //   Serial.println("Still Waiting");
  //   delayMicroseconds(1000000);
  //   yield();
  // }
  // while(millis() < timeoutmillis) {
  //    delay(1000);
  //   // @todo detect additional states, connect happens, then dhcp then get ip, there is some delay here, make sure not to timeout if waiting on IP
  //   // if (WiFi.status() == 3 || WiFi.status() == 4) { //3= connected and 4 = connection failed
  //   //   //return WiFi.status();
  //   //   return true;
  //   // }
  //   Serial.print(". ");
  //     Serial.print(WiFi.status());
  //         Serial.print(" | ");
  //         Serial.println(WiFi.status(),BIN);
   
  // }
  Serial.println("THIS IS THE STATUS");
  Serial.println(WiFi.status());
  //return WiFi.status();
  return true;
}

String WiFi_SSID(bool persistent) {

    struct station_config conf;
    if(persistent) wifi_station_get_config_default(&conf);
    else wifi_station_get_config(&conf);

    char tmp[33]; //ssid can be up to 32chars, => plus null term
    memcpy(tmp, conf.ssid, sizeof(conf.ssid));
    tmp[32] = 0; //nullterm in case of 32 char ssid
    return String(reinterpret_cast<char*>(tmp));

}

bool connectWifi(String ssid, String pass) {
  Serial.println("Attempting Connection");
  uint8_t connRes = (uint8_t)WL_NO_SSID_AVAIL;


  
  // make sure sta is on before `begin` so it does not call enablesta->mode while persistent is ON ( which would save WM AP state to eeprom !)
  Serial.println(connRes);
  Serial.print("should be fine");
  WiFi.persistent(false);
      if((WiFi.getMode() & WIFI_STA) != 0) {
Serial.print("fail here?");
          ETS_UART_INTR_DISABLE(); // @todo probably not needed
          wifi_station_disconnect();
          ETS_UART_INTR_ENABLE();        

      }// disconnect before begin, in case anything is hung, this causes a 2 seconds delay for connect
  // @todo find out what status is when this is needed, can we detect it and handle it, say in between states or idle_status

  // if ssid argument provided connect to that
      unsigned long _saveTimeout   = 1200000; 
  if (ssid != "") {


WiFiMode_t newMode;
      WiFiMode_t currentMode = WiFi.getMode();
newMode     = (WiFiMode_t)(currentMode | WIFI_STA);

      ETS_UART_INTR_DISABLE();
 wifi_set_opmode(newMode);

      ETS_UART_INTR_ENABLE();

  WiFi.persistent(true);
       WiFi.hostname("Induti");
  WiFi.begin(ssid.c_str(), pass.c_str());
  WiFi.persistent(false);
Serial.println("connecting?");

     connRes = waitForConnectResult(_saveTimeout); // use default save timeout for saves to prevent bugs in esp->waitforconnectresult loop
  
  }
  else {
    // connect using saved ssid if there is one
    if (WiFi_SSID(true) != "") {

WiFiMode_t newMode;
      WiFiMode_t currentMode = WiFi.getMode();
newMode     = (WiFiMode_t)(currentMode | WIFI_STA);

      ETS_UART_INTR_DISABLE();
 wifi_set_opmode(newMode);

      ETS_UART_INTR_ENABLE();
           WiFi.hostname("Induti");
WiFi.begin();
      connRes = waitForConnectResult(_saveTimeout);
    }

  }

Serial.println("Does this get here?");
//  DEBUG_WM(DEBUG_VERBOSE,F("Connection result:"),getWLStatusString(connRes));

// WPS enabled? https://github.com/esp8266/Arduino/pull/4889


  if(connRes != WL_SCAN_COMPLETED){
        if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD){
         Serial.println('Messed up the password BOZO');
        }
  }

  return connRes;
}



void setup(){
      Serial.begin(115200);
  while (!Serial);
  delay(200);
  Serial.setDebugOutput(true);
  WiFi.persistent(true);
  Serial.println();
  configMode = false;
   setupAccessPoint();
  // WiFi.mode(WIFI_AP);
  // WiFi.begin("TheWall", "137udnC025");

  // Serial.print("Connecting");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println();

  // Serial.print("Connected, IP address: ");
  // Serial.println(WiFi.localIP());
}

void notFound(AsyncWebServerRequest * request) {
  request -> send(404, "text/plain", "Not found");
}

bool setUp = false;
void setUpInductionHeater(){
    if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP());
    Serial.println("Connect");
    httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      digitalWrite(LED, LOW); // turn the LED on
      Serial.println("oooh");
      request -> send(200, "text/plain", "Hello, world");
    });
    httpServer.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
      digitalWrite(LED, LOW); // turn the LED on
      digitalWrite(IH, HIGH); // turn the IH on
      Serial.println("ih on");
      request -> send(200, "text/plain", "Turning On the IH");
    });
    httpServer.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
      digitalWrite(LED, HIGH); // turn the LED off
      digitalWrite(IH, LOW); // turn the IH off
      Serial.println("ih off");
      request -> send(200, "text/plain", "Turning Off the IH");
    });


    httpServer.onNotFound(notFound);

    httpServer.begin();
    setUp = true;
}
delay(3000);
}

void loop(){
    //if it's config mode
if(configMode){
dnsServer.processNextRequest();
}
if(!configMode && waitingWifi){
  if(WiFi.status() == 7){
    Serial.println("Still Waiting");
    delayMicroseconds(1000000);
  }
  else{
    Serial.println("nice it's connected");
      delayMicroseconds(1000000);
      waitingWifi = false;
      
  }
}
if(!setUp && !configMode && !waitingWifi){
  setUpInductionHeater();
}
}