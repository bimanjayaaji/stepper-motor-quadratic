#include <Arduino.h>

#define pul 9
#define dir 10
#define en 11
#define pulse_per_rot 12800
#define angularSpeed 60 // rpm

long stepperLastT= 0;
int stepperState = LOW;
bool s_state=0;

void motor_setup(){
  pinMode(pul, OUTPUT); 
  pinMode(dir, OUTPUT); 
  pinMode(en, OUTPUT);
  digitalWrite(en, s_state);
}

float getDelayDuration(float rpm) {
  if (rpm == 0) return 0;
  float pulse_per_sec = rpm / 60 * pulse_per_rot;
  return (0.5 / pulse_per_sec * 1000000);
}

void setup(){
  Serial.begin(115200);
  motor_setup();
}

void loop(){
  double dd = getDelayDuration(angularSpeed);
  if ((angularSpeed != 0) && (micros() - stepperLastT >= dd)) {
    digitalWrite(en, s_state);
    digitalWrite(dir, HIGH); digitalWrite(pul, stepperState);
    stepperLastT = micros();
    Serial.println("pulsing");
  }
}