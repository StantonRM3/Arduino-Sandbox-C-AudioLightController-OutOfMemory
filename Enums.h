#ifndef _ENUMS_H
#define _ENUMS_H

#define MATRIX_HEIGHT       8
#define MATRIX_WIDTH       32

#define STRIP_LEDS        120

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// STATE MACHINE VARIABLES
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum {
  NOW_PLAYING = 0,          // 1)     NOW_PLAYING
  DISP,                     // 2)       DISPLAY
  DISP_SHOW_CLOCK,          // 2.1)       DISPLAY CLOCK
  DISP_PAT,                 // 2.2)       DISPLAY PATTERN
  DISP_PAT_VU,              // 2.2.1)       DISPLAY PATTERN [VU]
  DISP_PAT_VU_PEAK,         // 2.2.2)       DISPLAY PATTERN [VU PEAKS]
  DISP_PAT_VU_HUEBARS,      // 2.2.3)       DISPLAY PATTERN [VU HUEBARS]
  DISP_PAT_VU_PULSAR,       // 2.2.4)       DISPLAY PATTERN [VU PULSAR]
  DISP_PAT_VU_REVERSE,      // 2.2.5)       DISPLAY PATTERN [VU REVERSE]
  DISP_PAT_VU_SIDE,         // 2.2.6)       DISPLAY PATTERN [VU SIDEWAYS]
  DISP_PAT_VU_SIDEPULSE,    // 2.2.7)       DISPLAY PATTERN [VU SIDE PULSE]
  DISP_PAT_OSCILLOSCOPE,    // 2.2.8)       DISPLAY PATTERN [OSCILLOSCOPE]
  DISP_COL,                 // 2.3)       DISPLAY COLOUR
  DISP_COL_RED,             // 2.3.1)       DISPLAY COLOUR [RED]
  DISP_COL_ORANGE,          // 2.3.2)       DISPLAY COLOUR [ORANGE]
  DISP_COL_YELLOW,          // 2.3.3)       DISPLAY COLOUR [YELLOW]
  DISP_COL_GREEN,           // 2.3.4)       DISPLAY COLOUR [GREEN]
  DISP_COL_AQUA,            // 2.3.5)       DISPLAY COLOUR [AQUA]
  DISP_COL_BLUE,            // 2.3.6)       DISPLAY COLOUR [BLUE]
  DISP_COL_PURPLE,          // 2.3.7)       DISPLAY COLOUR [PURPLE]
  DISP_COL_PINK,            // 2.3.8)       DISPLAY COLOUR [PINK]
  DISP_COL_RANDOM,          // 2.3.9)       DISPLAY COLOUR [RANDOM]
  DISP_ONOFF,               // 2.4)       DISPLAY ON/OFF
  SET,                      // 3)       SETTINGS
  SET_TONE,                 // 3.1)       SETTINGS: TONE
  SET_TONE_BASS,            // 3.1.1)       SETTINGS: TONE: BASS
  SET_TONE_TREBLE,          // 3.1.2)       SETTINGS: TONE: TREBLE
} eState;

typedef enum {
  ON = 0,
  OFF
} eOnOff;

typedef enum {
  LEFT = 0,
  RIGHT
} eLeftRight;

typedef enum {
  UP = 1,
  DOWN = -1
} eUpDown;

#endif
