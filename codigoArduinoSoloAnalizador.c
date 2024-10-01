#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED 2
#define pinBuzzer 3
#define Sensor A1

void setup() {
 lcd.init();
 lcd.backlight();
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 pinMode(pinBuzzer, OUTPUT);
}

void loop() {
 lcd.display();
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Sensor de gases");
 int value = analogRead(Sensor);
 lcd.setCursor(0, 1);
 lcd.print("Val:");
 lcd.setCursor(4, 1);
 lcd.print(value);
 delay (200);

 if (value > 50) {
  digitalWrite(LED, HIGH);
  lcd.setCursor(8, 1);
  lcd.print("GAS Det");
  tone(pinBuzzer, 700); // PIN DEL ZUMBADOR, FREQUENCIA.
  delay (150); // TIEMPO PARA MANTENER EL TONO.
  tone(pinBuzzer, 294);
  delay (50);
  tone(pinBuzzer, 494);
  delay (250);
 } else {
  digitalWrite(LED, LOW);
  noTone(pinBuzzer);
  lcd.setCursor(8, 1);
  lcd.print("No GAS");
  delay (450);
 }
}
