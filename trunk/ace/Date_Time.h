/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ACE_Date_Time.h
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu) (and he's darn proud of
//      this ;-))
//      Well he shouldn't be, no const accessors, tsck, tsck, tsck ;-)
//
// ============================================================================

#ifndef ACE_DATE_TIME_H
#define ACE_DATE_TIME_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Date_Time
{
  // = TITLE
  //     System independent representation of date and time.
public:
  // constructor with init values, no check for validy
  ACE_Date_Time (long day = 0,
                 long month = 0,
                 long year = 0,
                 long hour = 0,
                 long minute = 0,
                 long second = 0,
                 long microsec = 0);
  // Set/get portions of ACE_Date_Time, no check for validity.

  long day (void) const;
  // Get day.

  void day (long day);
  // Set day.

  long month (void) const;
  // Get month.

  void month (long month);
  // Set month.

  long year (void) const;
  // Get year.

  void year (long year);
  // Set year.

  long hour (void) const;
  // Get hour.

  void hour (long hour);
  // Set hour.

  long minute (void) const;
  // Get minute.

  void minute (long minute);
  // Set minute.

  long second (void) const;
  // Get second.

  void second (long second);
  // Set second.

  long microsec (void) const;
  // Get microsec.

  void microsec (long microsec);
  // Set microsec.

private:
  long day_;
  long month_;
  long year_;
  long hour_;
  long minute_;
  long second_;
  long microsec_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Date_Time.i"
#endif

#include "ace/post.h"
#endif /* ACE_DATE_TIME_H */
