// このファイルはArduino Unoで動作を確認するために使用したスケッチです。
// targetAngleに指定した角度にモーターが回転します。

#include <Servo.h>

Servo Parallax;

// set signal pin to Pin no.2
volatile float angle, targetAngle;
int theta = 0, thetaP = 0;
volatile unsigned long tHigh = 0, tLow = 0;
volatile unsigned long rise = 0, fall = 0;
volatile float dc;
volatile int tCycle;
int turns = 0;
int Kp = 1;
int unitsFC = 360;
float dcMin = 0.029;
float dcMax = 0.971;
float dutyScale = 1.0;
float q2min = unitsFC / 4.0;
float q3max = q2min * 3.0;

void setup() {
  // put your setup code here, to run once:
  Parallax.attach(3);
  attachInterrupt(0, feedback360, CHANGE);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int errorAngle, output, offset, value;
  targetAngle = 270;

  Serial.print(" Angle: ");
  Serial.println(angle);

  errorAngle = targetAngle - angle;
  output = errorAngle * Kp;
  if(output > 200)
    output = 200;
  if(output < -200)
    output = -200;
  if(errorAngle > 0)
    offset = 30;
  else if(errorAngle < 0)
    offset = -30;
  else
    offset = 0;
  value = (abs(errorAngle) < 4) ? 0 : output + offset;
  Parallax.writeMicroseconds(1490 - value);
}

void feedback360() {
  if(digitalRead(2)) {
    // rise
    rise = micros();
    tLow = rise - fall;

    // Calcuate duty
    tCycle = tHigh + tLow;
    if((tCycle < 1000) && (tCycle > 1200))
      return;
    
    dc = (dutyScale * tHigh) / (float)tCycle;
    theta = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);
  
    if(theta < 0)
      theta = 0;
    else if(theta > (unitsFC - 1))
      theta = unitsFC - 1;
  
    if((theta < q2min) && (thetaP > q3max))
      turns++;
    else if((thetaP < q2min) && (theta > q3max))
      turns--;
  
    if(turns >= 0)
      angle = (turns * unitsFC) + theta;
    else if(turns < 0)
      angle = ((turns + 1) * unitsFC) - (unitsFC - theta);
  
    thetaP = theta;
  
  } else {
    // fall
    fall = micros();
    tHigh = fall - rise;
  }
}
