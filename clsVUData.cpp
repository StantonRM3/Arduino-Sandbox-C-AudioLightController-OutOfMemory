//*********************************************************************************************************************************************************************************************************************************************
// clsVUData.cpp
//
// Class containing all the functionality to manage getting and manipulating VU Data fro  samplers...
//
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"
#include "clsVUData.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// I2C - COMMS (REQUEST DATA FROM SAMPLERS)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clsVUData::begin(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsVUData::begin()");
#endif

  // Blank all values down to zero (just to be sure to be sure)..
  for (int i = LEFT; i <= RIGHT; i++) {
    for (int j = 0; j < MATRIX_WIDTH; j++) {
      VUData[i][j] = 0;
      VUData_LastMax[i][j] = 0;
    }
    VUDataTotal[i] = 0;
    VUDataTotal_LastMax[i] = 0;
  }

  clsVUData::I2C_begin();
}

void clsVUData::I2C_begin(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsVUData::I2C_begin()");
#endif

  // Wire.begin();      // join i2c bus (address optional for master)
}

void clsVUData::I2C_loadVUData(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsVUData::I2C_loadVUData()");
#endif

  I2C_requestData(SLAVE_NUMBER_LEFT);
  I2C_requestData(SLAVE_NUMBER_RIGHT);
}

void clsVUData::I2C_requestData(int pFromSlave) {
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsVUData::I2C_requestData()");
#endif

  Wire.requestFrom(pFromSlave, (MATRIX_WIDTH / 2));  // request 16 bytes from slave device #8

#ifdef DEBUG_I2C
  if (pFromSlave == SLAVE_NUMBER_LEFT)
    Serial.print(">L:RD:IN-");         // print the character
  else
    Serial.print(">R:RD:IN-");         // print the character
#endif

  int idx = 0;

  if (pFromSlave == SLAVE_NUMBER_LEFT)
    VUDataTotal[LEFT] = 0;        // Calculate the total for this channel while we're at it...
  else
    VUDataTotal[RIGHT] = 0;

  while (Wire.available()) {          // slave may send less than requested
    char c = Wire.read();             // receive a byte as character

    if (pFromSlave == SLAVE_NUMBER_LEFT) {
      VUData[LEFT][idx] = c - '0';
      VUDataTotal[LEFT] += VUData[LEFT][idx];
    }
    else {
      VUData[RIGHT][idx] = c - '0';
      VUDataTotal[RIGHT] += VUData[RIGHT][idx];
    }

#ifdef DEBUG_I2C
    Serial.print(VUData[pFromSlave][idx]);         // print the character
#endif

    ++idx;
  }

#ifdef DEBUG_I2C
  Serial.println("<RD:OUT");         // print the character
#endif
}
