//*********************************************************************************************************************************************************************************************************************************************
// clsMatrixCtrl.cpp
//
// Class containing all the functionality to manage Matrix
//
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"

#include "clsMatrixCtrl.h"

void clsMatrixCtrl::begin(clsVUData *pVUData, clsColourCtrl *pColourCtrl, clsClock *pMyClock)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMatrixCtrl::begin()");
#endif

  // Seed the randomNumber...
  randomSeed(analogRead(A0));

  // Set pointers to the source array for the VU data...
  //  left = pLeft;
  //  right = pRight;
  VUData = pVUData;

  colourCtrl = pColourCtrl;
  myClock = pMyClock;

  FastLED.addLeds<NEOPIXEL, MATRIX_DATA_PIN>(leds, MATRIX_LEDS);
  FastLED.setBrightness(colourCtrl->brightness);

  resetLeds();
  startUp();

  FastLED.show();
}

eState localPrevPattern = NOW_PLAYING;

void clsMatrixCtrl::displaySample(eOnOff pDisplayMode, eOnOff pShowClock, eState pPatternMode) {
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::displaySample()");
#endif

  if (pShowClock == ON) {
    showClock();
  }
  else {
    patternMode = pPatternMode;

    // Force LED reset if pattern has changed to prevent lit leds remaining until overwritten by some of the patterns...
    if (patternMode != localPrevPattern)
    {
      localPrevPattern = patternMode;
      resetLeds();
    }

    if (pDisplayMode == ON) {

      switch (patternMode) {
        case DISP_PAT_VU:               resetLeds();                          pattern_DISP_PAT_VU(LEFT);            pattern_DISP_PAT_VU(RIGHT);           break;
        case DISP_PAT_VU_PEAK:          resetLeds();                          pattern_DISP_PAT_VU_PEAK(LEFT);       pattern_DISP_PAT_VU_PEAK(RIGHT);      break;
        case DISP_PAT_VU_HUEBARS:       pattern_DISP_PAT_VU_HUEBARS(LEFT);    pattern_DISP_PAT_VU_HUEBARS(RIGHT);   break;
        case DISP_PAT_VU_PULSAR:        pattern_DISP_PAT_VU_PULSAR(LEFT);     pattern_DISP_PAT_VU_PULSAR(RIGHT);    break;
        case DISP_PAT_VU_REVERSE:       resetLeds();                          pattern_DISP_PAT_VU_REVERSE(LEFT);    pattern_DISP_PAT_VU_REVERSE(RIGHT);   break;
        case DISP_PAT_VU_SIDE:          pattern_DISP_PAT_VU_SIDE(LEFT);       pattern_DISP_PAT_VU_SIDE(RIGHT);      break;
        case DISP_PAT_VU_SIDEPULSE:     resetLeds();                          pattern_DISP_PAT_VU_SIDEPULSE(LEFT);  pattern_DISP_PAT_VU_SIDEPULSE(RIGHT); break;
        case DISP_PAT_OSCILLOSCOPE:     pattern_DISP_PAT_OSCILLOSCOPE();      break;
      }
    }
  }

  FastLED.show();
}

void clsMatrixCtrl::pattern_DISP_PAT_VU(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU()");
#endif

  int displayCol;

  for (int col = 0; col < (MATRIX_WIDTH / 2); col++)
  {
    // Smooth the downwards column...
    if (VUData->VUData[pLeftRight][col] < VUData->VUData_LastMax[pLeftRight][col])
      VUData->VUData[pLeftRight][col] = VUData->VUData_LastMax[pLeftRight][col] - 1;

    if (pLeftRight == LEFT)
      displayCol = col;
    else
      displayCol = (MATRIX_WIDTH - 1) - col;

    for (int row = 0; (row < VUData->VUData[pLeftRight][col]) && (row < MATRIX_HEIGHT); row++) {
      if (row >= 6)
        setPixelVal(row, displayCol, CRGB(255, 0, 0));
      else if (row >= 4)
        setPixelVal(row, displayCol, CRGB(255, 255, 0));
      else
        setPixelVal(row, displayCol, CRGB(0, 255, 0));
    }

    VUData->VUData_LastMax[pLeftRight][col] = VUData->VUData[pLeftRight][col];
  }
}

void clsMatrixCtrl::pattern_DISP_PAT_VU_PEAK(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_PEAK()");
#endif

  int displayCol;

  for (int col = 0; col < (MATRIX_WIDTH / 2); col++)
  {
    // Smooth the downwards column...
    if (VUData->VUData[pLeftRight][col] < VUData->VUData_LastMax[pLeftRight][col])
      VUData->VUData[pLeftRight][col] = VUData->VUData_LastMax[pLeftRight][col] - 1;

    if (pLeftRight == LEFT)
      displayCol = col;
    else
      displayCol = (MATRIX_WIDTH - 1) - col;

    if (VUData->VUData[pLeftRight][col] >= 6)
      setPixelVal(VUData->VUData[pLeftRight][col], displayCol, CRGB(255, 0, 0));
    else if (VUData->VUData[pLeftRight][col] >= 4)
      setPixelVal(VUData->VUData[pLeftRight][col], displayCol, CRGB(255, 255, 0));
    else if (VUData->VUData[pLeftRight][col] > 0)
      setPixelVal(VUData->VUData[pLeftRight][col], displayCol, CRGB(0, 255, 0));
    else
      setPixelVal(VUData->VUData[pLeftRight][col], displayCol, CRGB(0, 0, 64));

    VUData->VUData_LastMax[pLeftRight][col] = VUData->VUData[pLeftRight][col];
  }
}

void clsMatrixCtrl::pattern_DISP_PAT_VU_HUEBARS(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_HUEBARS()");
#endif

  int displayCol;

  CRGB pixel;

  for (int col = 0; col < (MATRIX_WIDTH / 2); col++)
  {
    if (pLeftRight == LEFT)
      displayCol = col;
    else
      displayCol = (MATRIX_WIDTH - 1) - col;

    // Smooth the downwards column...
    // If the current level is less than previous level we need to take one off the top of the column...
    if (VUData->VUData[pLeftRight][col] < VUData->VUData_LastMax[pLeftRight][col])
    {
      VUData->VUData[pLeftRight][col] = VUData->VUData_LastMax[pLeftRight][col] - 1;
      setPixelVal(VUData->VUData_LastMax[pLeftRight][col], displayCol, CHSV(0, 0, 0));
    }
    // Otherwise we need to add to the top of the column....
    else if (VUData->VUData[pLeftRight][col] > VUData->VUData_LastMax[pLeftRight][col])
    {
      pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];

      for (int row = VUData->VUData_LastMax[pLeftRight][col]; (row < VUData->VUData[pLeftRight][col]) && (row < MATRIX_HEIGHT); row++) {
        setPixelVal(row, displayCol, pixel);
      }
    }
    // Otherwise check if its zero, in which case blank it...
    else if (VUData->VUData[pLeftRight][col] == 0)
      setPixelVal(0, displayCol, CHSV(0, 0, 0));

    VUData->VUData_LastMax[pLeftRight][col] = VUData->VUData[pLeftRight][col];
  }
}

void clsMatrixCtrl::pattern_DISP_PAT_VU_PULSAR(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_PULSAR()");
#endif

  int displayCol;

  CRGB pixel;

  for (int col = 0; col < (MATRIX_WIDTH / 2); col++)
  {
    if (pLeftRight == LEFT)
      displayCol = (MATRIX_WIDTH / 2) - col - 1;
    else
      displayCol = (MATRIX_WIDTH / 2) + col;

    // Smooth the downwards column...
    // If the current level is less than previous level we need to take one off the top of the column...
    if (VUData->VUData[pLeftRight][col] < VUData->VUData_LastMax[pLeftRight][col])
    {
      VUData->VUData[pLeftRight][col] = VUData->VUData_LastMax[pLeftRight][col] - 1;
      setPixelVal(VUData->VUData_LastMax[pLeftRight][col], displayCol, CHSV(0, 0, 0));
    }
    // Otherwise we need to add to the top of the column....
    else if (VUData->VUData[pLeftRight][col] > VUData->VUData_LastMax[pLeftRight][col])
    {
      pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];

      for (int row = VUData->VUData_LastMax[pLeftRight][col]; (row < VUData->VUData[pLeftRight][col]) && (row < MATRIX_HEIGHT); row++) {
        setPixelVal(row, displayCol, pixel);
      }
    }
    // Otherwise check if its zero, in which case blank it...
    else if (VUData->VUData[pLeftRight][col] == 0)
      setPixelVal(0, displayCol, CHSV(0, 0, 0));

    VUData->VUData_LastMax[pLeftRight][col] = VUData->VUData[pLeftRight][col];
  }
}

void clsMatrixCtrl::pattern_DISP_PAT_VU_REVERSE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_REVERSE()");
#endif

  int displayCol;

  CRGB pixel;

  for (int col = 0; col < (MATRIX_WIDTH / 2); col++)
  {
    if (pLeftRight == LEFT)
      displayCol = col;
    else
      displayCol = (MATRIX_WIDTH - 1) - col;

    // Smooth the downwards column...
    if (VUData->VUData[pLeftRight][col] < VUData->VUData_LastMax[pLeftRight][col]) {
      VUData->VUData[pLeftRight][col] = VUData->VUData_LastMax[pLeftRight][col] - 1;
    }

    pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];
    for (int row = VUData->VUData[pLeftRight][col]; (row < MATRIX_HEIGHT); row++) {

      setPixelVal(row, displayCol, pixel);
    }

    VUData->VUData_LastMax[pLeftRight][col] = VUData->VUData[pLeftRight][col];
  }
}

int localPeakStore[2][MATRIX_HEIGHT];

void clsMatrixCtrl::pattern_DISP_PAT_VU_SIDE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_SIDE()");
#endif

  int displayCol;

  CRGB pixel;

  int colWidth = 0;

  for (int row = 0; row < MATRIX_HEIGHT; row++) {

    // Here we will double up the samples as the number of rows = half the number of columns.  Should do with with maths really, but for testing...
    colWidth = VUData->VUData[pLeftRight][row * 2] + VUData->VUData[pLeftRight][(row * 2) + 1];

    // Smooth the downwards row...
    if (colWidth < localPeakStore[pLeftRight][row])
      colWidth = localPeakStore[pLeftRight][row] - 1;

    // If the current level is less than previous level we need to take one off the top of the column...
    if (colWidth < localPeakStore[pLeftRight][row])
    {
      if (pLeftRight == LEFT)
        setPixelVal(row, colWidth, CHSV(0, 0, 0));
      else
        setPixelVal(row, (MATRIX_WIDTH - 1 - colWidth), CHSV(0, 0, 0));
    }
    else
    {
      if (colWidth > localPeakStore[pLeftRight][row]) {
        pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];

        for (int dispCol = localPeakStore[pLeftRight][row]; dispCol < colWidth; dispCol++) {
          if (pLeftRight == LEFT)
            setPixelVal(row, dispCol, pixel);
          else
            setPixelVal(row, (MATRIX_WIDTH - 1 - dispCol), pixel);
        }
      }
    }

    localPeakStore[pLeftRight][row] = colWidth;
  }
}

int lastDisplayTotal[2] = {0, 0};

void clsMatrixCtrl::pattern_DISP_PAT_VU_SIDEPULSE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_VU_SIDEPULSE()");
#endif

  int *useTotal;
  int displayCol;

  CRGB pixel;

  int total = VUData->VUDataTotal[pLeftRight];

  if (VUData->VUDataTotal[pLeftRight] < VUData->VUDataTotal_LastMax[pLeftRight])
    total = VUData->VUDataTotal_LastMax[pLeftRight] - 1;

  VUData->VUDataTotal_LastMax[pLeftRight] = total;

  // Exagerate the display...
  total = total * 1.5;
  if (total < lastDisplayTotal[pLeftRight])
    total = lastDisplayTotal[pLeftRight] - 1;
  lastDisplayTotal[pLeftRight] = total;

  int colIdx = 0;

  for (int i = 0; i < total; i++) {
    // pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];
    if (colIdx >= (MAX_RANDOM_COLOUR_PALLET - 1))
      colIdx = MAX_RANDOM_COLOUR_PALLET - 1;
    else colIdx = i;

    pixel = colourCtrl->colourPallette[colIdx];

    if (pLeftRight == LEFT)
      leds[i] = pixel;
    else
      leds[MATRIX_LEDS - i - 1] = pixel;
  }

  *useTotal = total;
}

void clsMatrixCtrl::pattern_DISP_PAT_OSCILLOSCOPE(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::pattern_DISP_PAT_OSCILLOSCOPE()");
#endif

  int displayCol;
  eLeftRight pLeftRight = LEFT;      // Leave this here for the time being I may want to reverse the Oscilloscope...

  // Move the columns to the right and fill the first column with new data...
  shiftColumns(pLeftRight == LEFT ? RIGHT : LEFT);

  CRGB pixel;

  if (pLeftRight == LEFT)
    displayCol = 0;
  else
    displayCol = (MATRIX_WIDTH - 1);

  int mappedActVal = map(VUData->VUDataTotal[pLeftRight], 0, 32, 0, 7);

  for (int i = 0; i < MATRIX_HEIGHT; i++) {

    if (mappedActVal == i && VUData->VUDataTotal[pLeftRight] != 0)
    {
      pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];
      setPixelVal(i, displayCol, pixel);
    }
    else
      setPixelVal(i, displayCol, CHSV(0, 0, 0));
  }
}

inline void clsMatrixCtrl::setPixelVal(int pRow, int pCol, CRGB pPixel) {
  int idx = 0;
  idx = pRow + (pCol * MATRIX_HEIGHT);
  leds[idx] = pPixel;
}

inline CRGB clsMatrixCtrl::getPixelVal(int pRow, int pCol) {
  int idx = 0;
  idx = pRow + (pCol * MATRIX_HEIGHT);
  return leds[idx];
}

void clsMatrixCtrl::resetLeds() {
  for (int i = 0; i < MATRIX_LEDS; i++)
    leds[i] = CHSV(0, 0, 0);
}

void clsMatrixCtrl::shiftColumns(eLeftRight pLeftRight) {

  if (pLeftRight == RIGHT)
  {
    for (int i = (MATRIX_WIDTH - 1); i > 0; i--)
    {
      for (int j = 0; j < MATRIX_HEIGHT; j++)
        setPixelVal(j, i, getPixelVal(j, i - 1));
    }
  }
  else
  {
    for (int i = 0; i < (MATRIX_WIDTH - 1); i++)
    {
      for (int j = 0; j < MATRIX_HEIGHT; j++)
        setPixelVal(j, i, getPixelVal(j, i + 1));
    }
  }
}

#define SCROLLSPEED 50

void clsMatrixCtrl::startUp(void)
{
  CRGB pixelCol = CRGB(128, 128, 128);
  CRGB bgPixelCol = CRGB(0, 0, 0);

  // Blank the Matrix ...
  for (int col = 0; col < MATRIX_WIDTH; col++) {
    for (int row = 0; row < MATRIX_HEIGHT; row++)
      setPixelVal(row, col, bgPixelCol);
  }

  int bitRow;
  int bitByte;
  int bitElement;

  // Now scroll the rest in...
  for (int col = 0; col < SIRENLOGO_WIDTH; col++) {
    FastLED.show();
    delay(SCROLLSPEED);
    clsMatrixCtrl::shiftColumns(LEFT);

    // And fill the next column from the left...
    for (int row = 0; row < MATRIX_HEIGHT; row++) {

      // SORRY - WISH I COULD SIMPLIFY IT AND KEEP THE LOGO BITMAP CONSTRUCTION EASY...
      // NOTE: HAD TO CHOP UP sirenOne INTO 16 BIT CHUNKS FOR bitRead TO WORK PROPERLY...
      bitRow = (MATRIX_HEIGHT - 1) - row;   // Reverse the rows in the logo bitmap, i.e. row 0 in bitmap = row 7 in matrix (effectively upside down)...
      bitByte = (col / 16);                 // Which byte of the bitmap, i.e. take the integer diviser (if col < 16 then byte = 0, 32 < cols > 15 then byte = 1, etc...
      bitElement = 15 - (col % 16);         // This reverses the bits extracted from the respective bitByte element and uses the remainder of the division to give us the actual element...

      // Some real hacking to get the sirenONE bits in the right order
      if (bitRead(sirenOne[bitRow][bitByte], bitElement))
        setPixelVal(row, MATRIX_WIDTH - 1, pixelCol);
      else
        setPixelVal(row, MATRIX_WIDTH - 1, bgPixelCol);

    }
  }

  // Go back to main programme...
  for (int col = 0; col < MATRIX_WIDTH; col++) {
    FastLED.show();
    delay(SCROLLSPEED);
    clsMatrixCtrl::shiftColumns(LEFT);
  }

}

void clsMatrixCtrl::showClock(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsMatrixCtrl::showClock()");
#endif

  CRGB pixel;

  // Define the values to look for in the clockMatrix[8][32]...
  int HourToLookFor = myClock->Hour + 200;
  int MinuteToLookFor = myClock->Minute + 100;

  // Go through the clockMatrix row by row, column, by column and check if we need to turn the LED's on/off...
  for (int row = 0; row < 8; row ++ )
  {
    for (int column = 0; column < 32; column ++)
    {
      // Turn LED off, we will turn it on if needed...
      // NOTE: The clockMatrix is 0,0 top-left, whereas the matrix is 0,0 bottom-left.  Need to translate this...
      setPixelVal((7 - row), column, CHSV(0, 0, 0));

      // If the matrix contains a value...
      if ( clockMatrix[row][column] > 0)
      {
        // If the value is < 200 its a minute value...
        if ( clockMatrix[row][column] < 199 )
        {
          // So we need to turn this LED on if the value <= to the current Minute value...
          if (clockMatrix[row][column] <= MinuteToLookFor)
          {
            // And lets highlight every fifth minute (ala a real clock)...
            int remainder = clockMatrix[row][column] % 5;

            // NOTE: The clockMatrix is 0,0 top-left, whereas the matrix is 0,0 bottom-left.  Need to translate this...
            if (remainder == 0)
              pixel = CRGB(64, 0, 0);
            else
              pixel = colourCtrl->colourPallette[clockMatrix[row][column]-100];

              setPixelVal((7 - row), column, pixel);
          }
        }
        // Otherwise its an hour value...
        else
        {
          // So we need to turn this LED on if the value == to the current Hour value...
          if (clockMatrix[row][column] == HourToLookFor)
          {
            // NOTE: The clockMatrix is 0,0 top-left, whereas the matrix is 0,0 bottom-left.  Need to translate this...
            setPixelVal((7 - row), column, CRGB(64, 0, 0));
          }
        }
      }
    }
  }
}
