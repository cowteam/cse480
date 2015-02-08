#include <QTRSensors.h>
#include <Servo.h> 

const int LEFT_MOTOR_PIN=10;
const int RIGHT_MOTOR_PIN=11;
const int LED_PIN=13;
const int BUTTON_PIN=1;

//servo stop values are 89 (determined by experimentation)
Servo leftMotor;
Servo rightMotor;

QTRSensorsRC qtr((unsigned char[]) {2,3,4,5,6,7,8,9}, 8);

// the setup function runs once when you press reset or power the board
void setup() {  
  
  //Initializes the serial port for debug communication
  Serial.begin(9600);
  
  //Attaches the motors to their respective pins on the Arduino
  leftMotor.attach(LEFT_MOTOR_PIN);
  rightMotor.attach(RIGHT_MOTOR_PIN);
  
  //Enable debug LED and button
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  //Sensor calibration routine, currently interaction based 
  //button press to initiate
  calibrateRoutine();
}

// the loop function runs over and over again forever
void loop() {

  unsigned int sensors[8];
  int position = qtr.readLine(sensors);

// Utilized for sensor debug
//  for (int i = 0; i < 8; i++)
//  {
//    Serial.println(sensors[i]);  
//  }
//
//  Serial.println("-----------");

//  Serial.println(position);
  
  int error = position - 3500;

  int leftSpeed = 91;
  int rightSpeed = 87;
  int offset = map(abs(error),500, 1500, 1,5);
  
  forward(leftSpeed, rightSpeed);
  
  if (error < -500){ // the line is on the right
    leftSpeed = leftSpeed + offset;
    forward(leftSpeed, rightSpeed);
  }
  else if (error > 500){ // the line is on the left
    rightSpeed = rightSpeed - offset;
    forward(leftSpeed, rightSpeed);
  }
  else{
  forward(leftSpeed, rightSpeed);
  }


}

void forward(int leftSpeed,int rightSpeed) {
  //default forward values should be:
  //leftSpeed = 97
  //rightSpeed = 82
  
  leftMotor.write(leftSpeed);
  rightMotor.write(rightSpeed);
}

void turnRight() {
  leftMotor.write(93);
  rightMotor.write(93);
  delay(850);
  leftMotor.write(89);
  rightMotor.write(89);
}

void turnLeft() {
  leftMotor.write(85);
  rightMotor.write(85);
  delay(800);
  leftMotor.write(89);
  rightMotor.write(89);
  delay(5000); 
}

void calibrateRoutine(){
  boolean buttonPressWait = true;
  
//  while(buttonPressWait)
//  {
//    if (digitalRead(BUTTON_PIN == HIGH))
//    {
//      buttonPressWait = false;
//    }
//  }
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int i = 0; i < 6; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  
  turnRight();
  turnRight();
  
  for (int i = 0; i < 6; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  
  digitalWrite(LED_PIN, LOW);
  
//  while(buttonPressWait==false);
//  {
//    if (digitalRead(BUTTON_PIN == HIGH))
//    {
//      buttonPressWait = true;
//    }
//  } 
}
