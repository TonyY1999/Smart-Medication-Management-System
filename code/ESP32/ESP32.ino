#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL2vTA-vAiT"
#define BLYNK_TEMPLATE_NAME "ESE519 Final" 
#define BLYNK_AUTH_TOKEN "H9o__ZDFtiWxtnDLemmTycKIvsmVtoYb"

// WiFi credentials 
#define MY_SSID "AirPennNet-Device"
#define MY_PASSWORD "penn1740wifi"

// Output pins definition
#define OUTPUT_PIN1 18
#define OUTPUT_PIN2 14
#define OUTPUT_PIN3 12

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Auth token for Blynk
char auth[] = BLYNK_AUTH_TOKEN;

// State variables to track switch status
bool switchState1 = false;
bool switchState2 = false;
bool switchState3 = false;

// First switch handler (V0)
BLYNK_WRITE(V0) {
 // Toggle state
 switchState1 = !switchState1;
 // Update output pin
 digitalWrite(OUTPUT_PIN1, switchState1);
 // Update app UI
 Blynk.virtualWrite(V0, switchState1);
 
 Serial.print("Switch 1 state: ");
 Serial.println(switchState1);
}

// Second switch handler (V1)
BLYNK_WRITE(V1) {
 switchState2 = !switchState2;
 digitalWrite(OUTPUT_PIN2, switchState2);
 Blynk.virtualWrite(V1, switchState2);
 
 Serial.print("Switch 2 state: ");
 Serial.println(switchState2);
}

// Third switch handler (V2) 
BLYNK_WRITE(V2) {
 switchState3 = !switchState3;
 digitalWrite(OUTPUT_PIN3, switchState3);
 Blynk.virtualWrite(V2, switchState3);
 
 Serial.print("Switch 3 state: ");
 Serial.println(switchState3);
}

void setup() {
 // Initialize serial communication
 Serial.begin(115200);
 delay(100);
 Serial.println("\nStarting...");
 
 // Initialize pinmode for outputs
 pinMode(OUTPUT_PIN1, OUTPUT);
 pinMode(OUTPUT_PIN2, OUTPUT);
 pinMode(OUTPUT_PIN3, OUTPUT);
 
 // Set default state as low
 digitalWrite(OUTPUT_PIN1, LOW);
 digitalWrite(OUTPUT_PIN2, LOW);
 digitalWrite(OUTPUT_PIN3, LOW);
 
 Serial.println("All output pins initialized to LOW");
 
 // Connect to Blynk
 Serial.println("Connecting to Blynk...");
 Blynk.begin(auth, MY_SSID, MY_PASSWORD);
 Serial.println("Connected to Blynk!");
}

void loop() {
 // Run Blynk processing
 Blynk.run();
 
 // Print switch states every 5 seconds
 static unsigned long lastPrint = 0;
 if (millis() - lastPrint > 5000) {
   Serial.println("====================");
   Serial.print("Switch 1 state: ");
   Serial.println(switchState1);
   Serial.print("Switch 2 state: ");
   Serial.println(switchState2);
   Serial.print("Switch 3 state: ");
   Serial.println(switchState3);
   Serial.println("====================");
   lastPrint = millis();
 }
}