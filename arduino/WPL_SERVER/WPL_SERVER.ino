#include <Servo.h>
#include <AFMotor.h>

AF_DCMotor motor(4);
Servo servo;
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 2);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  servo.attach(10);
}

int wtd = 0;

void loop() {
  if (Serial.available() || mySerial.available()) {
    wtd = 0;
    unsigned char inChar;
    if (Serial.available()) inChar = (unsigned char)Serial.read();
    else inChar = (unsigned char)mySerial.read();
    if ((inChar & 0x80) != 0x80) {
        return;
    }
    if ((inChar & 0xC0) == 0xC0) {
      Serial.println("Motor Command");
      bool back = inChar & 0x20;
      if (back) Serial.print("Back ");
      else Serial.print("Forward ");
      int deg = inChar & 0x1F;
      if (deg == 0) {
        motor.run(BRAKE);
        Serial.println(" Brake!");
      } else if (deg == 0x1F) {
        motor.run(RELEASE);
        
        Serial.println(" Release!");
      } else {
        
      }
      if (deg < 4 && deg != 0) deg = 4; 
      Serial.println(deg);
      deg = deg * 15;
      if (back) motor.run(BACKWARD);
      else motor.run(FORWARD);
      motor.setSpeed(deg);
      //delay(50);
    } else if ((inChar & 0xC0) == 0x80) {
      Serial.print("Servo Command: ");
      Serial.print(inChar);
      bool left = inChar & 0x20;
      if (left) Serial.print(" Left ");
      else Serial.print(" Right ");
      int deg = inChar & 0xF;
      
      if (deg > 4) deg = 4;
      Serial.println(deg);
      //if (deg != 0) {
        deg = deg * 10;
        if (left) deg = 90 + deg;
        else deg = 90 - deg;
        servo.write(deg);
      //}
      //delay(50);
    }
  }
  //delay(10);
  wtd = wtd + 1;
  //if (wtd == 1000) {
  //  Serial.println("Watch dog timeout, break the motor!");
  //  //motor.run(BRAKE);
  //  motor.setSpeed(0);
  //}
}
