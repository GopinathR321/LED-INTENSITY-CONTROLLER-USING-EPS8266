#define BLYNK_TEMPLATE_ID "TMPL39-wmt_f4"
#define BLYNK_TEMPLATE_NAME "Light Control"
#define BLYNK_AUTH_TOKEN "CaraTi0Y3eNrMIZh92jwJF2f1mzdpga1"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// WIFI credentials (replace with your SSID and password)
char ssid [] = "SSID”; // Replace with your WIFI SSID
char pass [] = "PASSWORD"; // Replace with your WIFI Password
// Pin definitions
int pwmPin = D2; // Pin connected to LED/MOSFET (PWM)
int ldrPin = A0; // Pin connected to LDR (Analog)
// Variables
int ldrValue = 0; // Stores the LDR reading
int dutyCycle = 0; // Stores PWM duty cycle value
bool manualControl = false; // Flag for manual or auto mode (false = auto, true = manual)
int manualBrightness = 0; // Stores brightness from Blynk slider
bool wifiConnected = false; // Flag to track WIFI connection
// Blynk virtual pins
#define V1 1 // Slider for manual brightness control
#define V2 2 // Button for switching between auto and manual mode
// Slider widget to manually control brightness
BLYNK_WRITE(V1) {
if (manualControl) {// Only adjust brightness in manual mode
manualBrightness = param.asInt(); // Get value from Blynk slider
analogWrite(pwmPin, manualBrightness);
}
46
}
// Button widget to toggle between manual and auto modes
BLYNK_WRITE(V2) {
manualControl = param.asInt(); // Get the state of the button (0 = auto, 1 = manual)
if (manualControl) {
Serial.println("Manual mode activated");
} else {
Serial.println("Automatic mode activated");
}
}
void controlByLDR() {
// Adjust brightness based on LDR reading in auto mode
ldrValue = analogRead(ldrPin);
Serial.print("LDR Value: ");
Serial.println(ldrValue);
// Map LDR value to PWM duty cycle
dutyCycle = map(ldrValue, 0, 1023, 1023, 0);
// Set the brightness based on LDR
analogWrite(pwmPin, dutyCycle);
}
void setup() {
// Set up serial monitor and pins
Serial.begin(9600);
pinMode(pwmPin, OUTPUT);
pinMode(ldrPin, INPUT);
// Immediately start controlling light based on LDR
Serial.println("Starting in automatic mode (LDR control)...");
controlByLDR(); // Control light right after startup
// Start non-blocking WIFI connection
47
WiFi.begin(ssid, pass);
Blynk.config(BLYNK_AUTH_TOKEN); // Configure Blynk, but don't block for
connection
}
void loop() {
// Non-blocking WIFI connection check
if (WiFi.status() == WL_CONNECTED) {
if (!wifiConnected) {
// If WiFi was previously disconnected and now connected
wifiConnected = true;
Serial.println("WiFi connected. Switching to user control.");
Blynk.connect(); // Attempt to connect Blynk if WiFi is connected
}
Blynk.run(); // Blynk only runs if WiFi is connected
} else {
if (wifiConnected) {
// If WiFi was previously connected and now disconnected, switch to auto mode
wifiConnected = false;
Serial.println("WiFi disconnected. Switching to automatic mode.");
}
}
// Default control using LDR, whether WiFi is connected or not
if (!manualControl || !wifiConnected) {
controlByLDR(); // Control by LDR in automatic mode or when WiFi is not available
} else if (manualControl) {
analogWrite(pwmPin, manualBrightness); // In manual mode, brightness is controlled by
the Blynk slider
}
// Add a small delay to reduce the rate of readingsdelay(100);
}
