#include <QTRSensors.h>
#include <Servo.h> 

//servo stop values are 89
Servo leftMotor;
Servo rightMotor;

QTRSensorsRC qtr((unsigned char[]) {2,3,4,5,6,7,8,9}, 8);

// the setup function runs once when you press reset or power the board
void setup() {  
  //Initializes the serial port for debug communication
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  int i;
  for (i = 0; i < 250; i++)  // make the calibration take about 5 seconds
  {
    qtr.calibrate();
    delay(20);
  }
  
  digitalWrite(13, LOW);
  
  //Attaches the motors to their respective pins on the Arduino
  leftMotor.attach(10);
  rightMotor.attach(11);
}

// the loop function runs over and over again forever
void loop() {
unsigned int sensors[8];
int position = qtr.readLine(sensors);

int i;
for (i = 0; i < 8; i++)  // make the calibration take about 5 seconds
{
Serial.println(sensors[i]);  
}

Serial.println("-----------");
delay(1000);

}

void forward() {
  leftMotor.write(97);
  rightMotor.write(82);
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


