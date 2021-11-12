#include <Arduino.h>

#define lim_open 9
#define lim_close 8

void setup() {
  Serial.begin(115200);
  pinMode(lim_open,INPUT_PULLUP);
  pinMode(lim_close,INPUT_PULLUP);
}

void loop() {
  Serial.print(digitalRead(lim_open));
  Serial.print("  ");
  Serial.println(digitalRead(lim_close));
  delay(10);
}