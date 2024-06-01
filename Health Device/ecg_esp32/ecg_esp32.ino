#define BLYNK_TEMPLATE_ID "TMPL39f48clrN"
#define BLYNK_TEMPLATE_NAME "CAREIFY"
#define BLYNK_AUTH_TOKEN "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;
char auth[] = "htwEcjVohIDGpCdGqFDWVFPRx72V8LLv";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "aritra";
char pass[] = "12345678";

const int heartPin = A0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5, sendUptime);

}

void sendUptime()
{
  
  int heartValue = analogRead(heartPin);
  Serial.println(heartValue);
  delay(1);
  Blynk.virtualWrite(V0,heartValue);

  
}

void loop() {

  Blynk.run();
  timer.run();
}