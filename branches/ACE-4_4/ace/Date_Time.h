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
//    Tim Harrison (harrison@cs.wustl.edu) (and he's darn proud of this ;-))
//
// ============================================================================

#if !defined (ACE_DATE_TIME_H)
#define ACE_DATE_TIME_H

#include "ace/ACE.h"

class ACE_Export ACE_Date_Time
  // = TITLE
  //     System independent representation of date and time.
{
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

  long day (void);
  // Get day.

  void day (long day);
  // Set day.

  long month (void);
  // Get month.

  void month (long month);
  // Set month.

  long year (void);
  // Get year.

  void year (long year);
  // Set year.

  long hour (void);
  // Get hour.

  void hour (long hour);
  // Set hour.

  long minute (void);
  // Get minute.

  void minute (long minute);
  // Set minute.

  long second (void);
  // Get second.

  void second (long second);
  // Set second.

  long microsec (void);
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

#include "ace/Date_Time.i"

#endif /* ACE_DATE_TIME_H */
