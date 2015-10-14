/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

#include <EEPROM.h>

byte START = 0xE0;
byte END = 0xE7;

// EEPROM setting address locations:
//
//  0x00        -- MAKE
//  0x01        -- SOFTWARE_REVISION
//
//  0x10+       -- CONFIGURATION ARRAY

// Each alarm:
//  - 3 bytes
//  0x0  -- Altitude MSB
//  0x1  -- Altitude LSB
//  0x2  -- Configuration

// Configuration byte:
// ALARM_SPEED - UNUSED - UNUSED - UNUSED - UNUSED - ALARM2..ALARM0
//
// ALARM2..ALARM0: The alarm sound upon activation
// ALARM_SPEED:
//   1: Fast (for freefall)
//   0: Slow (for canopy)

//   [MODE][ALARM][BYTE]
//   4 total modes, 10 total alarms per mode, 3 total bytes per alarm

const int MODES = 4;
const int ALARMS = 10;
const int BYTES = 3;
const int CONFIGURATION_LENGTH = MODES * ALARMS * BYTES;
byte configuration[CONFIGURATION_LENGTH];

const int EEPROM_OFFSET = 16;


void setAlarm(int alarmIndex, int alarmAltitude, int alarmSound, boolean alarmSpeed) {
  // set msb
  configuration[alarmIndex++] = highByte(alarmAltitude);
  
  // set lsb
  configuration[alarmIndex++] = lowByte(alarmAltitude);
  
  // set miscellaneous configuration
  configuration[alarmIndex] = (alarmSpeed << 7) | ((int)alarmSound);
}

int getAlarm(int mode, int alarm) {
  return mode * ALARMS * BYTES + alarm * BYTES;
}

void setDefaults() {
  setAlarm(getAlarm(0, 0), 4500, 1, 1);
  setAlarm(getAlarm(1, 0), 5000, 1, 1);
  setAlarm(getAlarm(2, 0), 5500, 1, 1);
  setAlarm(getAlarm(3, 0), 6000, 1, 1);
  
  for (int i = 0; i < MODES; i++) {
    // for each mode
    setAlarm(getAlarm(i, 1), 4000, 1, 1);
    setAlarm(getAlarm(i, 2), 3000, 2, 1);
    setAlarm(getAlarm(i, 3), 2000, 2, 1);
    setAlarm(getAlarm(i, 4), 0, 2, 1);
    setAlarm(getAlarm(i, 5), 900, 5, 0);
    setAlarm(getAlarm(i, 6), 600, 5, 0);
    setAlarm(getAlarm(i, 7), 300, 5, 0);
    setAlarm(getAlarm(i, 8), 0, 5, 0);
    setAlarm(getAlarm(i, 9), 0, 5, 0);
  }
}

void readConfigurationFromEeprom() {
  for ( int i = 0; i < CONFIGURATION_LENGTH; ++i ) {
    configuration[i] = EEPROM.read(EEPROM_OFFSET + i);
  }
}

void writeConfigurationToEeprom() {
  for ( int i = 0; i < CONFIGURATION_LENGTH; ++i ) {
    EEPROM.write(EEPROM_OFFSET + i, configuration[i]);
  }
}

void serialSendConfiguration() {
  Serial.write(START);
  Serial.write(EEPROM.read(0)); // make
  Serial.write(EEPROM.read(1)); // code revision
  for ( int i = 0; i < CONFIGURATION_LENGTH; ++i ) {
    Serial.write(configuration[i]);
  }
  Serial.write(END);
}

// @return true iff no errors
boolean copyConfiguration(byte tmp[124]) {
  if (tmp[1] != 1 || tmp[2] != 1) return 0;
  if (tmp[0] != START || tmp[123] != END) return 0;
  
  for ( int i = 0; i < CONFIGURATION_LENGTH; ++i ) {
    configuration[i] = tmp[i+3];
  }
    writeConfigurationToEeprom();
    serialSendConfiguration();
    return 1;
}

void checkEeprom() {
  if (EEPROM.read(0) != 1 || EEPROM.read(1) != 1) {
    // the EEPROM make/version does not match(!!)
    // @TODO beep a shitton
    // set make/version to eeprom, generate defaults, and save it to eeprom
    EEPROM.write(0, 1);
    EEPROM.write(1, 1);
    setDefaults();
    writeConfigurationToEeprom();
  }
}

void systemHealthCheck() {
  // @TODO beep, check battery, connect to barometer, check for rapidly changing altitude

  // beep();

  // checkBattery();

  // connectBarometer();

  // checkAltitudeStability();
  
  checkEeprom();
}

byte temp[124];

void programming() {
  if (!digitalRead(7)) {
    Serial.begin(9600);
    serialSendConfiguration();
  }
  while (!digitalRead(7)) {
    // We're connected to the computer -- also, we're powered from USB (no power concerns)
    
    if (Serial.available() > 0) {
      
      Serial.readBytes(temp, 124);
      // read the incoming byte:
      copyConfiguration(temp);
    }
  }
  Serial.end();
}

void setup() {
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);

  systemHealthCheck();

  readConfigurationFromEeprom();

  programming();

  
  
}

// the loop routine runs over and over again forever:
void loop() {
}



