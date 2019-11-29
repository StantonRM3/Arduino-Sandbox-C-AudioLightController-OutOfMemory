#include "Debug.h"

#include "clsVUData.h"
#include "clsPixelBuffer.h"
#include "clsStateMachine.h"
#include "clsColourCtrl.h"
#include "clsMatrixCtrl.h"
#include "clsStripCtrl.h"
#include "clsClock.h"
#include "clsMQTTClient.h"

clsStateMachine stateMachine;
clsColourCtrl colourCtrl;
clsMatrixCtrl matrixCtrl;
clsStripCtrl stripCtrl;
clsVUData VUData;
clsPixelBuffer pixelBuffer;
clsClock myClock;
clsMQTTClient myMQTTClient;

#define CYCLE_FREQ_POT_PIN            A6
#define LOWVOl_SENSITIVITY_POT_PIN    A7

void setup() {

#ifdef DEBUG
  Serial.begin(SERIAL_BAUD);
#endif

#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("setup()");
#endif

  I2C_Setup();

  VUData.begin();
  pixelBuffer.begin();

  inputCtrl_begin();

  colourCtrl.begin();
  myClock.begin();
  // myClock.setTimeDateFromCompilerTimestamp();    // Sets the clock to the timestamp in the compiled code

  stateMachine.begin(&colourCtrl, &myClock);
  stripCtrl.begin(&VUData, &pixelBuffer, &colourCtrl, &myClock);
  matrixCtrl.begin(&VUData, &colourCtrl, &myClock);

  myMQTTClient.begin();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MAIN LOOP()...
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned long prevTime = 0;
unsigned long prevColourTime = 0;

// Set by the Potentiaometers...
int cycleTime_ms = 60;          // Time to wait between program iterations.  This value will be used as multiplier when we need to flash items...
unsigned long colourCycleTime = 60000;

#define MIN_LOWVOLSENSITIVITY   12
int lowVolSensitivity = MIN_LOWVOLSENSITIVITY;       // At what point the stripLED triggers....

void loop() {
  unsigned long currTime = millis();

#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("loop()");
#endif

  if ((currTime - prevTime) >= cycleTime_ms)  {

    // Only load from the VU analyser if the clock isn't on display...
    if (stateMachine.showClock == OFF ) {

      VUData.I2C_loadVUData();
      pixelBuffer.movePixels();

      int peakLevel = VUData.VUDataTotal[LEFT] + VUData.VUDataTotal[RIGHT];

#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
      Serial.println(peakLevel);
#endif
      // Filter low volume sounds...
      if (peakLevel >= lowVolSensitivity) {
        pixelBuffer.addPixel(peakLevel, &colourCtrl);
      }
    }

    matrixCtrl.displaySample(stateMachine.displayMode, stateMachine.showClock, stateMachine.patternMode);
    stripCtrl.displaySample(stateMachine.displayMode, stateMachine.showClock, stateMachine.patternMode);

    if (myMQTTClient.checkForNewMessage() == true)
    {
      Serial.println("**** NEW MESSAGE - DO SOMETHING ****");
      myMQTTClient.dumpSubscriptionValues();
    }
    
    // Check if the display needs to be updated (for the clock)...  This function protects from getting over excited and will only check every 1(ish) seconds...
    stateMachine.refreshDisplay();

    readPotentiometers();

    // Check if its time to change colours...
    if ((currTime - prevColourTime) > colourCycleTime) {
      changeColour();
      prevColourTime = currTime;
    }

    prevTime = currTime;
  }

  inputCtrl_checkButtons();
}

void changeColour()
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("changeColour()");
#endif

  stateMachine.colourMode = colourCtrl.cycleNextColour(stateMachine.colourMode);
  colourCtrl.setColourPallet(stateMachine.displayMode, stateMachine.colourMode);
}

void readPotentiometers(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("readPotentiometers()");
#endif

  int cycleFreq;
  int volSensitivity;

  cycleFreq = analogRead(CYCLE_FREQ_POT_PIN);
  volSensitivity = analogRead(LOWVOl_SENSITIVITY_POT_PIN);

  cycleTime_ms = map(cycleFreq, 0, 1023, 50, 100);
  lowVolSensitivity = MIN_LOWVOLSENSITIVITY + map(volSensitivity, 0, 1023, 0, 30);

#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  char valStr[64];
  sprintf(valStr, "readPotentiometers(): Cycle:%d Sensitivity:%d", cycleTime_ms, lowVolSensitity);
  Serial.println(valStr);
#endif
}

/**
   I2C_ClearBus
   (http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html)
   (c)2014 Forward Computing and Control Pty. Ltd.
   NSW Australia, www.forward.com.au
   This code may be freely used for both private and commerical use
*/
/**
   This routine turns off the I2C bus and clears it
   on return SCA and SCL pins are tri-state inputs.
   You need to call Wire.begin() after this to re-enable I2C
   This routine does NOT use the Wire library at all.

   returns 0 if bus cleared
           1 if SCL held low.
           2 if SDA held low by slave clock stretch for > 2sec
           3 if SDA held low after 20 clocks.
*/
int I2C_ClearBus() {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("I2C_ClearBus()");
#endif

#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN));                             //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP);                       // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power

  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW);      // Check is SCL is Low.
  if (SCL_LOW) {                                    //If it is held low Arduno cannot become the I2C master.
    return 1;                                       //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);      // vi. Check SDA input.
  int clockCount = 20;                              // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) {             //  vii. If SDA is Low,

    clockCount--;

    // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT);                            // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT);                           // then clock SCL Low

    delayMicroseconds(10);                          //  for >5uS

    pinMode(SCL, INPUT);                            // release SCL LOW
    pinMode(SCL, INPUT_PULLUP);                     // turn on pullup resistors again

    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10);                          //  for >5uS

    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW);            // Check if SCL is Low.

    int counter = 20;

    while (SCL_LOW && (counter > 0)) {              //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }

    if (SCL_LOW) {                                  // still low after 2 sec error
      return 2;                                     // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }

    SDA_LOW = (digitalRead(SDA) == LOW);            //   and check SDA input again and loop
  }

  if (SDA_LOW) {                                    // still low
    return 3;                                       // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT);                              // remove pullup.
  pinMode(SDA, OUTPUT);                             // and then make it LOW i.e. send an I2C Start or Repeated start control.

  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS

  pinMode(SDA, INPUT);                              // remove output low
  pinMode(SDA, INPUT_PULLUP);                       // and make SDA high i.e. send I2C STOP control.

  delayMicroseconds(10);                            // x. wait >5uS

  pinMode(SDA, INPUT);                              // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);

  return 0; // all ok
}

void I2C_Setup() {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("I2C_Setup()");
#endif

  int rtn = I2C_ClearBus();                         // clear the I2C bus first before calling Wire.begin()

  if (rtn != 0) {

    Serial.println(F("ERROR: I2C bus error. Could not clear"));

    if (rtn == 1) {
      Serial.println(F("ERROR: SCL clock line held low"));
    }
    else if (rtn == 2) {
      Serial.println(F("ERROR: SCL clock line held low by slave clock stretch"));
    }
    else if (rtn == 3) {
      Serial.println(F("ERROR: SDA data line held low"));
    }

  }
  else {                                            // bus clear
    // re-enable Wire
    // now can start Wire Arduino master
    Wire.begin();
  }

#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("I2C_Setup() finished");
#endif
}
