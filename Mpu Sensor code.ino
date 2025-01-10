#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

int ax, ay, az;   //accelorometer readings
int gx, gy, gz;   //gyroscope readings
// int ledpin1 = 7;  
// int ledpin2 = 6;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data;

// Pin definitions
int motor_control = A0;   //speed control
int motor_dir = 8;
int movespeed = 3.5; //moter does't require full speed 


// Variables for motor control

int higher_bound = 10;   // Upper threshold for axis
int lower_bound = -10;   // Lower threshold for axis


void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1); // Halt execution if initialization fails
  }
  Serial.println("MPU6050 initialized successfully!");

  // Initialize motor pins
  pinMode(motor_control, OUTPUT);
  pinMode(motor_dir, OUTPUT);

}

void loop() {
  // Read sensor values
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Map axis values
  data.X = constrain(map(gx, -17000, 17000, -90, 90), -90, 90); //mapped to 180 degree range
  // data.Y = constrain(map(gy, -17000, 17000, -90, 90), -90, 90);
  // data.Z = constrain(map(gz, -17000, 17000, -90, 90), -90, 90);

  // Debug output
  Serial.print("Axis X = ");
  Serial.print(data.X);
//  Serial.print("  Axis Y = ");
//  Serial.print(data.Y);
//  Serial.print("  Axis Z = ");
//  Serial.println(data.Z);

  // Motor control logic
  if (data.X <= lower_bound) {
    analogWrite(motor_control, movespeed);
    digitalWrite(motor_dir, 0);
    Serial.println("Motor running forward");
  } else if (data.X > lower_bound && data.X <= higher_bound) {
    analogWrite(motor_control, 0);
    digitalWrite(motor_dir, 0);
    Serial.println("Motor not running");
  } else if (data.X > higher_bound) {
    analogWrite(motor_control, movespeed);
    digitalWrite(motor_dir, 1);
    Serial.println("Motor running in Reverse");
  } else {
    // Stop motor if data.X is invalid
    analogWrite(motor_control, 0);
    Serial.println("Motor stopped.");
  }

  delay(100);
}
