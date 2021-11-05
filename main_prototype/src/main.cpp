#include <Arduino.h>

#define pul 3
#define dir 5
#define en 6
#define pulse_per_rot 1600

bool direction = LOW;
float angularSpeed = 60;
float degrees = 60; 

long stepperLastT= 0;
bool en_state=0;
bool next_state=HIGH;
double dd;

float angres = (degrees/360)*pulse_per_rot;
float A = -1.8 ,B = 0 ,C = -1 ,D = 1.8;
float E1 = -4, E2 = 4, F = (E2-E1)/angres;
// notBad1 --> A = -1 ,B = 0, C = -0.8 ,D = 1.2;
//             E1 = -2 ,E2 = 2 ,F = 0.01498 -- T = 0.39
// notBad2 --> A = -1 ,B = 0, C = -1.8 ,D = 1;
//             E1 = -4 ,E2 = 4 ,F = 0.02996 -- T = 0.32s
// notBad3 --> A = -1 ,B = 0, C = -2.5 ,D = 1.3;
//             E1 = -6 ,E2 = 6 ,F = 0.04494 -- T = 0.41s 
// notBad4 --> A = -1.8 ,B = 0, C = -1.5 ,D = 2 or 1.8;
//             E1 = -6 ,E2 = 6 ,F = 0.04494 -- T = 0.59s or 0.54s
// notBad5 --> A = -1.8 ,B = 0, C = -1 ,D = 2;
//             E1 = -4 ,E2 = 4 ,F = 0.02996 -- T = 0.59s 
// notBad6 --> A = -1.8 ,B = 0, C = -1 ,D = 1.8;
//             E1 = -4 ,E2 = 4 ,F = 0.02996 -- T = 0.53s

/*


________________________________________________________
| num | A   | B   | C   | D   | E1  | E2  |   F   | T(s)|
| 1   | -1  | 0   | -0.8| 1.2 | -2  | 2   |0.01498| 0.39|
| 2   |     |     | -1.8| 1   | -4  | 4   |0.02996| 0.32|
| 3   |     |     | -2.5| 1.3 | -6  | 6   |0.04494| 0.41|
| 4   | -1.8|     | -1.5| 2   |     |     |       | 0.59| 
| 5   |     |     |     | 1.8 |     |     |       | 0.54|
| 6   |     |     | -1  | 2   | -4  | 4   |0.02996| 0.59|
| 7   |     |     |     | 1.8 |     |     |       | 0.53|
|_____|_____|_____|_____|_____|_____|_____|_______|_____|

*/

float e = 2.71828;
float multiplier = 500;

void motor_setup(){
  pinMode(pul, OUTPUT); 
  pinMode(dir, OUTPUT); 
  pinMode(en, OUTPUT);
  digitalWrite(en, en_state);
}

float getDelayDuration(float rpm) {
  if (rpm == 0) return 0;
  float pulse_per_sec = rpm / 60 * pulse_per_rot;
  return (0.5 / pulse_per_sec * 1000000);
}

float getrpm(float x){
  float y = (-(pow(x-1,2)/0.32));
  float rpm = 6*pow(e,y); 
  return rpm*60;
}

float getDelay(float x){
  float y = -(pow(x-B,2)/(2*pow(C,2)));
  float ddelay = (A*pow(e,y))+D;
  Serial.println(ddelay); 
  return ddelay*multiplier;
}

void cont_mv(float rpm){
  if ((rpm != 0) && (micros() - stepperLastT >= dd)) {
    if (next_state == HIGH){
      digitalWrite(en, en_state);
      digitalWrite(dir, direction); digitalWrite(pul, HIGH);
      next_state = LOW;
    }
    if (next_state == LOW){
      digitalWrite(en, en_state);
      digitalWrite(dir, direction); digitalWrite(pul, LOW);
      next_state = HIGH;
    }
    stepperLastT = micros();
  }
}

void move60(){
  for(int x = 0; x < 267; x++){
    digitalWrite(pul,HIGH);
    delayMicroseconds(5000);
    digitalWrite(pul,LOW);
    delayMicroseconds(5000);
  }
}

void move60delay(bool var){
  float k = 0, l = E1;
  double start = millis();
  digitalWrite(dir,var);
  while (k < angres){
    //Serial.println(k);
    if (l < E2){
      float ddelay = getDelay(l);
      //Serial.println(ddelay);
      digitalWrite(pul,HIGH);
      delayMicroseconds(ddelay);
      digitalWrite(pul,LOW);
      delayMicroseconds(ddelay);
      l += F;
    }
    k += 1;
  }
  Serial.println((millis()-start)/1000);
}

void gauss_loop(){
  float k = 0, l = -0.5; 
  while (k < 267){
    if (l < 2){
      float rpm = getrpm(l);
      Serial.println(rpm);
      dd = getDelayDuration(rpm);
      cont_mv(rpm);
      l += 0.009;
    }
    k += 1;
    delayMicroseconds(5000);
  }
}

void setup(){
  Serial.begin(115200);
  motor_setup();
}

void loop(){
  move60delay(HIGH);
  delay(1000);
  move60delay(LOW);
  delay(1000);
}