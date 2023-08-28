/*
Arduino Servo Test sketch
*/
#include <Servo.h>
Servo servoMain; // Define our Servo

void setup()
{
   servoMain.attach(9); // servo on digital pin 9
}

void loop()
{
   servoMain.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);          // Wait 1 second     
   servoMain.write(45);  // Turn Servo Left to 45 degrees
   delay(1000);          // Wait 1 second
   servoMain.write(0);  // Turn Servo Left to 0 degrees
   delay(1000);          // Wait 1 second   
}
