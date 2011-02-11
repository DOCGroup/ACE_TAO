/* -*- C++ -*- */
/* $Id$ */

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Timer.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, Sergio Flores-Gaitan and Nagarajan
//    Surendran.
//
// ============================================================================

#ifndef _MT_CUBIT_TIMER_H
#define _MT_CUBIT_TIMER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"

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
};

#endif /* _MT_CUBIT_TIMER_H */
