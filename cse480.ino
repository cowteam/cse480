#include <QTRSensors.h>
#include <Servo.h> 

//constant pin values
const int LEFT_MOTOR_PIN=10;
const int RIGHT_MOTOR_PIN=11;
const int LED_PIN=13;
const int BUTTON_PIN=12;

//servo stop values are 89 (determined by experimentation)
const int STOP_VALUE=89;

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
  
  //Sensor calibration routine
  calibrateRoutine();
}

// the loop function runs over and over again forever
void loop() {

  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int leftSpeed = 91;
  int rightSpeed = 87;
  
  //Utilized for sensor debug
//  for (int i = 0; i < 8; i++)
//  {
//    Serial.println(sensors[i]);  
//  }
//
//  Serial.println("-----------");
//  Serial.println(position);
//  Serial.println("-----------");
//  delay(1000);


  //if we sense an intersection
  if (sensors[0] > 100 && sensors[1] > 100 && sensors[2] > 100 && sensors[5] > 100 && sensors[6] > 100 && sensors[7] > 100){
    //intersection to the left/right?
    stopMovement();
    forward(leftSpeed, rightSpeed);
    delay(500);
    stopMovement();
    
    qtr.readLine(sensors);
  
    if (sensors[0] > 200 && sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200 && sensors[4] > 200 && sensors[5] > 200 && sensors[6] > 200 && sensors[7] > 200){
      stopMovement();
      celebrate();
      delay(5000);
    }
    else{
      turnRight();
    }
  }
  else if(sensors[0] > 200 && sensors[1] > 200 && sensors[2] > 200 && sensors[3] > 200){
    stopMovement();
    forward(leftSpeed, rightSpeed);
    delay(500);
    
    qtr.readLine(sensors);

   if (sensors[0] > 100 && sensors[1] > 100 && sensors[2] > 100 && sensors[3] > 100 && sensors[4] > 100 && sensors[5] > 100 && sensors[6] > 100 && sensors[7] > 100){
      stopMovement();
      celebrate();
      delay(5000);
    } 
    else{
    turnRight();
    }  
  }
  else if (sensors[5] > 200 && sensors[6] > 200 && sensors[7] > 200){
    stopMovement();
    forward(leftSpeed, rightSpeed);
    delay(500);
    stopMovement();
    
    qtr.readLine(sensors);
    
    if (sensors[0] < 50 && sensors[1] < 50 && sensors[2] < 50 && sensors[3] < 50 && sensors[4] < 50 && sensors[5] < 50 && sensors[6] < 50 && sensors[7] < 50){
    turnLeft();
    }
  }
  else if (sensors[0] < 50 && sensors[1] < 50 && sensors[2] < 50 && sensors[3] < 50 && sensors[4] < 50 && sensors[5] < 50 && sensors[6] < 50 && sensors[7] < 50){
    turnAround();
  }
  
  //position correction
  if((sensors[0] > 100 && sensors[1] > 100 && sensors[2] > 100 && sensors[3] > 100) || (sensors[4] > 100 && sensors[5] > 100 && sensors[6] > 100 && sensors[7] > 100)){
    position = 2750;  
  }
  
  
  int error = position - 2750;
  int offset = map(abs(error),100, 1500, 1,7);
  
  if (error < -100){ // the line is on the right
    leftSpeed = leftSpeed + offset;
    forward(leftSpeed, rightSpeed);
  }
  else if (error > 100){ // the line is on the left
    rightSpeed = rightSpeed - offset;
    forward(leftSpeed, rightSpeed);
  }
  else{
  forward(leftSpeed, rightSpeed);
  }

}

void forward(int leftSpeed,int rightSpeed) {
  //default forward values (fast) should be:
  //leftSpeed = 97
  //rightSpeed = 82
  
  leftMotor.write(leftSpeed);
  rightMotor.write(rightSpeed);
}

void stopMovement(){
  leftMotor.write(STOP_VALUE);
  rightMotor.write(STOP_VALUE);
}

void turnRight() {
  leftMotor.write(95);
  rightMotor.write(89);
  delay(650);
  leftMotor.write(89);
  rightMotor.write(89);
}

void turnLeft() {
  leftMotor.write(86);
  rightMotor.write(83);
  delay(800);
  leftMotor.write(89);
  rightMotor.write(89);
}

void turnAround() {
  leftMotor.write(85);
  rightMotor.write(85);
  delay(1050);
  leftMotor.write(89);
  rightMotor.write(89);
}

void calibrateRoutine(){
  boolean buttonPressWait = true;
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int i = 0; i < 6; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  
  turnRight();
  
  for (int i = 0; i < 6; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  
  digitalWrite(LED_PIN, LOW);
  
}

void celebrate(){
  leftMotor.write(97);
  rightMotor.write(97);  
  delay(1000);
  leftMotor.write(82);
  rightMotor.write(82);
  delay(1000);  
  leftMotor.write(97);
  rightMotor.write(82);
  delay(1000);
  leftMotor.write(82);
  rightMotor.write(97);
  delay(1000);
  leftMotor.write(89);
  rightMotor.write(89);
}
