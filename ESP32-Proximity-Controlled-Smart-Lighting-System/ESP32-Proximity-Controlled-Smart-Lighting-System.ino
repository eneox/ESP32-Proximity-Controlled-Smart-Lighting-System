#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int sharpPin = A0;
const int relayPin = 8;
const int buttonPin = 2;

const float thresholdDistance = 20.0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool lampState = false;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Distance:");
}

void loop() {
  int sensorValue = analogRead(sharpPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float distance = 27.86 * (1.0 / (voltage - 0.1));

  lcd.setCursor(8, 0);
  lcd.print("    ");
  lcd.setCursor(8, 0);
  lcd.print(distance);
  lcd.print(" cm");

  if (distance < thresholdDistance) {
    if (!lampState) {
      turnLampOn();
    }
  } else {
    if (lampState) {
      turnLampOff();
    }
  }

  checkButton();
  delay(100);
}

void checkButton() {
  static bool buttonState = HIGH;
  static bool lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        if (lampState) {
          turnLampOff();
        } else {
          turnLampOn();
        }
      }
    }
  }

  lastButtonState = reading;
}

bool isLampOn() {
  return lampState;
}

void turnLampOn() {
  digitalWrite(relayPin, HIGH);
  lampState = true;
}

void turnLampOff() {
  digitalWrite(relayPin, LOW);
  lampState = false;
}
