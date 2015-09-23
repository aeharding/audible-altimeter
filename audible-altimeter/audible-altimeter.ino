#include <LowPower.h>

#include <SFE_BMP180.h>
#include <Wire.h>

#define LED_PIN (13)

SFE_BMP180 pressure;


enum period_t sleepPeriod; // how long the processor should sleep for
double basePressure, previousPressure, previousAltitude; // pressure (mb): 0 AGL
boolean inPlaneTriggered;
//3.33 fpm elevator

void BMPSetup() {
  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1); // Pause forever.
  }

  // Get the baseline pressure:

  basePressure = getPressure();
  previousPressure = basePressure;
}

/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the serial comms and the
 *                Watch dog timeout.
 *
 ***************************************************/
void setup()
{
  //  Serial.begin(9600);
  //  Serial.println("Initialising...");

  //  pinMode(LED_PIN, OUTPUT);

  //  delay(100); //Allow for serial print to complete.

  BMPSetup();
  inPlaneTriggered = false;

  sleepPeriod = SLEEP_8S;

  //  Serial.println("Initialisation complete.");

  tone(3, 2048);
  delay(50);
  noTone(3);
  delay(50);
  tone(3, 2048);
  delay(50);
  noTone(3);

  //  delay(100); //Allow for serial print to complete.
}

/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
void loop()
{
  int i;
  double currentPressure, currentAltitude;

  currentPressure = getPressure();

  //  Serial.print("Current pressure:");
  //  Serial.println(currentPressure);

  currentAltitude = pressure.altitude(currentPressure, basePressure);

  //  Serial.print("Current altitude:");
  //  Serial.println(currentAltitude);

  if (currentAltitude < 5 && currentAltitude > -5 && !inPlaneTriggered) {
    //    Serial.println("On ground - adjusting for altitude");
    sleepPeriod = SLEEP_8S;
    // consider this noise, set as base
    basePressure = currentPressure;
  } else {
    //    Serial.println("In plane");
    inPlaneTriggered = true;
    // we're in the air
    sleepPeriod = SLEEP_1S;

    // human falls at 53 m/s
    // open canopy, full flight 5 m/s
    // refresh rate is 1 s on arduino
    //    Serial.print("DIFF:");
    //    Serial.println(previousAltitude - currentAltitude);
    while (previousAltitude - currentAltitude > 30 && // m/s
           currentAltitude < 1371) { // 4500 ft
      currentPressure = getPressure();
      currentAltitude = pressure.altitude(currentPressure, basePressure);
      //      Serial.println("TRIGGER!");
      beep1s();
      previousPressure = currentPressure;
      previousAltitude = currentAltitude;
    }
  }

  previousPressure = currentPressure;
  previousAltitude = currentAltitude;

  //  delay(100); // for console

  LowPower.powerDown(sleepPeriod, ADC_OFF, BOD_ON);
}


void beep1s() {
  // 142.8
  int i;
  for (i = 0; i < 10; i++) {
    tone(3, 1900);
    delay(30);
    noTone(3);
    delay(70);
  }
  
//  int i;
//  for (i = 1800; i < 2500; i += 20) {
//    tone(3, i);
//    delay(5);
//    noTone(3);
//  }
}



double getPressure()
{
  char status;
  double T, P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}




