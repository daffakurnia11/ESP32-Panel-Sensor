#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <HTTPClient.h>

String plant = "Panel1";
int delayLoop = 5000;

// Variable Declaration
float temperature = 0, pressure = 0;
int response, sensor1_id = 0, sensor2_id = 0, sensor3_id = 0;
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
const char *ssid = "Deltakilo";
const char *pass = "1sempak8";

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
  http.begin(baseUrl + "/panel_sensor/" + plant);
  http.GET();
  String response = http.getString();

  return response;
}

int postHttp(int id, float T, float P)
{
  http.begin(baseUrl + "/panel_sensor/temperature");
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> buff;
  String jsonParams;

  buff["sensor_id"] = id;
  buff["temperature"] = T;
  buff["pressure"] = P;
  serializeJson(buff, jsonParams);

  int code = http.POST(jsonParams);

  return code;
}

void firstBMPReader(int temp_id, float temp_warning, float temp_danger)
{
  temperature = bmp1.readTemperature();
  pressure = bmp1.readPressure();

  if (temp_id != 0 || temp_warning != 0 || temp_danger != 0)
  {
    if (sensor1_id != temp_id || sensor1_warning != temp_warning || sensor1_danger != temp_danger)
    {
      sensor1_id = temp_id;
      sensor1_warning = temp_warning;
      sensor1_danger = temp_danger;
    }
    Serial.println("--------- BMP280 1 ---------");

    Serial.print("BMP ID : ");
    Serial.print(sensor1_id);
    Serial.print("  Warning : ");
    Serial.print(sensor1_warning);
    Serial.print("  Danger : ");
    Serial.println(sensor1_danger);

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.print("  Pressure : ");
    Serial.println(pressure);

    response = postHttp(sensor1_id, temperature, pressure);
    Serial.print("Code : ");
    Serial.println(response);
    Serial.println();

    if (response > 0)
    {
      Serial.print("BMP Result : ");
      if (temperature < sensor1_warning)
      {
        Serial.println("Safe");
        digitalWrite(LED1_GREEN, HIGH);
        digitalWrite(LED1_YELLOW, LOW);
        digitalWrite(LED1_RED, LOW);
      }
      else if (temperature >= sensor1_danger)
      {
        Serial.println("DANGER!!");
        digitalWrite(LED1_GREEN, LOW);
        digitalWrite(LED1_YELLOW, LOW);
        digitalWrite(LED1_RED, HIGH);
      }
      else if (temperature >= sensor1_warning && temperature < sensor1_danger)
      {
        Serial.println("WARNING");
        digitalWrite(LED1_GREEN, LOW);
        digitalWrite(LED1_YELLOW, HIGH);
        digitalWrite(LED1_RED, LOW);
      }
      else
      {
        Serial.println("NO RESULT");
        digitalWrite(LED1_GREEN, LOW);
        digitalWrite(LED1_YELLOW, LOW);
        digitalWrite(LED1_RED, HIGH);
      }
    }
    else
    {
      ledBlinking();
    }
  }
}

void secondBMPReader(int temp_id, float temp_warning, float temp_danger)
{
  temperature = bmp2.readTemperature();
  pressure = bmp2.readPressure();

  if (temp_id != 0 || temp_warning != 0 || temp_danger != 0)
  {
    if (sensor2_id != temp_id || sensor2_warning != temp_warning || sensor2_danger != temp_danger)
    {
      sensor2_id = temp_id;
      sensor2_warning = temp_warning;
      sensor2_danger = temp_danger;
    }
    Serial.println("--------- BMP280 2 ---------");

    Serial.print("BMP ID : ");
    Serial.print(sensor2_id);
    Serial.print("  Warning : ");
    Serial.print(sensor2_warning);
    Serial.print("  Danger : ");
    Serial.println(sensor2_danger);

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.print("  Pressure : ");
    Serial.println(pressure);

    response = postHttp(sensor2_id, temperature, pressure);
    Serial.print("Code : ");
    Serial.println(response);
    Serial.println();

    if (response > 0)
    {
      Serial.print("BMP Result : ");
      if (temperature < sensor2_warning)
      {
        Serial.println("Safe");
        digitalWrite(LED2_GREEN, HIGH);
        digitalWrite(LED2_YELLOW, LOW);
        digitalWrite(LED2_RED, LOW);
      }
      else if (temperature >= sensor2_danger)
      {
        Serial.println("DANGER!!");
        digitalWrite(LED2_GREEN, LOW);
        digitalWrite(LED2_YELLOW, LOW);
        digitalWrite(LED2_RED, HIGH);
      }
      else if (temperature >= sensor2_warning && temperature < sensor2_danger)
      {
        Serial.println("WARNING");
        digitalWrite(LED2_GREEN, LOW);
        digitalWrite(LED2_YELLOW, HIGH);
        digitalWrite(LED2_RED, LOW);
      }
      else
      {
        Serial.println("NO RESULT");
        digitalWrite(LED2_GREEN, LOW);
        digitalWrite(LED2_YELLOW, LOW);
        digitalWrite(LED2_RED, HIGH);
      }
    }
    else
    {
      ledBlinking();
    }
  }
}

void thirdBMPReader(int temp_id, float temp_warning, float temp_danger)
{
  temperature = bmp3.readTemperature();
  pressure = bmp3.readPressure();

  if (temp_id != 0 || temp_warning != 0 || temp_danger != 0)
  {
    if (sensor3_id != temp_id || sensor3_warning != temp_warning || sensor3_danger != temp_danger)
    {
      sensor3_id = temp_id;
      sensor3_warning = temp_warning;
      sensor3_danger = temp_danger;
    }
    Serial.println("--------- BMP280 3 ---------");

    Serial.print("BMP ID : ");
    Serial.print(sensor3_id);
    Serial.print("  Warning : ");
    Serial.print(sensor3_warning);
    Serial.print("  Danger : ");
    Serial.println(sensor3_danger);

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.print("  Pressure : ");
    Serial.println(pressure);

    response = postHttp(sensor3_id, temperature, pressure);
    Serial.print("Code : ");
    Serial.println(response);
    Serial.println();

    if (response > 0)
    {
      Serial.print("BMP Result : ");
      if (temperature < sensor3_warning)
      {
        Serial.println("Safe");
        digitalWrite(LED3_GREEN, HIGH);
        digitalWrite(LED3_YELLOW, LOW);
        digitalWrite(LED3_RED, LOW);
      }
      else if (temperature >= sensor3_danger)
      {
        Serial.println("DANGER!!");
        digitalWrite(LED3_GREEN, LOW);
        digitalWrite(LED3_YELLOW, LOW);
        digitalWrite(LED3_RED, HIGH);
      }
      else if (temperature >= sensor3_warning && temperature < sensor3_danger)
      {
        Serial.println("WARNING");
        digitalWrite(LED3_GREEN, LOW);
        digitalWrite(LED3_YELLOW, HIGH);
        digitalWrite(LED3_RED, LOW);
      }
      else
      {
        Serial.println("NO RESULT");
        digitalWrite(LED3_GREEN, LOW);
        digitalWrite(LED3_YELLOW, LOW);
        digitalWrite(LED3_RED, HIGH);
      }
    }
    else
    {
      ledBlinking();
    }
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
  int temp_id = 0;
  float temp_warning = 0, temp_danger = 0;
  StaticJsonDocument<1024> buff;

  if (getPlantResponse())
  {
    deserializeJson(buff, getPlantResponse());
    JsonObject obj = buff.as<JsonObject>();

    temp_id = obj[String("data")][0][String("id")];
    temp_warning = obj[String("data")][0][String("set_point")][String("warning")];
    temp_danger = obj[String("data")][0][String("set_point")][String("danger")];

    firstBMPReader(temp_id, temp_warning, temp_danger);

    temp_id = obj[String("data")][1][String("id")];
    temp_warning = obj[String("data")][1][String("set_point")][String("warning")];
    temp_danger = obj[String("data")][1][String("set_point")][String("danger")];

    secondBMPReader(temp_id, temp_warning, temp_danger);

    temp_id = obj[String("data")][2][String("id")];
    temp_warning = obj[String("data")][2][String("set_point")][String("warning")];
    temp_danger = obj[String("data")][2][String("set_point")][String("danger")];

    thirdBMPReader(temp_id, temp_warning, temp_danger);

    delay(delayLoop);
  }
}
