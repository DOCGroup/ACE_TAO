/* $Id$ */

/* _*_C++_*_ */


#if !defined (_MT_CUBIT_TIMER_H)
#define _MT_CUBIT_TIMER_H

#include "Task_Client.h"

class MT_Cubit_Timer
{
  // = TITLE
  //     A class that encapsulates the pccTimer for chorus and uses
  //     ACE Timer for other platforms.
public:
  MT_Cubit_Timer (Task_State *ts);
  void start (void);
  void stop (void);
  ACE_timer_t get_elapsed (void);
private:
#if defined (CHORUS)
  int pstartTime_;
  int pstopTime_;
  // variables for the pccTimer.
#endif
  
  ACE_High_Res_Timer timer_;
  // timer.
  
  ACE_Time_Value delta_;
  // Elapsed time in microseconds.
  
  Task_State *ts_;
  // task state.
};

#endif
