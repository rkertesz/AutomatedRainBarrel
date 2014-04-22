/*
Following the simple example of running shell commands using Process class.
This is used to parse data from wunderground for the rainbarrel to predict upcoming rainfall.

From the original sketch description:
 This sketch demonstrate how to run linux shell commands
 using an Arduino Yún. It runs the  script on the linino side
 of the Yun...
 created 12 Jun 2013
 by Cristian Maglie
 modified 25 June 2013
 by Tom Igoe
 This example code is in the public domain.
 http://arduino.cc/en/Tutorial/ShellCommands
 */

#include <Process.h>

void setup() {
  Bridge.begin();	// Initialize the Bridge
  Serial.begin(9600);	// Initialize the Serial

  // Wait until a Serial Monitor is connected.
  while (!Serial);
}

void loop() {
  Process p;
  // This command line runs the weather underground script (symlinked on the SD card)
  p.runShellCommand("/mnt/sda1/arduino/try.py");

  // do nothing until the process finishes, so you get the whole output:
  while (p.running());

  // Read command output. 
//  while (p.available()) {
//    SOME result = p.parseSOMETHING();			// look for rainfall, etc
//    Serial.println(result);			// print the result as well
//  }
  delay(60000);  // wait at leas a minute
}



