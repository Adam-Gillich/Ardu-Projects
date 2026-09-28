#include <Arduino.h>

// Szervo vezerles potenciometerrel (valaszthato feladat)
//  * a potenciometer allasa szerint all be a szervo karja,
//  * az LCD-n az alapallapothoz (kozepallas) viszonyitott szog latszik,
//  * nyomogombbal megfordul a potenciometer -> szervo irany.
//
// Bekotes (Arduino MEGA 2560):
//  * potenciometer: szelso labak 5V es GND, csuszka A0
//  * szervo: barna/fekete GND, piros 5V, narancs/sarga jel a 9-es labra
//  * nyomogomb: 7-es lab es GND kozott (belso felhuzo ellenallas,
//    a 10 kOhm-os kulso felhuzoval is mukodik)
//  * LCD i2c backpack: 5V, GND, DAT -> SDA (20), CLK -> SCL (21)

#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include <Servo.h>

const int pinPOTM = A0;             // potenciometer csuszkaja
const int pinSZERVO = 9;            // szervo jelvezeteke
const int pinGOMB = 7;              // iranyvalto nyomogomb

const int ALAP = 90;                // alapallapot: kozepallas fokban
const unsigned long PERGES = 50;    // gomb pergesmentesitesi ideje ms

Adafruit_LiquidCrystal lcd(0);      // i2c, 0-s cim (A0-A2 nincs athidalva)
Servo szervo;

bool forditott = false;             // megforditott iranyu mukodes
int gombElozo = HIGH;               // gomb elozo (elfogadott) allapota
unsigned long gombIdo = 0;          // utolso elfogadott gombvaltas ideje
int szogElozo = -1;                 // utoljara beallitott szog

int olvasPot() {                    // 8 minta atlaga a zaj ellen
  long osszeg = 0;
  for (int i = 0; i < 8; i++) {
    osszeg += analogRead(pinPOTM);
  }
  return osszeg / 8;
}

void kiir(int szog) {               // szog es irany kiirasa az LCD-re
  int elteres = szog - ALAP;        // alapallapothoz viszonyitott szog
  lcd.setCursor(0, 0);
  lcd.print("Szog: ");
  if (elteres > 0) {
    lcd.print('+');
  }
  lcd.print(elteres);
  lcd.print((char)223);             // fok jel az LCD karakterkeszleteben
  lcd.print("     ");               // elozo, hosszabb szam maradekanak torlese
  lcd.setCursor(0, 1);
  lcd.print(forditott ? "Irany: forditott" : "Irany: normal   ");
}

void setup() {
  pinMode(pinGOMB, INPUT_PULLUP);   // lenyomva LOW, elengedve HIGH
  szervo.attach(pinSZERVO);
  lcd.begin(16, 2);                 // LCD 16 karakter mindket sorban
  lcd.setBacklight(HIGH);
}

void loop() {
  int gomb = digitalRead(pinGOMB);
  if (gomb != gombElozo && millis() - gombIdo > PERGES) {
    gombIdo = millis();
    gombElozo = gomb;
    if (gomb == LOW) {              // lenyomas pillanataban fordulunk
      forditott = !forditott;
      szogElozo = -1;               // kiiras frissitesenek kikenyszeritese
    }
  }

  int szog = map(olvasPot(), 0, 1023, 0, 180);  // 0..1023 -> 0..180 fok
  if (forditott) {
    szog = 180 - szog;
  }

  if (szog != szogElozo) {          // csak valtozaskor irunk, nem villog
    szervo.write(szog);
    kiir(szog);
    szogElozo = szog;
  }
  delay(15);                        // a szervonak ido kell a mozgashoz
}
