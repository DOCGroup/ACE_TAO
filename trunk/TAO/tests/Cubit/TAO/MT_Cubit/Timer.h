/* -*- C++ -*- */
/* $Id$ */

#if !defined (_MT_CUBIT_TIMER_H)
#define _MT_CUBIT_TIMER_H

#include "Task_Client.h"

class MT_Cubit_Timer
{
  // = TITLE
  //     A class that encapsulates the pccTimer for chorus and uses
  //     ACE Timer for other platforms.
public:
  MT_Cubit_Timer (u_int granularity);

  void start (void);
  void stop (void);

  ACE_timer_t get_elapsed (void);

private:
  ACE_High_Res_Timer timer_;
  // timer.
  
  ACE_Time_Value delta_;
  // Elapsed time in microseconds.
  
  u_int granularity_;
  // This is the granularity of the timing of the CORBA requests. A
  // value of 5 represents that we will take time every 5 requests,
  // instead of the default of every request (1).

#if defined (CHORUS)
  // Variables for the pccTimer.
  int pstartTime_;
  int pstopTime_;
#endif /* CHORUS */
};

#endif /* _MT_CUBIT_TIMER_H */
