#include <LowPower.h>

#include <SFE_BMP180.h>
#include <Wire.h>

#define LED_PIN (13)

#define ON_GROUND 0
#define IN_AIR    1
#define SKYDIVING 2

SFE_BMP180 pressure;

double baseline; // baseline pressure
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
    while(1); // Pause forever.
  }

  // Get the baseline pressure:
  
  baseline = getPressure();
}



struct state;
typedef void state_fn(struct state *);

struct state
{
    state_fn * next;
//    int i; // data
    enum period_t sleepPeriod; // how long the processor should sleep for the state
    float baseAltitude; // altitude, in m, above MSL == 0 AGL
};

state_fn foo, bar;

void foo(struct state * state)
{
    Serial.println(__func__);
    state->next = bar;
}

void bar(struct state * state)
{
    Serial.println( __func__);
    state->next = foo;
}

struct state state = { foo, SLEEP_8S };


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
  Serial.begin(9600);
  Serial.println("Initialising...");

  pinMode(LED_PIN, OUTPUT);

  delay(100); //Allow for serial print to complete.

  BMPSetup();

  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
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
  if (!state.next) state = { foo, SLEEP_8S};

  /* Toggle the LED */
  digitalWrite(LED_PIN, HIGH);

  // execute the next step
  state.next(&state);

  digitalWrite(LED_PIN, LOW);
  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

}




void getAltitude() {
  
  double newAlt,P;

  // Get a new pressure reading:

  P = getPressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:

  newAlt = pressure.altitude(P,baseline);

  baseline = P;
}

//void inPlane() {
//  if (speed > 80) {
//    WDT1Sec();
//    state = SKYDIVING;
//  }
//}
//
//void skydiving() {
//  if (oldAlt - newAlt > 8.0) {
//    // We are descending quickly!
//    if (absAlt < ALERT_THRESHOLD) {
//      state = ALERTING;
//    } else {
//      state = SKYDIVING;
//    }
//  }
//}
//
//void alert() {
//  while (still descending > 80 mph) {
//    // alert
//  }
//  else {
//    // go ON_GROUND
//  }
//}
//
//void onGround() {
//
//  delay(100);
//  Serial.print("relative altitude: ");
//  if (newAlt >= 0.0) Serial.print(" "); // add a space for positive numbers
//  Serial.println(newAlt,1);
//  Serial.print(oldAlt);
//  Serial.print(" ");
//  Serial.print(newAlt);
//  Serial.print(" ");
//  Serial.println(oldAlt - newAlt);
//  delay(100);
//
//
//}

void inAir() {
  
}





double getPressure()
{
  char status;
  double T,P,p0,a;

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

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}




