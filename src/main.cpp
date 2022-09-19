#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <HTTPClient.h>

String plant = "Panel1";
int delayLoop = 8000;

// Variable Declaration
float temperature1 = 0, pressure1 = 0;
float temperature2 = 0, pressure2 = 0;
float temperature3 = 0, pressure3 = 0;
int response;
float sensor1_warning = 0, sensor1_danger = 0;
float sensor2_warning = 0, sensor2_danger = 0;
float sensor3_warning = 0, sensor3_danger = 0;

// BMP Pin Declaration
#define BMP1_SCK 22  // SCL
#define BMP1_MOSI 21 // SDA
#define BMP1_MISO 19 // SDO
#define BMP1_CS 18   // CSB1
#define BMP2_CS 17   // CSB2
#define BMP3_CS 16   // CSB2

// LED Pin Declaration
#define LED1_RED 32
#define LED1_YELLOW 33
#define LED1_GREEN 25
#define LED2_RED 27
#define LED2_YELLOW 14
#define LED2_GREEN 12
#define LED3_RED 4
#define LED3_YELLOW 0
#define LED3_GREEN 2

// SSID Declaration
const char *ssid = "KP-Unilever";
const char *pass = "smartfren";

// API Declaration
String baseUrl = "http://128.199.87.189/api";
HTTPClient http;
WiFiClient wifi;

// BMP280 Declaration
Adafruit_BMP280 bmp1(BMP1_CS, BMP1_MOSI, BMP1_MISO, BMP1_SCK);
Adafruit_BMP280 bmp2(BMP2_CS, BMP1_MOSI, BMP1_MISO, BMP1_SCK);
Adafruit_BMP280 bmp3(BMP3_CS, BMP1_MOSI, BMP1_MISO, BMP1_SCK);

void ledBlinking()
{
  digitalWrite(LED1_GREEN, LOW);
  digitalWrite(LED1_YELLOW, LOW);
  digitalWrite(LED1_RED, LOW);
  digitalWrite(LED2_GREEN, LOW);
  digitalWrite(LED2_YELLOW, LOW);
  digitalWrite(LED2_RED, LOW);
  digitalWrite(LED3_GREEN, LOW);
  digitalWrite(LED3_YELLOW, LOW);
  digitalWrite(LED3_RED, LOW);
  delay(500);
  digitalWrite(LED1_GREEN, HIGH);
  digitalWrite(LED1_YELLOW, HIGH);
  digitalWrite(LED1_RED, HIGH);
  digitalWrite(LED2_GREEN, HIGH);
  digitalWrite(LED2_YELLOW, HIGH);
  digitalWrite(LED2_RED, HIGH);
  digitalWrite(LED3_GREEN, HIGH);
  digitalWrite(LED3_YELLOW, HIGH);
  digitalWrite(LED3_RED, HIGH);
  delay(500);
  digitalWrite(LED1_GREEN, LOW);
  digitalWrite(LED1_YELLOW, LOW);
  digitalWrite(LED1_RED, LOW);
  digitalWrite(LED2_GREEN, LOW);
  digitalWrite(LED2_YELLOW, LOW);
  digitalWrite(LED2_RED, LOW);
  digitalWrite(LED3_GREEN, LOW);
  digitalWrite(LED3_YELLOW, LOW);
  digitalWrite(LED3_RED, LOW);
  delay(500);
  digitalWrite(LED1_GREEN, HIGH);
  digitalWrite(LED1_YELLOW, HIGH);
  digitalWrite(LED1_RED, HIGH);
  digitalWrite(LED2_GREEN, HIGH);
  digitalWrite(LED2_YELLOW, HIGH);
  digitalWrite(LED2_RED, HIGH);
  digitalWrite(LED3_GREEN, HIGH);
  digitalWrite(LED3_YELLOW, HIGH);
  digitalWrite(LED3_RED, HIGH);
  delay(500);
  digitalWrite(LED1_GREEN, LOW);
  digitalWrite(LED1_YELLOW, LOW);
  digitalWrite(LED1_RED, LOW);
  digitalWrite(LED2_GREEN, LOW);
  digitalWrite(LED2_YELLOW, LOW);
  digitalWrite(LED2_RED, LOW);
  digitalWrite(LED3_GREEN, LOW);
  digitalWrite(LED3_YELLOW, LOW);
  digitalWrite(LED3_RED, LOW);
}

void bmpSetup()
{
  if (!bmp1.begin())
  {
    Serial.println("First BMP280 Error!");
    while (1)
    {
      digitalWrite(LED1_RED, HIGH);
      digitalWrite(LED1_GREEN, LOW);
    }
  }
  else
  {

    digitalWrite(LED1_RED, LOW);
    digitalWrite(LED1_GREEN, HIGH);
  }
  delay(1000);

  if (!bmp2.begin())
  {
    Serial.println("Second BMP280 Error!");
    while (1)
    {
      digitalWrite(LED2_RED, HIGH);
      digitalWrite(LED2_GREEN, LOW);
    }
  }
  else
  {
    digitalWrite(LED2_RED, LOW);
    digitalWrite(LED2_GREEN, HIGH);
  }
  delay(1000);

  if (!bmp3.begin())
  {
    Serial.println("Third BMP280 Error!");
    while (1)
    {
      digitalWrite(LED3_RED, HIGH);
      digitalWrite(LED3_GREEN, LOW);
    }
  }
  else
  {
    digitalWrite(LED3_RED, LOW);
    digitalWrite(LED3_GREEN, HIGH);
  }
  delay(1000);
  Serial.println("BMP280 Setup success!");
  ledBlinking();
}

void ledSetup()
{
  Serial.println("Setting up LED");
  pinMode(LED1_RED, OUTPUT);
  pinMode(LED1_YELLOW, OUTPUT);
  pinMode(LED1_GREEN, OUTPUT);

  pinMode(LED2_RED, OUTPUT);
  pinMode(LED2_YELLOW, OUTPUT);
  pinMode(LED2_GREEN, OUTPUT);

  pinMode(LED3_RED, OUTPUT);
  pinMode(LED3_YELLOW, OUTPUT);
  pinMode(LED3_GREEN, OUTPUT);

  digitalWrite(LED1_RED, HIGH);
  digitalWrite(LED2_RED, HIGH);
  digitalWrite(LED3_RED, HIGH);
  digitalWrite(LED1_YELLOW, HIGH);
  digitalWrite(LED2_YELLOW, HIGH);
  digitalWrite(LED3_YELLOW, HIGH);
  digitalWrite(LED1_GREEN, HIGH);
  digitalWrite(LED2_GREEN, HIGH);
  digitalWrite(LED3_GREEN, HIGH);
  delay(2000);
  Serial.println("LED Setup success!");

  digitalWrite(LED1_RED, LOW);
  digitalWrite(LED2_RED, LOW);
  digitalWrite(LED3_RED, LOW);
  digitalWrite(LED1_YELLOW, LOW);
  digitalWrite(LED2_YELLOW, LOW);
  digitalWrite(LED3_YELLOW, LOW);
  digitalWrite(LED1_GREEN, LOW);
  digitalWrite(LED2_GREEN, LOW);
  digitalWrite(LED3_GREEN, LOW);
}

void wifiConnect()
{
  Serial.print("Connecting to Wifi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.println("\nWifi Connected!");
}

String getPlantResponse()
{
  http.begin(baseUrl + "/" + plant + "/setpoint");
  http.GET();
  String response = http.getString();

  return response;
}

int postHttp(float T1, float P1, float T2, float P2, float T3, float P3)
{
  http.begin(baseUrl + "/" + plant);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> buff;
  String jsonParams;

  buff["temperature1"] = T1;
  buff["pressure1"] = P1;
  buff["temperature2"] = T2;
  buff["pressure2"] = P2;
  buff["temperature3"] = T3;
  buff["pressure3"] = P3;
  serializeJson(buff, jsonParams);

  int code = http.POST(jsonParams);

  return code;
}

void firstBMPReader()
{
  temperature1 = bmp1.readTemperature();
  pressure1 = bmp1.readPressure();

  Serial.println("--------- BMP280 1 ---------");

  Serial.print("Warning : ");
  Serial.print(sensor1_warning);
  Serial.print("  Danger : ");
  Serial.println(sensor1_danger);

  Serial.print("Temperature : ");
  Serial.print(temperature1);
  Serial.print("  Pressure : ");
  Serial.println(pressure1);
}

void secondBMPReader()
{
  temperature2 = bmp2.readTemperature();
  pressure2 = bmp2.readPressure();

  Serial.println("--------- BMP280 2 ---------");

  Serial.print("Warning : ");
  Serial.print(sensor2_warning);
  Serial.print("  Danger : ");
  Serial.println(sensor2_danger);

  Serial.print("Temperature : ");
  Serial.print(temperature2);
  Serial.print("  Pressure : ");
  Serial.println(pressure2);
}

void thirdBMPReader()
{
  temperature3 = bmp3.readTemperature();
  pressure3 = bmp3.readPressure();

  Serial.println("--------- BMP280 3 ---------");

  Serial.print("Warning : ");
  Serial.print(sensor3_warning);
  Serial.print("  Danger : ");
  Serial.println(sensor3_danger);

  Serial.print("Temperature : ");
  Serial.print(temperature3);
  Serial.print("  Pressure : ");
  Serial.println(pressure3);
}

void firstBMPChecker()
{
  Serial.print("First BMP Result : ");
  if (temperature1 < sensor1_warning)
  {
    Serial.println("Safe");
    digitalWrite(LED1_GREEN, HIGH);
    digitalWrite(LED1_YELLOW, LOW);
    digitalWrite(LED1_RED, LOW);
  }
  else if (temperature1 >= sensor1_danger)
  {
    Serial.println("DANGER!!");
    digitalWrite(LED1_GREEN, LOW);
    digitalWrite(LED1_YELLOW, LOW);
    digitalWrite(LED1_RED, HIGH);
  }
  else if (temperature1 >= sensor1_warning && temperature1 < sensor1_danger)
  {
    Serial.println("WARNING");
    digitalWrite(LED1_GREEN, LOW);
    digitalWrite(LED1_YELLOW, HIGH);
    digitalWrite(LED1_RED, LOW);
  }
}

void secondBMPChecker()
{
  Serial.print("Second BMP Result : ");
  if (temperature2 < sensor2_warning)
  {
    Serial.println("Safe");
    digitalWrite(LED2_GREEN, HIGH);
    digitalWrite(LED2_YELLOW, LOW);
    digitalWrite(LED2_RED, LOW);
  }
  else if (temperature2 >= sensor2_danger)
  {
    Serial.println("DANGER!!");
    digitalWrite(LED2_GREEN, LOW);
    digitalWrite(LED2_YELLOW, LOW);
    digitalWrite(LED2_RED, HIGH);
  }
  else if (temperature2 >= sensor2_warning && temperature2 < sensor2_danger)
  {
    Serial.println("WARNING");
    digitalWrite(LED2_GREEN, LOW);
    digitalWrite(LED2_YELLOW, HIGH);
    digitalWrite(LED2_RED, LOW);
  }
}

void thirdBMPChecker()
{
  Serial.print("Third BMP Result : ");
  if (temperature3 < sensor3_warning)
  {
    Serial.println("Safe");
    digitalWrite(LED3_GREEN, HIGH);
    digitalWrite(LED3_YELLOW, LOW);
    digitalWrite(LED3_RED, LOW);
  }
  else if (temperature3 >= sensor3_danger)
  {
    Serial.println("DANGER!!");
    digitalWrite(LED3_GREEN, LOW);
    digitalWrite(LED3_YELLOW, LOW);
    digitalWrite(LED3_RED, HIGH);
  }
  else if (temperature3 >= sensor3_warning && temperature3 < sensor3_danger)
  {
    Serial.println("WARNING");
    digitalWrite(LED3_GREEN, LOW);
    digitalWrite(LED3_YELLOW, HIGH);
    digitalWrite(LED3_RED, LOW);
  }
}

void setup()
{
  Serial.begin(9600);
  wifiConnect();
  ledSetup();
  bmpSetup();
}

void loop()
{
  StaticJsonDocument<1024> buff;

  if (getPlantResponse())
  {
    deserializeJson(buff, getPlantResponse());
    JsonObject obj = buff.as<JsonObject>();

    sensor1_warning = obj[String("data")][0][String("set_point")][String("warning1")];
    sensor1_danger = obj[String("data")][0][String("set_point")][String("danger1")];

    sensor2_warning = obj[String("data")][0][String("set_point")][String("warning2")];
    sensor2_danger = obj[String("data")][0][String("set_point")][String("danger2")];

    sensor3_warning = obj[String("data")][0][String("set_point")][String("warning3")];
    sensor3_danger = obj[String("data")][0][String("set_point")][String("danger3")];
  }

  firstBMPReader();
  secondBMPReader();
  thirdBMPReader();

  response = postHttp(temperature1, pressure1, temperature2, pressure2, temperature3, pressure3);
  Serial.print("Code : ");
  Serial.println(response);
  Serial.println();

  if (response == 201)
  {
    firstBMPChecker();
    secondBMPChecker();
    thirdBMPChecker();
  }
  else
  {
    Serial.println("NO RESULT");
    ledBlinking();
  }

  delay(delayLoop);
}
