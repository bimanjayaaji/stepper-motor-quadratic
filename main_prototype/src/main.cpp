#include <Arduino.h>

#define pul 3
#define dir 5
#define en 6
#define pulse_per_rot 1600

float angularSpeed = 125;

long stepperLastT= 0;
int stepperState = LOW;
bool s_state=0;
bool next_state=LOW;
double dd;

float e = 2.71828;

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

float getrpm(float x){
  float y = (-(pow(x,2)/0.32));
  float rpm = pow(e,y); 
}

void cont_mv(float rpm){
  if ((rpm != 0) && (micros() - stepperLastT >= dd)) {
    if (next_state == HIGH){
      digitalWrite(en, s_state);
      digitalWrite(dir, HIGH); digitalWrite(pul, HIGH);
      next_state = LOW;
    }
    if (next_state == LOW){
      digitalWrite(en, s_state);
      digitalWrite(dir, HIGH); digitalWrite(pul, LOW);
      next_state = HIGH;
    }
    stepperLastT = micros();
  }
}

void move60(){
  for(int x = 0; x < 267; x++){
    digitalWrite(pul,HIGH);
    delayMicroseconds(500);
    digitalWrite(pul,LOW);
    delayMicroseconds(500);
  }
}

void setup(){
  Serial.begin(115200);
  motor_setup();
  dd = getDelayDuration(angularSpeed);
}

void loop(){
  cont_mv(angularSpeed);
  //move60();
  //delay(2000);
}