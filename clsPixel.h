//*********************************************************************************************************************************************************************************************************************************************
// clsPixel.h
//
// Class containing all the functionality to remember pixel direction, speed, etc for use with Ring Buffer
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSPIXEL_H
#define CLSPIXEL_H

#include "Enums.h"

#include "clsColourCtrl.h"

class clsPixel
{
  public:

    void begin(int pPeakLevel, clsColourCtrl *pColourCtrl);
    bool pixelStep(void);

    int pixelPos;                 // Current pixel position...
    bool pixelDead = false;       // Check if the pixel is dead or not.
    CRGB colour;                  // Select random colour for this from the provided pColourCtrl
    int width = 0;                // Derive width from peakLevel
     int mySpeed = 0;                // Derive speed from peakLevel
        
  private:

    clsColourCtrl *colourCtrl;
    int peakLevel = 0;            // Max Level of Pixel
    eUpDown myDirection = UP;
};

#endif
