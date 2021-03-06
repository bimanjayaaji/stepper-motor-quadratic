/*

This script contains algorithm for :
  - guadratic movement with direct delay based speed control
    (move60quad)

*/

#include <Arduino.h>

#define pul 3
#define dir 5
#define en 6
#define lim 7
#define pulse_per_rot 1600

bool en_state=0;

float degrees = 60; 
float angres = (degrees/360)*pulse_per_rot;
float A = 3 ,B = 0.18;
float E1 = -1.1, E2 = 1.1, F = (E2-E1)/angres;
float multiplier = 500;

/*

                Quadratic Graph
_____________________________________________
| num | A   | B   | E1  | E2  |   F   | T(s)|
| 1   | 2   | 0.25| -1.5| 1.5 |0.01123| 0.59|
| 2   | 2   | 0.23| -1.4| 1.4 |0.01048| 0.53|
| 3   | 2   | 0.20| -1.3| 1.3 |0.00973| 0.47|
| 4   | 3   | 0.18| -1.1| 1.1 |0.00823| 0.49|
|_____|_____|_____|_____|_____|_______|_____|

Notations:
A         = Constant of x^2
B         = Constant "c"
E1 and E2 = range of the desired curve area
F         = increment of the range associated with angres
T(s)      = time needed each cycle

*/

void motor_setup(){
  pinMode(pul, OUTPUT); 
  pinMode(dir, OUTPUT); 
  pinMode(en, OUTPUT);
  digitalWrite(en, en_state);
}

float getDelayquad(float x){
  float y = (A*x*x) + B;
  Serial.println(y);
  return y*multiplier;
}

void move60quad(bool var){
  float k = 0, l = E1;
  double start = millis();
  digitalWrite(dir,var);
  while (k < angres){
    if (l < E2){
      float ddelay = getDelayquad(l);
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

void calibrate(){
  bool lim_sw = false;
  while(lim_sw){
    digitalWrite(dir,HIGH);
    digitalWrite(pul,HIGH);
    delayMicroseconds(5000);
    digitalWrite(pul,LOW);
    delayMicroseconds(5000);
    lim_sw = digitalRead(lim);
  }  
}

void setup(){
  Serial.begin(115200);
  motor_setup();
}

void loop(){
  move60quad(HIGH);
  delay(1000);
  move60quad(LOW);
  delay(1000);
}