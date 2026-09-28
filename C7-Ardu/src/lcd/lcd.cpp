#include <Arduino.h>

// meghajto vezerlesehez szukseges konyvtar
// adafruit liquid crystal
/*
 Demonstration sketch for Adafruit i2c/SPI LCD backpack
 using MCP23008 I2C expander
 ( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )

 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5
 * DAT to Analog #4
*/

// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

const int switchPin = 6;        // higyankapcsolo laba
int switchState = 0;            // kapcsolo allapota
int prevSwitchState = 0;        // kapcsolo elozo allapota
int reply;                      // tarolo

void setup() {
  lcd.begin(16, 2);             // LCD 16 karakter mindket sorban
  pinMode(switchPin, INPUT);    // higanykapcsolo bemenetkent
  lcd.print("Alapallapot. Ova");// elso uzenet
  lcd.setCursor(0, 1);
  lcd.print("tosan razz meg..");
}

void loop() {
  switchState = digitalRead(switchPin);   // kapcsolo allapota
  if (switchState != prevSwitchState) {   // atmenet
    if (switchState == LOW) {
      lcd.setBacklight(HIGH);             // háttér világos
      reply = random(8);                  // sorsoljunk szamot
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorsoltam:");
      lcd.setCursor(0, 1);
      switch (reply) {                    // sorsolas fuggo uzenet az LCD-re
        case 0:
        lcd.print("nulla");
        break;
        case 7:
        lcd.print("nemkocka");
        break;
        default:
        lcd.print(reply);
        break;
      }
    } else {
      lcd.setBacklight(LOW);              // háttér sötét
    }
  }
  prevSwitchState = switchState;          // kapcsolo allapotanak mentese
}
