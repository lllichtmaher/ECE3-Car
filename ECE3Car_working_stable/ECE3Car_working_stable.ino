#include <ECE3.h>

// YURP!!!!!!!!

uint16_t sensorMinimum[8] = {666, 550, 666, 500, 759, 596, 605, 642};
uint16_t sensorMaximum[8] = {1834, 1950, 1834, 1728, 1741, 1904, 1671, 1858};
uint16_t sensorValues[8];

const int left_nslp_pin = 31;
const int left_dir_pin  = 29;
const int left_pwm_pin  = 40;

const int right_nslp_pin = 11;
const int right_dir_pin  = 30;
const int right_pwm_pin  = 39;

const float Kp = 0.0087;
const float Kd = 0.05;

const int baseSpeed = 80;

float pasterr = 0;

void setup() {
  Serial.begin(9600);

  ECE3_Init();
  delay(2000);

  pinMode(left_nslp_pin, OUTPUT);
  pinMode(left_dir_pin,  OUTPUT);
  pinMode(left_pwm_pin,  OUTPUT);

  pinMode(right_nslp_pin, OUTPUT);
  pinMode(right_dir_pin,  OUTPUT);
  pinMode(right_pwm_pin,  OUTPUT);

  digitalWrite(left_dir_pin,  LOW);
  digitalWrite(left_nslp_pin, HIGH);

  digitalWrite(right_dir_pin,  LOW);
  digitalWrite(right_nslp_pin, HIGH);

    analogWrite(left_pwm_pin,  baseSpeed);
  analogWrite(right_pwm_pin, baseSpeed);
}

void loop() {


  ECE3_read_IR(sensorValues);

  uint16_t raw[8];
  for (int i = 0; i < 8; i++) raw[i] = sensorValues[i];

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] < sensorMinimum[i]) sensorValues[i] = sensorMinimum[i];
    if (sensorValues[i] > sensorMaximum[i]) sensorValues[i] = sensorMaximum[i];

    sensorValues[i] = (uint32_t)(sensorValues[i] - sensorMinimum[i]) * 1000
                    / (sensorMaximum[i] - sensorMinimum[i]);
  }

  float error = (sensorValues[0]15 + sensorValues[1]12 + sensorValues[2]2 + sensorValues[3]
               
sensorValues[4]   - sensorValues[5]
2 - sensorValues[6]12 - sensorValues[7]15) / 4.0;

  // error -= 226.4;

  float correction = Kp * error + Kd * (error - pasterr);
   pasterr = error;

  int leftSpd  = constrain(baseSpeed + (int)correction, 0, 255);
  int rightSpd = constrain(baseSpeed - (int)correction, 0, 255);



  analogWrite(left_pwm_pin,  leftSpd);
  analogWrite(right_pwm_pin, rightSpd);
}
