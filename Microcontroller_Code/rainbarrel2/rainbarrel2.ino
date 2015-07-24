
// Rainbarrel Logical Control
//  Original Author: Ruben Kertesz
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


//Here we go. Logic totally integrated onboard
//***Before Startup

//Rainbarrel characteristics
int tDrainage = (30 * 60) * 60 ; // half an hour to fully drain the tank using valve (seconds)
int tOFPump = (15 * 60) * 60 ; // Time it takes to drain the tank using an overflow pump
float radius = 11.5 / 12 ; //ft
float maxDist = 5 ; // 32 / 12 ; //ft // depth to water surface
float minDist = 4 / 12 ; //ft // dist to water surface
const float mmToft = 1 / 25.4 / 12 ;
float Cpi = 3.1415 ;
float maxVolume = Cpi * maxDist * radius * radius;

// Select how to overflow � whether to overflow by opening a valve, by running the irrigation pump, or overflow pump
int useAutoIrrig = false ;
int useAutoValve = true;
int useDrainPump = true;
int useInletValve = false ;

//Select nighttime operation
int UseNightTime = true;

//Site characteristics
int rArea = 20 * 50 ; // ft^2 Enter roof area, in sq ft for 1 downspout (A ft * B ft)
//int zipcode = 45202 not needed now. Just directly calling web address

// Check vitals when awakening
// Initialize conditions 

float dist = 0;
float Volume = 0;

// called once on startup
void setup() {

// Setup valves and pumps 
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
//  pinMode(D5, OUTPUT);

  // Get time
  Spark.syncTime(); // Syncs time with the cloud

// COMMENTED OUT FOR PRESENTATION
//   if (dist < minDist) {
//     Serial.print("Water depth too high");
//      // add some code to open valve
//     int tEndHr = max((50 - Time.minute()), 5);
//     Spark.sleep(SLEEP_MODE_DEEP, int(tEndHr * 60));
//   }

  Serial.begin(115200);//9600); //115200 won't work with the maxbotix


  //Get current hour
  int currentHour = Time.hour();

  // Don't operate at night if boolean off
  if (!UseNightTime) {
    if (currentHour > 21) {
      int tToMidnight = 24 - currentHour;
        Serial.println("sleepnight");
      Spark.sleep(SLEEP_MODE_DEEP, int ((tToMidnight + 6.8) * 60 * 60)); // want to wake up a few minutes before 7 so I don't sleep for 50 min again
    }
    if (currentHour < 7) {
      Serial.println("sleepnight");
      Spark.sleep(SLEEP_MODE_DEEP, int ((6.8 - currentHour) * 60 * 60)); // same thing, wait until morning a little before 7
    }
  }

  int readanalog3 = analogRead(A3);
  delay(100);
  dist = mmToft * (readanalog3) * 5/4 ; //EZread();
  Volume = Cpi * dist * radius * radius; // Initialize volume - this is the available capacity

  // Lets listen for the hook response
  Spark.subscribe("hook-response/HEREgetSomebody'sWeatherDATA", gotWeatherData, MY_DEVICES);

  // Lets give ourselves 10 seconds before we actually start the program.
  // That will just give us a chance to open the serial monitor before the program sends the request
  for (int i = 0; i < 10; i++) {
    Serial.println("waiting " + String(10 - i) + " x4 seconds before we publish");
    readanalog3 = analogRead(A3);
    delay(100);
    Serial.println(readanalog3);
    delay(100);
    dist = mmToft * (readanalog3) * 5/4 ; //EZread();
    Serial.println(dist);
    delay(1000);
  }
}
// SETUP COMPLETE
// Now go into loop

// called forever really fast
void loop() {

  Serial.println("Loop setup");
  delay(300);

  /* Try to keep system in relative sync so it samples about
  10 minutes before the hour, projecting for the next X hours.*/
  int currentMinute = Time.minute();
//  if (currentMinute < 50) {
//     Serial.println("wait till 50 after hr");
//     Spark.sleep(SLEEP_MODE_DEEP, int(50 - currentMinute) * 60);
//  }

  // Let's request the weather, but no more than once every 60 seconds.
  Serial.println("Requesting Weather!");
  delay(300);
  // publish the event that will trigger our webhook
  Spark.publish("GetUsSomeWeatherData");
    // and wait at least 60 seconds before doing it again
    delay(500000);
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
  int counter = 0;

  data++; // kludgy way of getting rid of the (") on the first character I think
  char *copy = strdup(data);
  Serial.println(copy);
    delay(500);
  // Set up arrays to hold the values from the string
  //int i = strlen(copy);
  char strArray[24][3] = {0};// = 0; // 30 different "words", each one varied length
  //  char strArray1[30][6] = {0};// = 0; // 30 different "words", each one varied length
  char strArray2[24][5] = {0};// = 0; // 30 different "words", each one varied length
  char strArray3[24][3] = {0};// = 0; // 30 different "words", each one varied length
  Serial.println("strArray");
  delay(200);
  Serial.println(strArray[1]);
  delay(400);
  Serial.println(strArray[2]);
  delay(700);

  // get the first token
  const char s[2] = "~";
  char *token;

  token = strtok(copy, s);
  if (token) {
    //    strcpy(strArray[counter++],token); //instead of strncpy,14);
    //    token = strtok(NULL, s); // Keep parsing the same string
    int q = 0;
    while (token != NULL && counter < 24)
    {
      if (q>2) q=0;
      if (q == 0) {
        strncpy(strArray[counter], token, 2); // You've got to COPY the data pointed to
            Serial.println("token1");
            delay(100);
            Serial.println(token);
            delay(200);
        //q++;
        //continue;
      }
      if (q == 1) {
        strncpy(strArray2[counter], token, 4); // You've got to COPY the data pointed to
            Serial.println("token2");
            delay(100);
            Serial.println(token);
            delay(200);
        //q++;
        //continue;
      }
      if (q == 2) {
        strncpy(strArray3[counter++], token, 2); // You've got to COPY the data pointed to
            Serial.println("token3");
            delay(100);
            Serial.println(token);
            delay(200);
        //q = 0;
      }
      q++;
      token = strtok(NULL, s);
    }
  }

  //releave memory for data

Serial.println("we madeit this far");
  delay(500);
  int vHour[24] = {0};
  //vTemp [36] // we aren't doing the temperature check right now but will in the future
  float vQPF[24] = {0};
  float vPOP[24] = {0};

//  int i = 0 ;
Serial.println("vectors> vHour, vQPF, vPOP ") ;
  for (int i = 0; i < 24; i++) { // step through all 36 hours of data
    vHour [i] = atoi(strArray[i]);
    vQPF [i] = atof(strArray2[i]);
    vPOP [i] = 0.01 * atof(strArray3[i]); // turn it into percentage
    Serial.println("vectors " + String(vHour[i]) + " , " + String(vQPF[i]) + " , " + String(vPOP[i]) + " , " ) ;
      delay(250);
  }

  // free data	????????????

  //	Multiply the chance of rainfall by the QPF to get proportinal rain
  float vConvolved[24] = {0};
  for (int i = 0; i < 24; i++) {
    vConvolved[i] = vQPF[i] * vPOP[i] ;
  }

  //(also taking in uncertainty with hours from now)
  for (int i = 0; i < 24; i++) {
    vConvolved[i] = vConvolved[i] / (i+1); // the X-i is a difference to weight hours farther away lees than close hours.
    //the /Y is just to create ratiometric multiplier
    // currently only using 3 hrs as a stopgap test
  }

  // free vQPF and vPOP ?????????????

  //	Use the equation that converts depth of rainfall to volume of runoff
  for (int i = 0; i < 24; i++) {
    vConvolved[i] = (vConvolved[i] / 12) * rArea ;
   Serial.println(vConvolved[i]) ;
  }

  operateSystem(vConvolved);
}


void operateSystem(float vConvPassed[24]) {

int flagOpenValve = 0;

  // By ranking storms, we get an idea of what we can do and when.
  // Taking the long view helps us send out messages to manually irrigate,
  // to wait and auto irrigate in the evening or early morning if there is dry weather,
  // etc.

  /* I am doing a poor job at really dividing up storms into actionable information.
  What I would like to know are things like
  * If the current time is midday and it is going to storm tomorrow then tell folks to irrigate tonight
  * If auto irrigation isn't there and it is dry for a number of days, and there is water in the tank
     then send a text message or email to the owner to irrigate.
  * etc.

  Rather than shoehorn this in, I invite others to contribute to better "ranking"" code here

  // Determine time to first rainfall
  for(int i = 0; i < 24; i++){
  	if(vConvPassed[i]>0) {
  		break;
  	}
  int firstStormIndex = i; //hours into the future (0 indicates the next hour or nearly immediately)

  //	Divide the rainfall data into individual storms
  float vStorm[]; // not the most efficient, how to make dynamic?
  j = 0 ;
  for(int i = firstStormIndex; i <= 35; i++){
  	if(vConvolved[i]>0) {
  		vStorm[j] = vStorm[i] + vConvPassed[i]
  	}
  	if(vConvPassed[i]<0) {
  		if
  		j++ ;
  	}
  }

  int nStorms = j;
*/
  


  // Not worrying about freezing weather right now
  /*
  �	Check if temperature is < 32F in next 24 hours
  o	Yes
  	?	Is there a storm before freezing?
  	�	AND Yes
  		o	Is automated inlet valve present?
  		?	Yes
  			�	Then get the time it will drop below 35*F (three degrees of safety)
  				o	If the end of the storm > 1 hours earlier than the temperature drops below 35.
  					?	Then allow the rainbarrel to receive water
  					?	Drain and close inlet following the passing of the storm
  				o	Else close the inlet valve
  				o	Catch error if inlet valve doesn�t close (if equipped with position sensor) and send user a message
  		?	No automated inlet valve :(
  			�	Instruct the owner to close the inlet valve or disconnect the downspout from the tank
  		o	Catch error if tank begins filling up with water (inlet valve didn�t close) and send user a message
  	�	NO storm before freezing - break
  o	NO temperature below 32 F - break
  */


  // Just looking 3 hours into the future right now. Not using the fancier logic
  //	Is available capacity enough to capture the entire volume of the next three hours?
  float threeHrVolume = vConvPassed[0] + vConvPassed[1] + vConvPassed[2] ;
  
    Serial.println ("3 hours of volume (ft^3)" + String(vConvPassed[0]) + " , " + String(vConvPassed[1]) + " , " + String(vConvPassed[2]) ) ;
    delay(200);
    
    threeHrVolume = threeHrVolume + 200 ;
    Serial.println ("We artificially added 200 gallons of runoff for demonstration..." + String(threeHrVolume) ) ;
    delay(200);


  if (threeHrVolume < Volume) { // here available volume is greater than expected precip
    //	Do nothing, just wait
    // May want to look at rainfall father into the future (in future builds) to do something about irrigation etc. 
    Serial.println("we have enough capacity at these many gallons..." + String(Volume) );
  }

  else { //Not enough space curently available
    //	Could determine if  the total capcity of the rainbarrel enough to capture storm but what's the point? We'll just drain it completely anyway.
    
    //	Drain volume to the depth required to capture all the storm.
    // open valve and possibly run pump
    if (useAutoValve) {
        // I'm using a slave arduino with a motor shield on it. We should be able to directly attach the H-bridge motor shield but 
        // I'm pressed for time and I had trouble getting it to work correctly and I have arduino unos laying around (with seeedstudio 1.0 shields)
        // First we need to send a signal over to the slave arduino
        Serial.println("Open the Valve");
        delay(200);
      digitalWrite(D3, HIGH); // I've just sent a command to open the valve
      delay (2000);
      digitalWrite(D3, LOW);
      delay (200);
      flagOpenValve = 1; // make sure we remember that the valve is open
    }
    while ((Volume < threeHrVolume) && (dist < maxDist)) {
      if (useDrainPump) {
        Serial.println("Run Drainage pump");
        delay(200);
        digitalWrite (D3, HIGH);
        digitalWrite (D4, HIGH);
        delay (2000);
      }
      else {
        // no pump, just wait
      }
        int readanalog3 = analogRead(A3);
        delay(200);
        dist = mmToft * (readanalog3) * 5/4 ; //EZread();
        Volume = Cpi * dist * radius * radius; // Initialize volume
        Serial.println("3 hr volume=" + String(threeHrVolume) + "Available volume" + String(Volume));
        delay(2000);
    }
    // Out of while loop, let's turn the pump off so we don't destroy it
    // probably should stick this inside of an if (useDrainPump) statement for those who may connect something else here instead

    Serial.println("Turn Pump Off");
    delay(200);
    digitalWrite (D3, LOW); // PUMP off
    digitalWrite (D4, LOW); // pump is off
    delay (2000);
  }

  // Make sure everything is closed up
  if ((Volume > threeHrVolume) || (dist > maxDist)) {
    Serial.println("Make sure everything is Off and Close the Valve");
      delay (100);
    digitalWrite (D4, LOW); // pump is off JUST in case
//      delay (2000);
    digitalWrite(D3, LOW); //valve isn't trying to open, just in case
    delay (2000);
    if (flagOpenValve == 1){
       digitalWrite(D4, HIGH); // I've just sent a command to close the valve
       delay (2000);
       digitalWrite(D4, LOW);
       delay (2000);
    }
  }

  //Fancier logic that I am not implementing right now
  
//  If available capacity IS NOT enough to capture the entire volume of the storm
//  	o	Determine when the peak hour of precipitation is.
//  		?	Set HourToStartStoring variable to the appropriate hour.
  

  //	Is the next event in greater than 3 hours
  //	o	Yes
  //		?	Determine when irrigation is appropriate
  // Determine if there are long gaps between storms where irrigation can occur
//  if (useAutoIrrig) {
    ////Not doing this right now because irrigation is not being used
    //	�	If rainfall is not expected >0.1 in for > 24 hrs
    //		�	Is it before 8:00 AM or after 7:00 PM?
    //		o	Yes
    //			?	Is there a signal from the garden?
    //			�	Yes
    //				o	If the signal shows that the soil is < 20 percent saturated, then irrigate until rainbarrel is empty or until moisture reaches 20 percent
    //			�	No
    //				o	If it has been dry for >24 hours, then irrigate 15 gallons in the morning, 15 gallons at night.
    //		o	No (after 8 or before 7)
    //			?	Irrigate using necessary capacity if not freezing
//  }

  Serial.println("Go to Sleep");
  delay(100);
  int tEndHr = max((50 - Time.minute()), 5);
  Spark.sleep(SLEEP_MODE_DEEP, int(tEndHr * 60));
}


/*
// This is if we want to sample distance serially
float EZread() { 
  int result;
  int index = 0;
  boolean startreading = false;
  char in[5]={0};// = {0,0,0,0,0,0,0};
  Serial.flush(); // flushes current serial data into memory. Slightly different than the name suggests 

while(index < 4){
    if(Serial.available()){
      byte incoming = Serial.read();    
        
      if(incoming == 'R') {
        startreading = true; 
      } 
      else if(startreading == true) {
        in[index++] = incoming; 
      }
    //else {return result = 1;}//error check
    }
   // else{return result = 2;}//error check
}

  result = atoi(in);        // Changes string data into an integer for use
 // Serial.print(result);   // Debugging
  return result;
}
*/
