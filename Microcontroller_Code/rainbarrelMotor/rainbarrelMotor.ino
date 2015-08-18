
// Rainbarrel valve and pump control module
// Extension of the Motor driver shield- 2012 Copyright (c) Seeed Technology Inc.
//
//  Original Author: Jimbo.we
//  Contribution: LG
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

int pinI1 = 8; //define I1 interface
int pinI2 = 11; //define I2 interface
int speedpinA = 9; //enable motor A
int pinI3 = 12; //define I3 interface
int pinI4 = 13; //define I4 interface
int speedpinB = 10; //enable motor B
int spead = 127; //define the spead of motor
int incomingByte;      // a variable to read incoming serial data into

void setup()
{
  pinMode(pinI1, OUTPUT);
  pinMode(pinI2, OUTPUT);
  pinMode(speedpinA, OUTPUT);
  pinMode(pinI3, OUTPUT);
  pinMode(pinI4, OUTPUT);
  pinMode(speedpinB, OUTPUT);

  Serial.begin(9600);
}


void loop()
{
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();

    if (incomingByte == 'O') {
      vopen();
      delay(3000);
      Serial.write('O');
      delay(300);
      stopnow(); //  stopnow includes delay
    }

    if (incomingByte == 'P') {
      //just in case the valve isn't open it, let's open it.
      // the valve itself won't open any more than its maximum (has a limiter),
      // so this shouldn't burn it out. ..
      // ..this will eventually be the secondary valve.
      vopen();
      delay(3000);
      stopnow(); //  stopnow includes delay
      forward(); // RUN PUMP
      Serial.write('P');
      delay(1000);
    }

    if (incomingByte == 'S') {
      stopnow(); //  stopnow includes delay
      Serial.write('S');
      delay(1000);
    }
    
    if (incomingByte == 'C') {
      vclose();
      delay(3000);
      Serial.write('C');
      delay(300);
      stopnow();
    }


    // IF I WANTED TO RUN PUMP IN REVERSE
    //  backward();
    //  delay(3000);
    //  stopnow();
  }
}


void forward() // Pump forward
{
  digitalWrite(pinI4, LOW); //turn DC Motor B move clockwise
  digitalWrite(pinI3, LOW);
  analogWrite(speedpinA, spead); //input a simulation value to set the speed
  analogWrite(speedpinB, spead);
  digitalWrite(pinI2, LOW); //turn DC Motor A move anticlockwise
  digitalWrite(pinI1, HIGH);
}
void backward()// Pump Backward
{
  digitalWrite(pinI4, LOW); //turn DC Motor B move anticlockwise
  digitalWrite(pinI3, LOW);
  analogWrite(speedpinA, spead); //input a simulation value to set the speed
  analogWrite(speedpinB, spead);
  digitalWrite(pinI2, HIGH); //turn DC Motor A move clockwise
  digitalWrite(pinI1, LOW);
}
void vopen()// open valve
{
  digitalWrite(pinI2, LOW); //turn DC Motor A move clockwise
  digitalWrite(pinI1, LOW);
  analogWrite(speedpinA, spead); //input a simulation value to set the speed
  analogWrite(speedpinB, spead);
  digitalWrite(pinI4, HIGH); //turn DC Motor B move clockwise
  digitalWrite(pinI3, LOW);
}
void vclose()// close valve
{
  digitalWrite(pinI2, LOW); //turn DC Motor A move clockwise
  digitalWrite(pinI1, LOW);
  analogWrite(speedpinA, spead); //input a simulation value to set the speed
  analogWrite(speedpinB, spead);
  digitalWrite(pinI4, LOW); //turn DC Motor B move anticlockwise
  digitalWrite(pinI3, HIGH);
}
void stopnow()//
{
  digitalWrite(pinI4, LOW); //turn DC Motor B move anticlockwise
  digitalWrite(pinI3, LOW);
  digitalWrite(pinI2, LOW); //turn DC Motor A move clockwise
  digitalWrite(pinI1, LOW);
  digitalWrite(speedpinA, LOW); // Unenble the pin, to stopnow the motor. this should be done to avid damaging the motor.
  digitalWrite(speedpinB, LOW);
  delay(1000);
}



