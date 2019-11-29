//*********************************************************************************************************************************************************************************************************************************************
// clsStateMachine.h
//
// Class containing all the functionality to manage system state machine
//
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef _CLSSTATEMACHINE_H
#define _CLSSTATEMACHINE_H

#include "Enums.h"
#include "clsMenuCtrl.h"

class clsStateMachine {

  public:

    eState currentState = NOW_PLAYING;
    eState patternMode = DISP_PAT_VU_PULSAR;
    eState colourMode = DISP_COL_PINK;
    eState nextSelectState;
    eState nextUpState;
    eState nextDnState;
    eOnOff displayMode = ON;
    eOnOff showClock = ON;

    void begin(clsColourCtrl *pClsColourCtrl, clsClock *pClsClock);
    void update(void);
    void display(void);
    void stateSelected(void);
    void backSelected(void);
    void upSelected(void);
    void downSelected(void);
    void refreshDisplay();
        
  private:

    // Pointer to the OLED display and Inputs device...
    clsMenuCtrl menuCtrl;
    clsColourCtrl *colourCtrl;
    clsClock *myClock;

};

#endif
