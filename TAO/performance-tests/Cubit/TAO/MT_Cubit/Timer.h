/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Timer.h
 *
 *  $Id$ */
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


#ifndef _MT_CUBIT_TIMER_H
#define _MT_CUBIT_TIMER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"

/**
 * @class MT_Cubit_Timer
 *
 * @brief A class that encapsulates the pccTimer for chorus and uses
 * ACE Timer for other platforms.
 */
class MT_Cubit_Timer
{
public:
  MT_Cubit_Timer (u_int granularity);

  void start (void);
  void stop (void);

  ACE_timer_t get_elapsed (void);

private:
  /// timer.
  ACE_High_Res_Timer timer_;

  /// Elapsed time in microseconds.
  ACE_Time_Value delta_;

  /**
   * This is the granularity of the timing of the CORBA requests. A
   * value of 5 represents that we will take time every 5 requests,
   * instead of the default of every request (1).
   */
  u_int granularity_;
};

#endif /* _MT_CUBIT_TIMER_H */
