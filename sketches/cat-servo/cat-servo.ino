#define USE_SONAR 1

#ifdef USE_SONAR
#include <Ultrasonic.h>
const int trigPin = 2;
const int echoPin = 3;
Ultrasonic ultrasonic(trigPin, echoPin);
#endif

#include <Servo.h>
const int servoPin = 9;
Servo servo;

void setup(){  
#ifdef USE_SONAR
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
#endif

  servo.attach(servoPin);    
  servo.write(90);
  delay(1000);
  servo.detach();

  Serial.begin(9600);
}

const int minDist = 10;

void loop(){

#ifdef USE_SONAR
  float dist = ultrasonic.Ranging(CM);
#else
  float dist = 5;
#endif

  if (dist<minDist) {

    Serial.println(dist);
    
    servo.attach(servoPin);
    servo.write(45);
    delay(500);
    servo.write(90);
    delay(500);
    servo.detach();
    delay(1000);
  }
}
