#include <Arduino.h>

#define pul 3
#define dir 5
#define en 6
#define pulse_per_rot 1600

bool direction = LOW;
float angularSpeed = 60;

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
  float y = (-(pow(x-1,2)/0.32));
  float rpm = pow(e,y); 
  return rpm*60;
}

void cont_mv(float rpm){
  if ((rpm != 0) && (micros() - stepperLastT >= dd)) {
    if (next_state == HIGH){
      digitalWrite(en, s_state);
      digitalWrite(dir, direction); digitalWrite(pul, HIGH);
      next_state = LOW;
    }
    if (next_state == LOW){
      digitalWrite(en, s_state);
      digitalWrite(dir, direction); digitalWrite(pul, LOW);
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

void gauss_loop(){
  float k = 0, l = 0; 
  while (k < 267){
    if (l < 2){
      float rpm = getrpm(l);
      Serial.println(rpm);
      dd = getDelayDuration(rpm);
      cont_mv(rpm);
      l += 0.007;
    }
    k += 1;
  }
}

void setup(){
  Serial.begin(115200);
  motor_setup();
}

void loop(){
  //Serial.println("loop");
  //dd = getDelayDuration(angularSpeed);
  //cont_mv(angularSpeed);
  //move60();
  gauss_loop();
  delay(1000);
}