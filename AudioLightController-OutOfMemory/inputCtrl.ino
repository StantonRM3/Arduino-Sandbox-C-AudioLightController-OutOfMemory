//*********************************************************************************************************************************************************************************************************************************************
// inputCtrl
//
// File containing all the functionality to manage switches
//
//*********************************************************************************************************************************************************************************************************************************************
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Selector Buttons Setup...
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int BUTTON_SELECT = 22;
const int BUTTON_BACK = 24;
const int BUTTON_UP = 26;
const int BUTTON_DOWN = 28;

// Time to ignore any more button detections...
const int DEBOUNCE_TIME_MS = 200;

unsigned long lastBtnCheck = 0;

void inputCtrl_begin()
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("inputCtrl_begin()");
#endif

  pinMode(BUTTON_SELECT, INPUT);
  pinMode(BUTTON_BACK, INPUT);
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
}

// Passed as pointers to enable more than one button to be high at the same time i.e. SEL+UP...
void inputCtrl_checkButtons()
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("inputCtrl_checkButtons()");
#endif

  unsigned long currTime = millis();

  // We dont need to worry about buttons here as these are handled by the Timer1 interrupt.
  if ((currTime - lastBtnCheck) >= DEBOUNCE_TIME_MS)  {

    if (digitalRead(BUTTON_SELECT) == HIGH)
      stateMachine.stateSelected();

    if (digitalRead(BUTTON_BACK) == HIGH)
      stateMachine.backSelected();

    if (digitalRead(BUTTON_UP) == HIGH)
      stateMachine.upSelected();

    if (digitalRead(BUTTON_DOWN) == HIGH)
      stateMachine.downSelected();

    lastBtnCheck = currTime;
  }
}
