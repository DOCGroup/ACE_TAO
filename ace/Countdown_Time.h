// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Value.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_COUNTDOWN_TIME_H
#define ACE_COUNTDOWN_TIME_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_HAS_WINCE) && !defined (ACE_PSOS_DIAB_MIPS)
#   include "ace/os_include/sys/os_time.h"
# endif /* ACE_HAS_WINCE ACE_PSOS_DIAB_MIPS */

#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"

#if defined (ACE_WIN32) && defined (_WIN32_WCE)
// Something is a bit brain-damaged here and I'm not sure what... this code
// compiled before the OS reorg for ACE 5.4. Since then it hasn't - eVC
// complains that the operators that return ACE_Time_Value are C-linkage
// functions that can't return a C++ class. The only way I've found to
// defeat this is to wrap the whole class in extern "C++".
//    - Steve Huston, 23-Aug-2004
extern "C++" {
#endif

/**
 * @class ACE_Countdown_Time
 *
 * @brief Keeps track of the amount of elapsed time.
 *
 * This class has a side-effect on the <max_wait_time> -- every
 * time the <stop> method is called the <max_wait_time> is
 * updated.
 */
class ACE_Export ACE_Countdown_Time
{
public:
  // = Initialization and termination methods.
  /// Cache the <max_wait_time> and call <start>.
  ACE_Countdown_Time (ACE_Time_Value *max_wait_time);

  /// Call <stop>.
  ~ACE_Countdown_Time (void);

  /// Cache the current time and enter a start state.
  int start (void);

  /// Subtract the elapsed time from max_wait_time_ and enter a stopped
  /// state.
  int stop (void);

  /// Calls stop and then start.  max_wait_time_ is modified by the
  /// call to stop.
  int update (void);

  /// Returns 1 if we've already been stopped, else 0.
  int stopped (void) const;

private:
  /// Maximum time we were willing to wait.
  ACE_Time_Value *max_wait_time_;

  /// Beginning of the start time.
  ACE_Time_Value start_time_;

  /// Keeps track of whether we've already been stopped.
  int stopped_;

  // Prevent copying
  ACE_Countdown_Time (const ACE_Countdown_Time &);
  ACE_Countdown_Time &operator= (const ACE_Countdown_Time &);
};

#if defined (ACE_WIN32) && defined (_WIN32_WCE)
}
#endif

#include /**/ "ace/post.h"

#endif /* ACE_COUNTDOWN_TIME_H */
