/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/
//A0 Angle ee
//A1 Angle base
//A2 X base
//A3 Not connected
//A4 Maybe X ee
//A5 X ee
//A6 Y ee
//A7 Y base
// These constants won't change. They're used to give names to the pins used:
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
ros::NodeHandle nh;
std_msgs::Float32 str_msg;
ros::Publisher chatter("chatter", &str_msg);
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  //configure pin 2 and 3 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP); //PushButton right
  pinMode(3, INPUT_PULLUP); //PushButton left
  nh.initNode();
  nh.advertise(chatter);
  
}

void loop() {
  int linear_vel_max_base = 35; //mm/s
  float angular_vel_max_base = 5; //10*rad/s
  int linear_vel_max_ee = 35; //mm/s
  float angular_vel_max_ee = 5; //10*rad/s
  //read the digital state
  int rightButton = digitalRead(2);
  int leftButton = digitalRead(3);

  // read the analog in value:
  int angle_ee_DO = analogRead(A0);
  int X_ee_DO     = analogRead(A5);
  int Y_ee_DO     = analogRead(A6);

  int angle_base_DO = analogRead(A1);
  int X_base_DO     = analogRead(A2);
  int Y_base_DO     = analogRead(A7);


  // map it to the range of the analog out:
  float Y_base_vel = (float) map(Y_base_DO, 0, 1023, linear_vel_max_base, -linear_vel_max_base)/100;
  float X_base_vel = (float) map(X_base_DO, 0, 1023, linear_vel_max_base, -linear_vel_max_base)/100;

  float Y_ee_vel = (float) map(Y_ee_DO, 0, 1023, linear_vel_max_base, -linear_vel_max_base)/100;
  float X_ee_vel = (float) map(X_ee_DO, 0, 1023, linear_vel_max_base, -linear_vel_max_base)/100;

  float angle_ee_vel = 0.0;
  if (angle_ee_DO >100)
  {
    angle_ee_vel = (float) map(angle_ee_DO, 100, 600, angular_vel_max_ee, -angular_vel_max_ee)/10;
  }
  if (angle_ee_DO < 60)
  {
    angle_ee_vel = (float) map(angle_ee_DO, 60, 0, angular_vel_max_ee, -angular_vel_max_ee)/10;
  }

  float angle_base_vel = 0.0;
  if (angle_base_DO >60)
  {
    angle_ee_vel = (float) map(angle_base_DO, 60, 550, angular_vel_max_base, -angular_vel_max_base)/10;
  }
  if (angle_ee_DO < 30)
  {
    angle_ee_vel = (float) map(angle_base_DO, 30, 0, angular_vel_max_base, -angular_vel_max_base)/10;
  }
  char hello[13] = "hello world!";
  str_msg.data = 5;
  chatter.publish( &str_msg );
  std_msgs::Float32 commands;
  //commands.data = 5;
  //commands.data.push_back(3.2);
  //commands.data.push_back(2.8);
  //chatter_pub.publish(commands);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
