#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 8); // RX, TX
Servo servo;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  servo.attach(10);
  pinMode(3,OUTPUT) ; //Logic pins are also set as output
  pinMode(4,OUTPUT) ;
}

int wtd = 0;
int servo_pos_curve[16] = {
    0, //stop
    60, //1
    70, //2
    80, //3
    86, //4
    100, //5
    110, //6
    120 //7
};

void loop() {
if (Serial.available() || mySerial.available()) {
    wtd = 0;
    unsigned char inChar;
    if (Serial.available()) inChar = (unsigned char)Serial.read();
    else inChar = (unsigned char)mySerial.read();
    Serial.println(inChar);
    if ((inChar & 0xF0) != 0x50) {
        Serial.println("not command");
        return;
    }
    int servo_pos = inChar & 0x7;
    
    if (servo_pos != 0) {
        int degree = servo_pos_curve[servo_pos];
        Serial.println(degree);
        servo.write(servo_pos_curve[servo_pos]);
    } else {
        Serial.println("not servo");
    }
    if ((inChar & 0x0F) == 0) {
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
    } else if ((inChar & 0x08)) {
        analogWrite(3,120);
        digitalWrite(4,LOW);        
    } else {
        digitalWrite(3,LOW);
        digitalWrite(4,HIGH);        
    }
    
  //delay(10);
  //wtd = wtd + 1;
  //if (wtd == 1000) {
  //  Serial.println("Watch dog timeout, break the motor!");
  //  //motor.run(BRAKE);
  //  motor.setSpeed(0);
  }
}
