```cpp
/*
 * DIY Automated Brewing System
 * Hardware: Arduino Nano, DS18B20 Temp Probe, SSR (Solid State Relay), 12V Pump Relay
 */

#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
#define ONE_WIRE_BUS 2      // Temp sensor data pin
#define HEATER_SSR 3        // PWM/Digital pin for Heater
#define PUMP_RELAY 4        // Digital pin for Pump
#define BUZZER 5            // Piezo buzzer

// Setup Temperature Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Brewing States
enum BrewState { IDLE, MASHING, BOILING, COOLING };
BrewState currentState = IDLE;

// Targets (Modify these as needed)
float targetTemp = 65.0;     // Typical Mash Temp
unsigned long mashTime = 60; // Minutes
unsigned long startTime;

void setup() {
  pinMode(HEATER_SSR, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  Serial.begin(9600);
  sensors.begin();
  
  Serial.println("System Ready. Send 'S' to Start Mash.");
}

void loop() {
  sensors.requestTemperatures();
  float currentTemp = sensors.getTempCByIndex(0);

  // Serial Monitor feedback
  Serial.print("Current Temp: "); Serial.print(currentTemp);
  Serial.print(" | State: "); Serial.println(currentState);

  switch (currentState) {
    case IDLE:
      digitalWrite(HEATER_SSR, LOW);
      digitalWrite(PUMP_RELAY, LOW);
      if (Serial.read() == 'S') { 
        currentState = MASHING; 
        startTime = millis();
      }
      break;

    case MASHING:
      maintainTemp(currentTemp, targetTemp);
      digitalWrite(PUMP_RELAY, HIGH); // Keep wort moving
      
      // Check if time is up
      if ((millis() - startTime) / 60000 >= mashTime) {
        currentState = BOILING;
        beep(3); // Alert user
      }
      break;

    case BOILING:
      // In boil, we just run the heater at high duty cycle (e.g., 90%)
      digitalWrite(HEATER_SSR, HIGH); 
      digitalWrite(PUMP_RELAY, LOW); // Usually pump is off during boil
      break;
      
    case COOLING:
      digitalWrite(HEATER_SSR, LOW);
      digitalWrite(PUMP_RELAY, HIGH); // Use pump to circulate through chiller
      break;
  }
  
  delay(1000); // 1-second updates are plenty for brewing
}

// Simple "On-Off" logic with a 1-degree buffer (Hysteresis)
// For professional results, swap this for a PID library
void maintainTemp(float current, float target) {
  if (current < target - 0.5) {
    digitalWrite(HEATER_SSR, HIGH);
  } else if (current > target + 0.5) {
    digitalWrite(HEATER_SSR, LOW);
  }
}

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

```
