#define BLYNK_TEMPLATE_ID "TMPL39f48clrN"
#define BLYNK_TEMPLATE_NAME "CAREIFY"
#define BLYNK_AUTH_TOKEN "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BLYNK_PRINT Serial  // Comment this out to disable prints and save space

char auth[] = "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv";
/* WiFi credentials */
char ssid[] = "aritra";
char pass[] = "12345678";

SimpleTimer timer;

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temp;
float Fahrenheit = 0;


#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(0, 25);
  display.print("HEALTH BIT");
  display.display();

  Blynk.begin(auth, ssid, pass);
  DS18B20.begin();
  timer.setInterval(1000L, getSendData);
}

void loop() {
  timer.run();  // Initiates SimpleTimer
  Blynk.run();
}

/***************************************************
 * Send Sensor data to Blynk
 **************************************************/
void getSendData() {
  DS18B20.requestTemperatures();
  temp = DS18B20.getTempCByIndex(0);        // Celcius
  Fahrenheit = DS18B20.toFahrenheit(temp);  // Fahrenheit
  Serial.println(temp);
  Serial.println(Fahrenheit);
  Blynk.virtualWrite(V3, Fahrenheit);  //virtual pin V3
  // Blynk.virtualWrite(V4, temp); //virtual pin V4


  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("TEMP:");
  display.print(Fahrenheit);
  display.display();
}