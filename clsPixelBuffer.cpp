//*********************************************************************************************************************************************************************************************************************************************
// clsPixelRingBuffer.cpp
//
// Class containing ring buffer to store and manage dynamic pixels
//
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"

#include "clsPixelBuffer.h"

void clsPixelBuffer::begin(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsPixelBuffer::begin()");
#endif

  // Force all the pixels as dead to enable them to be loaded...
  for (int i = 0; i < MAX_PIXELS_BUFFERSIZE; i++)
    pixels[i].pixelDead = true;

}

bool clsPixelBuffer::addPixel(int pPeakLevel, clsColourCtrl *pColourCtrl)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsPixelBuffer::addPixel()");
#endif

  bool retval = false;

  // If theres space in the buffer, add pixel to the buffer...
  for (int i = 0; i < MAX_PIXELS_BUFFERSIZE; i++)
  {
    // We have found a slot...
    if (pixels[i].pixelDead == true)
    {
      pixels[i].begin(pPeakLevel, pColourCtrl);
      return true;
      break;
    }
  }

  return retval;
}

void clsPixelBuffer::movePixels(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsPixelBuffer::movePixels()");
#endif

  for (int i = 0; i < MAX_PIXELS_BUFFERSIZE; i++) {
    if (pixels[i].pixelDead == false) {
      pixels[i].pixelStep();
    }
  }
}
