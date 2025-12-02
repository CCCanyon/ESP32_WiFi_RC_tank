/*
by Chang, Chia-Chi, 2025

Made to work on a seeed studio XIAO ESP32-C3

Libraries:
Async TCP by ESP32Async: https://github.com/ESP32Async/AsyncTCP
ESP Async WebServer by ESP32Async: https://github.com/ESP32Async/ESPAsyncWebServer
ESP32Servo by Kevin Harrington,John K.Bennett: https://madhephaestus.github.io/ESP32Servo/annotated.html

*/

#include <EEPROM.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <stdio.h>
#include <DNSServer.h>
#include <ESP32Servo.h>
#include "web_page.h"
#include "web_captive_portal.h"
#include "driver/ledc.h"


#define version F(__FILE__ " " __DATE__ " " __TIME__)

#define bootPin 0    // button

// button
uint32_t boot_timer = 0;
bool boot_state = 0;   // press: LOW, release: HIGH
bool _boot_state = 0;  // press: LOW, release: HIGH

bool blink = 0;
uint32_t blink_timer = 0;
int LED_ = 10;

// control
int32_t drive_timer = 0;
const int32_t drive_looptime = 8000;
int32_t drive_failsafe_timer = 0;
const int32_t drive_failsafe_timeout = 2000000;
const int drive_freq = 20000; // example high freq
const int drive_resolution = 10;  // 65536
int motor_max;
const int EN = 3; // D1 -> EEP
const int AIN1 = 4; // D2 -> IN1
const int AIN2 = 5; // D3 -> IN2
const int BIN1 = 6; // D4 -> IN3
const int BIN2 = 7; // D5 -> IN4
const int MAX_SPEED = 128;   // match your joystick range
int speed_L = 0;
int speed_R = 0;

//const float pwm_scalar = 13.1072; // 16383 example duty cycle for 1.25ms pulse. 65536 = 5ms

const int turret_traverse_pin = 9; // D9
const int turret_elevation_pin = 10; // D10
Servo traverse_servo, elevation_servo;
int turret_T = 0;
int turret_E = 0;
int turret_T_pwm = 0;
int turret_E_pwm = 0;


// serial
String str;

// timer
uint32_t then = 0;
uint32_t dt = 0;
bool web_dbug = 0;

// wifi
const byte DNS_PORT = 53;
DNSServer dnsServer;
AsyncWebServer webServer(80);  // Web server on port 80
AsyncWebSocket ws("/ws");      // WebSocket endpoint at /ws
#define BUFFER_MAX_LEN 255
char buffer[BUFFER_MAX_LEN + 1];
int buffer_l;

bool wifi_AP_restarting = 0;
uint32_t wifi_AP_restart_timer = 0;
const uint32_t wifi_AP_restart_timeout = 1000000;

// Mode
uint32_t wifiState_timestamp = 0;
uint32_t wifiState_wifi_begin_timeout = 15000000;
uint32_t wifiState_server_timestamp = 0;
enum WiFi_State
{
  // Initialise Wi-Fi for STA
  STATE_WIFI_INITIALISE,
  // Idle and wait for Wi-Fi reconnection
  STATE_WIFI_CONNECTING,
  // Connected to hotspot (station mode), initialise web server
  STATE_SERVER_INITIALISE,
  // Idle and monitor connection
  STATE_SERVING,
  // Pause and reconnect
  STATE_WIFI_DISCONNECTED
};
WiFi_State wifiState = STATE_WIFI_INITIALISE;

struct ConfigData
{
  // AP mode, ESP32's
  byte ip_AP[4];     // IP address (IPv4)
  char ssid_AP[33];  // SSID (WiFi network name)
  char pass_AP[65];  // WiFi password
  // STA mode, of device ESP32 connects to
  byte ip_STA[4];     // IP address (IPv4)
  char ssid_STA[33];  // SSID (WiFi network name)
  char pass_STA[65];  // WiFi password
  // control settings
  int deadzone;
  int traverse_centre;
  int traverse_limit;
  int elevation_max;
  int elevation_min;
};

ConfigData defaultConfig = {
  // AP mode, ESP32's
  { 192, 168, 1, 1 },  // IP address
  "RC01",            // SSID
  "",          // WiFi password
  // STA mode, of device ESP32 connects to
  { 0, 0, 0, 0 },  // IP address
  "Arduino:D",          // SSID
  "99999999",        // IP address
  // control settings
  400,
  1500,
  -200,
  2200,
  1300
};

// Initialize config
ConfigData config;

// EEPROM
#define eepromAddress 0
byte configInitializedFlag = 0;

// Captive Portal
void sendCaptivePortal(AsyncWebServerRequest *request);
class CaptiveRequestHandler : public AsyncWebHandler
{
  public:
  bool canHandle(__unused AsyncWebServerRequest *request) const override
  {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    sendCaptivePortal(request);
  }
};

void setup()
{
  
  if (!EEPROM.begin(256))
  {
    Serial.println("EEPROM failed to initialise");
    while (true)
      ;
  }
  // Read the initialization flag from EEPROM
  EEPROM.get(sizeof(ConfigData), configInitializedFlag);
  // Check if the config data has been initialized
  if (configInitializedFlag != 1)
  {
    // Write the initialization flag to EEPROM
    EEPROM.put(sizeof(ConfigData), 1);
    // Factory reset
    configReset();
  }
  else
  {
    // Initialise config
    configRead(config);
    Serial.println("Config data already present in EEPROM");
  }

  motor_max = (1 << drive_resolution) - 1;
  pinMode(EN, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  ledcAttachChannel(AIN1, drive_freq, drive_resolution, 2);
  ledcAttachChannel(AIN2, drive_freq, drive_resolution, 3);
  ledcAttachChannel(BIN1, drive_freq, drive_resolution, 4);
  ledcAttachChannel(BIN2, drive_freq, drive_resolution, 5);

  // small delay to let DRV8833 inputs settle
  delay(10);
  Serial.begin(115200);          // initialize serial communication
  pinMode(LED_, OUTPUT);  // set the LED pin mode
  pinMode(bootPin, INPUT);       // set the arm pin mode

  // Motor PWM setup
  setMotor(AIN1, AIN2, 0);
  setMotor(BIN1, BIN2, 0);

  ESP32PWM::allocateTimer(0); // Use timer 0 for servo
  traverse_servo.setPeriodHertz(50); // Standard 50Hz servo
  elevation_servo.setPeriodHertz(50); // Standard 50Hz servo
  traverse_servo.attach(turret_traverse_pin);
  elevation_servo.attach(turret_elevation_pin);
  traverse_servo.writeMicroseconds(config.traverse_centre);
  elevation_servo.writeMicroseconds((config.elevation_max + config.elevation_min) * 0.5);

  str.reserve(100);  // Allocate 100 bytes upfront

  WiFi.mode(WIFI_AP_STA);
  WIFI_START_AP();

  ws.onEvent(onWebSocketEvent);
  webServer.addHandler(&ws);

  // UI
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", PAGE_HOME);
  });

  // captive portal
  webServer.on("/ap-update", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    Serial.println("captive receive");
    if (request->hasParam("network", true))
    {
      request->getParam("network", true)->value().toCharArray(config.ssid_STA, 32);
    }
    if (request->hasParam("password", true))
    {
      request->getParam("password", true)->value().toCharArray(config.pass_STA, 32);
    }
    WIFI_RESTART_STA();
    //request->redirect("/");
    sendCaptivePortal(request);
  });
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);  // only when requested from AP

  webServer.begin();
  
  digitalWrite(EN, HIGH);
}
/*
structure:
#Time
#Serial UI
#State Switch
*/
void loop()
{
  // Time looprate 500Hz ==================================================================
  while (micros() - then < 2000) yield();
  dt = micros() - then;
  then += dt;

  // Serial UI ==================================================================
  if (Serial.available() > 0)
  {
    str = "";
    str = Serial.readStringUntil('\n');
    if (str.equals("web"))
    {
      web_dbug = !web_dbug;
      Serial.print("\nweb activity display ");
      if (web_dbug) { Serial.println("ON"); }
      else { Serial.println("OFF"); }
    }
    else if (str.equals("config"))
    {
      configPrint(config);
    }
    else if (str.equals("config reset"))
    {
      configReset();
    }
    else if (str.equals("wifi"))
    {
      Serial.print("\nWiFi Connection State: ");
      Serial.println(wifiState);
      printWifiStatus();
    }
    else if (str.equals("help"))
    {
      Serial.println("\n= help =\nwifi = print WiFi status\nstate = print machine state\nconfig = print config data\nconfig reset = reset config data\nweb = toggle web server activity display\np = show control\n0-3 = control\n========\n");
    }
    else
    {
      Serial.println("Try 'help'");
    }
  }

  // State Switch ==================================================================
  switch (wifiState)
  {
    // Initialise Wi-Fi for STA connection
    case STATE_WIFI_INITIALISE:
      // STA
      WiFi.begin(config.ssid_STA, config.pass_STA);
      wifiState_timestamp = micros();
      wifiState = STATE_WIFI_CONNECTING;
      break;
    // Idle and wait for Wi-Fi reconnection
    case STATE_WIFI_CONNECTING:
      if (WiFi.status() == WL_CONNECTED)
      {
        // connected, transition to SERVER_INITIALISE_STA
        wifiState = STATE_SERVER_INITIALISE;
        wifiState_timestamp = then;
      }
      else if (then - wifiState_timestamp > wifiState_wifi_begin_timeout)
      {
        // timeout, retry
        wifiState = STATE_WIFI_DISCONNECTED;
        wifiState_timestamp = then;
      }
      break;
    // Connected to hotspot, initialise web server
    case STATE_SERVER_INITIALISE:
      if (then - wifiState_timestamp < 250000) break;

      config.ip_STA[0] = WiFi.localIP()[0];
      config.ip_STA[1] = WiFi.localIP()[1];
      config.ip_STA[2] = WiFi.localIP()[2];
      config.ip_STA[3] = WiFi.localIP()[3];
      WiFi.config(WiFi.localIP(), WiFi.localIP(), IPAddress(255, 255, 0, 0));

      wifiState = STATE_SERVING;
      wifiState_timestamp = then;
      break;
    // Server running
    case STATE_SERVING:
      // monitor connection
      if (then - wifiState_timestamp < 250000) break;
      if ((WiFi.status() != WL_CONNECTED) || (WiFi.localIP()[0] == 0))
      {
        WIFI_RESTART_STA();
        break;
      }
      break;
    // Restart STA connection
    case STATE_WIFI_DISCONNECTED:
      if (then - wifiState_timestamp < 5000000) break;
      wifiState = STATE_WIFI_INITIALISE;
      break;

    default:
      break;
  }

  
  // websocket heartbeat ==================================================================
  wifiState_server_timestamp += dt;
  if (wifiState_server_timestamp >= 1000000)
  {
    while (wifiState_server_timestamp >= 1000000)
    {
      wifiState_server_timestamp -= 1000000;
    }
    //Serial.print("HB");
    webSocketHeartbeat();
  }

  dnsServer.processNextRequest();

  // AP restart timing ==================================================================
  if (wifi_AP_restarting)
  {
    if (wifi_AP_restart_timer < wifi_AP_restart_timeout)
    {
      wifi_AP_restart_timer += dt;
    }
    else
    {
      WIFI_START_AP();
      wifi_AP_restarting = 0;
      wifi_AP_restart_timer = 0;
    }
  }

  // button ==================================================================

  // blink
  if (blink_timer < 500000)  // when blink timer resets
  {
    blink_timer += 2000;
    digitalWrite(LED_, LOW);  // light off
  }
  // LED
  else if (wifiState == STATE_SERVING)
  {
    digitalWrite(LED_, HIGH);
  }
  else
  {
    digitalWrite(LED_, LOW);
  }

  // control ==================================================================
  drive_failsafe_timer +=dt;
  drive_timer += dt;
  if (drive_timer >= drive_looptime)
  {
    while(drive_timer > drive_looptime)
    {
      drive_timer -= drive_looptime;
    }
    if(wifiState == STATE_SERVING && drive_failsafe_timer < drive_failsafe_timeout)
    {
      setMotor(AIN1, AIN2, speed_L);
      setMotor(BIN1, BIN2, speed_R);
      
    }
    else
    {
      setMotor(AIN1, AIN2, 0);
      setMotor(BIN1, BIN2, 0);
    }
  }

}

// AP
void WIFI_START_AP()
{
  //WiFi.softAP(config.ssid_AP, config.pass_AP);
  WiFi.softAP(config.ssid_AP, config.pass_AP, 6, 0, 4); // channel 6 is somehow faster to show up

  IPAddress ap_ip(config.ip_AP[0], config.ip_AP[1], config.ip_AP[2], config.ip_AP[3]);
  IPAddress gateway = ap_ip;
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(ap_ip, gateway, subnet);
  
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}
void WIFI_RESTART_AP()
{
  dnsServer.stop();
  WiFi.softAPdisconnect(true);  // fully stop current AP
  wifi_AP_restarting = 1;       // restart flag, loop calls WIFI_START_AP() 5 seconds later
}
// captive portal page
void sendCaptivePortal(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream("text/html");

  response->print(PAGE_REDIRECT_0);

  if (wifiState == STATE_SERVING)
  {
    response->print(PAGE_REDIRECT_1_CONNECTED_0);
    response->print(config.ip_STA[0]);
    response->print('.');
    response->print(config.ip_STA[1]);
    response->print('.');
    response->print(config.ip_STA[2]);
    response->print('.');
    response->print(config.ip_STA[3]);
    response->print(PAGE_REDIRECT_1_CONNECTED_1);
  }
  else
  {
    response->print(PAGE_REDIRECT_1_NOT_CONNECTED);
  }
  response->print(PAGE_REDIRECT_2);
  response->print(config.ssid_STA);
  response->print(PAGE_REDIRECT_3);
  request->send(response);
}
// reconnect now
void WIFI_RESTART_STA()
{
  WiFi.disconnect(true, false, 2);
  wifiState = STATE_WIFI_DISCONNECTED;
  //WiFi.mode(WIFI_OFF);
  //WIFI_START_AP();
  wifiState_timestamp = then;
}
// state update
void webSocketHeartbeat()
{
  //snprintf(buffer, BUFFER_MAX_LEN, "time;%d", play_time);
  ws.textAll("HB");
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("WebSocket client #%u connected\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
  }
  else if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
      buffer_l = (len < BUFFER_MAX_LEN) ? len : BUFFER_MAX_LEN;
      memcpy(buffer, data, buffer_l);
      buffer[buffer_l] = '\0';  // Null-terminate to make it a C-string

      //Serial.printf("[WS] Message received: %s\n", buffer);

      char *sep = strchr(buffer, ';');  // pointer to the separator
      if (sep)                          // in separator not Null
      {
        *sep = '\0';              // change the seperator into Null
        char *tag = buffer;       // so tag = buffer terminated by the separator
        char *payload = sep + 1;  // and payload is after the separator
        
        if (strcmp(tag, "drv") == 0)
        {
          //Serial.println("drv!");
          int L, R;
          if (sscanf(payload, "%d,%d", &L, &R) == 2)
          {
            speed_L = constrain(L, -MAX_SPEED, MAX_SPEED);
            speed_R = constrain(R, -MAX_SPEED, MAX_SPEED);
            drive_failsafe_timer = 0;
          }
        }
        else if (strcmp(tag, "tur") == 0)
        {
          int T, E;
          if (sscanf(payload, "%d,%d", &T, &E) == 2)
          {
            turret_T = map(T, -128, 128, config.traverse_centre - config.traverse_limit, config.traverse_centre + config.traverse_limit);

            turret_E = map(E, -128, 128, config.elevation_min, config.elevation_max);
            
            traverse_servo.writeMicroseconds(turret_T);
            elevation_servo.writeMicroseconds(turret_E);
            //Serial.printf("T=%d,E=%d,duty_T=%d,duty_E=%d\n", T, E, turret_T, turret_E);
          }
        }
        else if (strcmp(tag, "settings-reset") == 0)
        {
          configReset();
        }
        else if (strcmp(tag, "settings") == 0)
        {
          // READ
          if (*payload == '\0')
          {
            snprintf(buffer, BUFFER_MAX_LEN,
                     "settings;ip_AP=%hhu.%hhu.%hhu.%hhu;ssid_AP=%s;pass_AP=%s;"
                     "ip_STA=%hhu.%hhu.%hhu.%hhu;ssid_STA=%s;pass_STA=%d;deadzone=%d;traverse_centre=%d;traverse_limit=%d;elevation_max=%d;elevation_min=%d",
                     config.ip_AP[0], config.ip_AP[1], config.ip_AP[2], config.ip_AP[3], config.ssid_AP, config.pass_AP,
                     config.ip_STA[0], config.ip_STA[1], config.ip_STA[2], config.ip_STA[3], config.ssid_STA, strlen(config.pass_STA), config.deadzone,
                     config.traverse_centre, config.traverse_limit, config.elevation_max, config.elevation_min);
            Serial.println("send settings");
            ws.textAll(buffer);
            // report all config values - settings;v0=v0;v1=v1...
          }
          // WRITE
          // AP settings
          else
          {
            if (sscanf(payload, "ip_AP=%hhu.%hhu.%hhu.%hhu",
                          &config.ip_AP[0], &config.ip_AP[1], &config.ip_AP[2], &config.ip_AP[3]))
              ;
            else if (sscanf(payload, "ssid_AP=%s", &config.ssid_AP))
              ;
            else if (sscanf(payload, "pass_AP=%s", &config.pass_AP))
              ;
            // STA settings
            else if (sscanf(payload, "ip_STA=%hhu.%hhu.%hhu.%hhu",
                            &config.ip_STA[0], &config.ip_STA[1], &config.ip_STA[2], &config.ip_STA[3]))
              ;
            else if (sscanf(payload, "ssid_STA=%s", &config.ssid_STA))
              ;
            else if (sscanf(payload, "pass_STA=%s", &config.pass_STA))
              ;
            else if (sscanf(payload, "deadzone=%d", &config.deadzone))
              ;
            else if (sscanf(payload, "traverse_centre=%d", &config.traverse_centre))
              ;
            else if (sscanf(payload, "traverse_limit=%d", &config.traverse_limit))
              ;
            else if (sscanf(payload, "elevation_max=%d", &config.elevation_max))
              ;
            else if (sscanf(payload, "elevation_min=%d", &config.elevation_min))
              ;
            // control settings
            configWrite(config);
          }
        }
        else if (strcmp(tag, "ap_restart") == 0)
        {
          WIFI_RESTART_AP();
        }
        else if (strcmp(tag, "sta_restart") == 0)
        {
          WIFI_RESTART_STA();
        }
        else if (strcmp(tag, "info") == 0)
        {
          snprintf(buffer, BUFFER_MAX_LEN, "info;%s", version);
          ws.textAll(buffer);
        }
        else if (strcmp(tag, "blink") == 0)
        {
          blink = 1;
        }
        else
        {
          Serial.println("No responce.\n");
          //
        }
      }
    }
  }
}


/*
ESP32 WiFi statuses
0 |	WL_IDLE_STATUS	temporary status assigned when WiFi.begin() is called
1 |	WL_NO_SSID_AVAIL	 when no SSID are available
2 |	WL_SCAN_COMPLETED	scan networks is completed
3 |	WL_CONNECTED	when connected to a WiFi network
4 |	WL_CONNECT_FAILED	when the connection fails for all the attempts
5 |	WL_CONNECTION_LOST	when the connection is lost
6 |	WL_DISCONNECTED	when disconnected from a network
*/
void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("\nWiFi status: ");
  Serial.println(WiFi.status());
  // print IP addresses:
  IPAddress ip = WiFi.localIP();
  Serial.print("AP mode\n| IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("| SSID: ");
  Serial.println(config.ssid_AP);
  Serial.print("| PASSWORD: ");
  Serial.println(config.pass_AP);
  Serial.print("| Connected Devices: ");
  Serial.println(WiFi.softAPgetStationNum());
  Serial.print("STA mode\n| IP: ");
  Serial.println(ip);
  Serial.print("| SSID: ");
  Serial.println(config.ssid_STA);
  Serial.print("| PASSWORD: ");
  Serial.println(config.pass_STA);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("| signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// =====================================================
// Helper: set motor speed (signed value -128..128)
// =====================================================
void setMotor(int pin1, int pin2, int speed)
{
  speed = constrain(speed, -128, 128);
  int duty = speed == 0 ? 0 : map(abs(speed), 0, 128, config.deadzone, motor_max);

  if (speed > 0)
  {
    ledcWrite(pin1, duty);
    ledcWrite(pin2, 0);
  }
  else if (speed < 0)
  {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, duty);
  }
  else
  {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, 0);
  }
}

// config
// EEPROM
void configWrite(const ConfigData &configData)
{
  EEPROM.put(eepromAddress, configData);
  EEPROM.commit();
}
void configRead(ConfigData &configData)
{
  EEPROM.get(eepromAddress, configData);
}
// reset factory config
void configReset()
{
  configWrite(defaultConfig);
  configRead(config);
  Serial.println("Default config data written to EEPROM");
  configPrint(config);
}
// print
void configPrint(const ConfigData &config)
{
  Serial.println("\nConfig:");
  // AP info
  Serial.print("AP mode:\n| IP: ");
  for (int i = 0; i < 4; ++i)
  {
    Serial.print(config.ip_AP[i]);
    if (i < 3) { Serial.print("."); }
  }
  Serial.print("\n| SSID: ");
  Serial.print(config.ssid_AP);
  Serial.print("\n| PASSWORD: ");
  Serial.print(config.pass_AP);
  // STA info
  Serial.print("\nSTA mode:\n| IP: ");
  for (int i = 0; i < 4; ++i)
  {
    Serial.print(config.ip_STA[i]);
    if (i < 3) { Serial.print("."); }
  }
  Serial.print("\n| SSID: ");
  Serial.print(config.ssid_STA);
  Serial.print("\n| PASSWORD: ");
  Serial.print(config.pass_STA);
  
  // control settings

  Serial.println("\n\n");
}

// tools