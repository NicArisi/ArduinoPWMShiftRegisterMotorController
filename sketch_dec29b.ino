/**
 * Two Channel Receiver
 * Author: Shawn Hymel (SparkFun Electronics)
 * Editted and Stolen by:Nic
 * Mixes two channels for arcade drive.
 */

#include "ShiftRegisterPWM.h"
// Controller pins
const int CH_1_PIN = 10;
const int CH_2_PIN = 11;
const int BInPin = 9;
ShiftRegisterPWM sr(1, 16);

// Parameters
const int deadzone = 35;  // Anything between -20 and 20 is stop. This code acts as a circle's area of when the PWM values will start to be transmitted. 
//Joystick is in the circle, no PWM transmitted, when out, PWM transmitted to ESC. 
/*We may to adjust this to our controller. 
 * Some sort of a test in the setup function to see the current voltage and set the deadzone to somewhere around that.
 */
void setup() {
  pinMode(2, OUTPUT); // sr data pin
  pinMode(3, OUTPUT); // sr clock pin
  pinMode(4, OUTPUT); // sr latch pin
  pinMode(CH_1_PIN, INPUT);
  pinMode(CH_2_PIN, INPUT);
  pinMode(BInPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::Medium);
}

void loop() {

  // Read pulse width from receiver
  int y = pulseIn(CH_2_PIN, HIGH);
  int x = pulseIn(CH_1_PIN, HIGH);
  int b = pulseIn(BInPin, HIGH);
  // Convert to PWM value (-255 to 255)
  y = pulseToPWMY(y);
  x = pulseToPWMX(x);
  b = pulseToPWMB(b);
  //blade drive function with the integer of b being the input value of int b from the function. 
  bladeDrive(b);
  // Drive motor
  drive(x, y);
  //these are randoms for the last pins of the register
  sr.set(5,255);
  sr.set(6,200);
  sr.set(7,100);
}

void bladeDrive(int b){
  if(b>100){
    sr.set(4, 255);
  }else{
    sr.set(4, 0);
  }
}
// Positive for forward, negative for reverse
void drive(int speed_a, int speed_b) {
  // Set direction for motor A
  speed_a = constrain(speed_a, -255, 255);
  speed_b = constrain(speed_b, -255, 255);
  if ( speed_a > 0 ) {
    sr.set(1,255);
  } else{
    sr.set(1,0);
  }
  // Set direction for motor B
  if ( speed_b > 0 ) {
    sr.set(3,255);
  } else{
    sr.set(3,0);
  }
  //The digital outputs were inverted to allow the forward movement with the x to y config. 

  // Set speed
  sr.set(0,abs(speed_a));
  sr.set(2,abs(speed_b));
}

// Convert RC pulse value to motor PWM value. The reason for multiple pulse readers is due to different signal ranges coming from the receiever. This allows for correct movement
int pulseToPWMY(int pulse) {
  // If we're receiving numbers, convert them to motor PWM
    pulse = map(pulse, 670, 1170, -500, 500);
    pulse = constrain(pulse, -255, 255);
  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }

  return pulse;
}
int pulseToPWMX(int pulse) {
  // If we're receiving numbers, convert them to motor PWM
  pulse = map(pulse, 685, 1176, -500, 500);
  pulse = constrain(pulse, -255, 255);
  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }
  return pulse;
}
int pulseToPWMB(int pulse) {
  // If we're receiving numbers, convert them to motor PWM
    pulse = map(pulse, 911, 1159, -500, 500);
    pulse = constrain(pulse, -255, 255);
  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }
  return pulse;
}
