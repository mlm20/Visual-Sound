#include <Servo.h>


Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;

void setup() {
//  servo_0.attach(2);
//  servo_1.attach(3);
//  servo_2.attach(4);
  servo_3.attach(5);
  servo_4.attach(6);

//  servo_0.write(90);
//  servo_1.write(90);
//  servo_2.write(90);
  servo_3.write(90);
  servo_4.write(90);

}

void loop() {
  // put your main code here, to run repeatedly:
//  servo_0.write(90);
//  servo_1.write(90);
//  servo_2.write(90);
  servo_3.write(90);
  servo_4.write(90);

  delay(1000);

//  servo_0.write(0);
//  servo_1.write(0);
//  servo_2.write(0);
  servo_3.write(0);
  servo_4.write(0);

  delay(1000);

//  servo_0.write(180);
//  servo_1.write(180);
//  servo_2.write(180);
  servo_3.write(180);
  servo_4.write(180);

  delay(1000);
}
