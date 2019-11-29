// HEADER TO MANAGE THE DEBUGGER FLAGS

#ifndef _DEBUG_H
#define _DEBUG_H

// COMMENT FOLLOWING LINE OUT TO TURN OFF ALL DEBUG MESSAGES
// ALSO COMMENT OUT #include "Debug.h" IN SPECIFIC CLASS HEADER TO REMOVE THAT CLASS FROM DEBUGING IF REQUIRED..
#define DEBUG                           TRUE

#ifdef DEBUG

#define DEBUG_TRACE_PGM_ROUTE           TRUE              // Show all function calls
// #define DEBUG_TRACE_PGM_ROUTE_HIFREQ    TRUE              // Show all function calls (Including the Hi Frequency function calls.  This may obscure the information in Serial Monitor
// #define DEBUG_I2C                       TRUE              // Set to see the transmission data between the VU analysers and Master....

#endif

#define SERIAL_BAUD                     19200             // 9600, 19200, 38400, 57600, 74880, 115200, 230400, 250000, 500000, 1000000, 2000000

#endif
