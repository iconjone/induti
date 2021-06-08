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



//###Setup Access Point###
void setupAccessPoint() {
setUpVariables();
  static char szSSID[12];
  sprintf(szSSID, "Induti %02d", ESP.getChipId() % 100);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
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




void setup(){
      Serial.begin(115200);
  while (!Serial);
  delay(200);
    setupAccessPoint();
}

void loop(){
    //if it's config mode
dnsServer.processNextRequest();
}