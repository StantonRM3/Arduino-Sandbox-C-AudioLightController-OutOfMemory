//*********************************************************************************************************************************************************************************************************************************************
// clsPixelRingBuffer.h
//
// Class containing all the functionality to manage pixels direction, speed, etc for use with Ring Buffer
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSPIXELBUFFER_H
#define CLSPIXELBUFFER_H

#include "Enums.h"
#include "clsPixel.h"

#define MAX_PIXELS_BUFFERSIZE   64

class clsPixelBuffer
{
  public:

    clsPixel pixels[MAX_PIXELS_BUFFERSIZE];
    
    void begin(void);
    bool addPixel(int pPeakLevel, clsColourCtrl *pColourCtrl);
    void movePixels(void);
    
  private:

};

#endif
