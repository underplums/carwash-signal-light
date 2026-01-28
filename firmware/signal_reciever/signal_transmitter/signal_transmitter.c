#include <IRremote.h>


#define CODE_RED    0xFF6897
#define CODE_GREEN  0xFF30CF

const int IR_LED_PIN = 3;
const int LDR_PIN = A0;

const int DARK_THRESHOLD = 500;
const int LIGHT_THRESHOLD = 300;

IRsend irsend;
bool lastState = false;
unsigned long lastSendTime = 0;
const int SEND_DELAY = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  Serial.println("Signal Light System Started.");
  Serial.print("Dark Threshold: "); Serial.println(DARK_THRESHOLD);
  Serial.print("Light Threshold: "); Serial.println(LIGHT_THRESHOLD);
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  unsigned long currentTime = millis();
  
  Serial.print("Light Level: ");
  Serial.print(lightLevel);
  

  if (lightLevel > DARK_THRESHOLD && !lastState) {

    lastState = true;
    Serial.println(" -> DARK (sending RED command)");
    sendIRCode(CODE_RED);
    
  } else if (lightLevel < LIGHT_THRESHOLD && lastState) {

    lastState = false;
    Serial.println(" -> LIGHT (sending GREEN command)");
    sendIRCode(CODE_GREEN);
    
  } else {

    Serial.println(lastState ? " -> Status: BUSY" : " -> Status: FREE");
  }
  
  delay(500);
}

void sendIRCode(unsigned long code) {
  unsigned long currentTime = millis();
  
  if (currentTime - lastSendTime > SEND_DELAY) {
    irsend.sendNEC(code, 32);
    Serial.print("Sent IR Code: 0x");
    Serial.println(code, HEX);
    lastSendTime = currentTime;
  } else {
    Serial.println("Too frequent transmission.");
  }
}
