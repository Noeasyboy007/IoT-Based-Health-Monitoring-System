#define BLYNK_TEMPLATE_ID "TMPL39f48clrN"
#define BLYNK_TEMPLATE_NAME "CAREIFY"
#define BLYNK_AUTH_TOKEN "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"
#include <BlynkSimpleEsp8266.h>
 
char auth[] = "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv";
/* WiFi credentials */
char ssid[] = "aritra";
char pass[] = "12345678";
 
#define REPORTING_PERIOD_MS     2000
SimpleTimer timer;
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
    
}
 

 // for the OLED display
 #define SCREEN_WIDTH 128 // OLED display width, in pixels
 #define SCREEN_HEIGHT 64 // OLED display height, in pixels

 // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
 #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
 {
    Serial.begin(115200);
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(1);
    display.setCursor(0,25);
    display.print("HEALTH BIT");
    display.display();

    Blynk.begin(auth, ssid, pass);
    Serial.print("Initializing pulse oximeter..");

  
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin())
     {
        Serial.println("FAILED");

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1);
        display.setCursor(0, 0);
        display.println("Please Connect Your Network....");
        display.display();
        for(;;);
      }

     else
     {
        Serial.println("SUCCESS");

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(1);
        display.setCursor(0,0);
        display.println("DEVICE IS OK");
        display.display();
        digitalWrite(1,HIGH);
     }

       pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);
 
       // Register a callback for the beat detection
       pox.setOnBeatDetectedCallback(onBeatDetected);
 
       timer.setInterval(2000L, getSendData);
 
       display.clearDisplay();
       display.setTextColor(WHITE);
  }
 
      void loop()
     {
        timer.run(); // Initiates SimpleTimer
        Blynk.run();
        // Make sure to call update as fast as possible
        pox.update();
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
       {
         // to android cell phone application
         Serial.print("BPM: ");
         Serial.print(pox.getHeartRate());
         //blue.println("\n");
        
         Serial.print("    SpO2: ");
         Serial.print(pox.getSpO2());
         Serial.print("%"); 
         Serial.println("\n");
 
         Blynk.virtualWrite(V1,pox.getHeartRate() );
         Blynk.virtualWrite(V2,pox.getSpO2());
 
 
         tsLastReport = millis();    
        }
      }
 
    void getSendData()
     {
       // Oled display
       display.clearDisplay();
       // display R G B Values
//////////////////////////////////////////////////////////////////

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(1);
    display.setCursor(5,0);
    display.print("Beat.....");
    
//////////////////////////////////////////////////////////////////////

       display.setTextSize(2);
       display.setCursor(0,25); // column row
       display.print("BPM:");
 
       display.setTextSize(2);
       display.setCursor(55, 25);
       display.print(pox.getHeartRate());
 
/////////////////////////////////////////////////////////////////////

       display.setTextSize(2);
       display.setCursor(0,50);
       display.print("SpO2:");
 
       display.setTextSize(2);
       display.setCursor(65,50);
       display.print(pox.getSpO2());

       display.setCursor(95,50);
       display.print("%");

////////////////////////////////////////////////////////////////////

      //  display.setTextSize(2);
      //  display.setCursor(0,50);
      //  display.print("TEMP:");

      //  display.print("");
       display.display(); 
     }