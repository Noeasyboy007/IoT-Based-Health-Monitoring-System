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

#define ONE_WIRE_BUS 2  // GPIO2 (D4 on NodeMCU)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temp;
float Fahrenheit = 0;

#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BATTERY_PIN A0    // Analog pin for battery voltage
#define MIN_VOLTAGE 3.0   // Voltage corresponding to 0% battery
#define MAX_VOLTAGE 4.2   // Voltage corresponding to 100% battery

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
  timer.setInterval(3000L, getBatteryPercentage); // Call battery check every 3 seconds
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
  temp = DS18B20.getTempCByIndex(0);        // Celsius
  Fahrenheit = DS18B20.toFahrenheit(temp);  // Fahrenheit
  Serial.println(temp);
  Serial.println(Fahrenheit);
  Blynk.virtualWrite(V3, Fahrenheit);  // Virtual pin V3
  // Blynk.virtualWrite(V4, temp); // Virtual pin V4

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("TEMP:");
  display.print(Fahrenheit);
  display.display();
} 

/***************************************************
 * Read Battery Voltage, Convert to Percentage, and Send to Blynk
 **************************************************/
void getBatteryPercentage() {
  int analogValue = analogRead(BATTERY_PIN);
  float voltage = analogValue * (4.2 / 1023.0);  // Convert ADC value to voltage

  // Convert voltage to battery percentage
  int batteryPercentage = (int)((voltage - MIN_VOLTAGE) * 100 / (MAX_VOLTAGE - MIN_VOLTAGE));
  if (batteryPercentage > 100) batteryPercentage = 100;
  if (batteryPercentage < 0) batteryPercentage = 0;

  Serial.println("Battery Percentage: " + String(batteryPercentage) + "%");
  Blynk.virtualWrite(V5, batteryPercentage);  // Virtual pin V4

  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("BATT:");
  display.print(batteryPercentage);
  display.print("%");
  display.display();
} 
