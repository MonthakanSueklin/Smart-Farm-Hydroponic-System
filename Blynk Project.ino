#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#define NUM_LEDS1 74
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#include <BH1750FVI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#define SEALEVELPRESSURE_HPA (1013.25)
unsigned long delayTime;
BlynkTimer timer;
Adafruit_BME280 bme; 
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes); // สร้างออปเจกเซนเซอร์ BH1750
CRGB leds1[NUM_LEDS1];
char auth[] = "na5adDdzU5IMUYuIiBSH0rbEQDlWts62";
char ssid[] = "DiwPeerawich";
char pass[] = "keiichi666";
#define PIN1 D8
int data=255;
int r,g,b;
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, millis() / 1000);
  Blynk.virtualWrite(V4, millis() / 1000);
  Blynk.virtualWrite(V5, millis() / 1000);
  Blynk.virtualWrite(V6, millis() / 1000);
}
void setup()
{
  Serial.begin(9600);                               
  Blynk.begin(auth, ssid, pass);
  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection( TypicalLEDStrip );
  Serial.println(F("BME280 test"));
  Serial.println("ArduinoAll TEST BH1750");
    bool status;
    status = bme.begin(0x76);
      if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
      }
  Serial.println("-- Default Test --");
  delayTime = 1000;
  Serial.println();
LightSensor.begin(); // สั่งให้เซนเซอร์ เริ่มทำางน
timer.setInterval(1000L, printValues);
pinMode(D3, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา D0 เป็น OUTPUT
pinMode(D4, OUTPUT);
}

BLYNK_WRITE(V1)
{
  r = param[0].asInt();
  g = param[1].asInt();
  b = param[2].asInt();
                            
  static1(r, g, b,data);
}

void loop()
{
  Blynk.run();
  timer.run();
      if (bme.readTemperature() > 27){
      digitalWrite(D3,LOW);
      digitalWrite(D4,LOW);
   }

      if (bme.readTemperature() < 27){
      digitalWrite(D3,HIGH);
      digitalWrite(D4,HIGH);
   }
}
BLYNK_WRITE(V2)
{
data = param.asInt(); 
static1(r, g, b,data);
}
void static1(int r, int g, int b,int brightness)
{
  FastLED.setBrightness(brightness);
  for (int i = 0; i < NUM_LEDS1; i++ )
  {
    leds1[i] = CRGB(r, g, b);
  }
  FastLED.show();
}
void printValues()
{
  uint16_t lux = LightSensor.GetLightIntensity();  
  if (isnan (lux))
  {
    Serial.println(F("failed to read from BH1750")); 
    Serial.println(F("BME280 test"));
    return;
  }
  Serial.print(F("Light : "));
  Serial.print(lux);
  Serial.print(F("Temperature = "));
  Serial.print(bme.readTemperature());
  Serial.print(F("Humidity = "));
  Serial.print(bme.readHumidity());
  Serial.print(F("Pressure = "));
  Serial.print(bme.readPressure() / 100.0F);
  
  Blynk.virtualWrite(V3, lux);
  Blynk.virtualWrite(V4, bme.readTemperature());
  Blynk.virtualWrite(V5, bme.readHumidity());
  Blynk.virtualWrite(V6, bme.readPressure() / 100.0F);

  delay(100);
}
