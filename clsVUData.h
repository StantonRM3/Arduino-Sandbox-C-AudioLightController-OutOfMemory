//*********************************************************************************************************************************************************************************************************************************************
// clsVUData.h
//
// Class containing all the functionality to manage and store VU data from samplers
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSVUDATA_H
#define CLSVUDATA_H

#include <Arduino.h>
#include <Wire.h>
#include "Enums.h"

#define SLAVE_NUMBER_LEFT       8
#define SLAVE_NUMBER_RIGHT      9

//#define PEAK_LEVEL_THRESHOLD    4
//#define PEAK_LEVEL_SAMPLES    256

class clsVUData
{
  public:

    void begin(void);
    void I2C_loadVUData(void);

    // Variables for Matrix presentation VUData=raw data; VUData_CurrentMax=current maximum value for each sample
    int VUData[2][MATRIX_WIDTH];
    int VUData_LastMax[2][MATRIX_WIDTH];

    // Variables for single display, i.e. strip LEDS or totals (simple sum of all the sample channels...
    int VUDataTotal[2];
    int VUDataTotal_LastMax[2];
    int bpm;

  private:

    void I2C_begin(void);
    void I2C_requestData(int pFromSlave);
    void calculateBPM(void);

};

#endif
