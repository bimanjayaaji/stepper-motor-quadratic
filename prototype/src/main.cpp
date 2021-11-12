/*

This script contains algorithm for :
  - normal closing/opening
  - guadratic opening/closing with
  - calibration 0 deg using lim_sw

-------------------------------------------------------
  
          Quadratic Graph Trial/Error
_____________________________________________
| num | A   | B   | E1  | E2  |   F   | T(s)|
| 1   | 2   | 0.25| -1.5| 1.5 |0.01123| 0.59|
| 2   | 2   | 0.23| -1.4| 1.4 |0.01048| 0.53|
| 3   | 2   | 0.20| -1.3| 1.3 |0.00973| 0.47|
| 4   | 3   | 0.18| -1.1| 1.1 |0.00823| 0.49|
|_____|_____|_____|_____|_____|_______|_____|

Formula : y = ax^2 + c
Notations:
A         = Constant of x^2
B         = Constant "c"
E1 and E2 = Range of the desired curve area
F         = Increment of the range associated with angres
T(s)      = Time needed each cycle

-------------------------------------------------------

dir = HIGH --> CCW
dir = LOW  --> CW

to calibrate use calibrate()
to move use either open/close normal/quad()
can be modified using certain condition

*/

#include <Arduino.h>

#define pul 3
#define dir 5
#define en 6
#define lim_open 9
#define lim_close 8
#define pulse_per_rot 1600

bool en_state = 0;
int stepper_stat;

float degrees = 60; 
float angres = (degrees/360)*pulse_per_rot;
float A = 0.3 ,B = 0;
float E1 = -5, E2 = 5, F = (E2-E1)/angres;
float multiplier = 500;

struct stats{
  int opened;
  int closed;
  int error;
}; stats status = {0,1,2};

void pin_setup(){
  pinMode(pul, OUTPUT); 
  pinMode(dir, OUTPUT); 
  pinMode(en, OUTPUT);
  digitalWrite(en, en_state);

  pinMode(lim_close,INPUT_PULLUP);
  pinMode(lim_open,INPUT_PULLUP);
}

bool get_status(){
  if (digitalRead(lim_open) == 1 && digitalRead(lim_close) == 0){
    return status.opened; 
  }
  else if (digitalRead(lim_open) == 0 && digitalRead(lim_close) == 1){
    return status.closed;    
  }
  else {
    return status.error;    
  }
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

void open_quad(){
  move60quad(LOW);
  stepper_stat = get_status();
}

void close_quad(){
  move60quad(HIGH);
  stepper_stat = get_status();
}

void movenormal(bool var, int pin){
  digitalWrite(dir,var);
  while(!digitalRead(pin)){
    digitalWrite(pul,HIGH);
    delayMicroseconds(1000);
    digitalWrite(pul,LOW);
    delayMicroseconds(1000);
  }
}

void open_normal(){
  movenormal(LOW,lim_open);
  stepper_stat = get_status();
}

void close_normal(){
  movenormal(HIGH,lim_close);
  stepper_stat = get_status();
}

void calibrate(){
  while(get_status() != status.opened){
    digitalWrite(dir,LOW);
    digitalWrite(pul,HIGH);
    delayMicroseconds(5000);
    digitalWrite(pul,LOW);
    delayMicroseconds(5000);
  }
}

void setup(){
  Serial.begin(115200);
  pin_setup();
  calibrate();
  delay(1000);
}

void loop(){
  close_quad();
  delay(3000);
  open_quad();
  delay(3000);
}