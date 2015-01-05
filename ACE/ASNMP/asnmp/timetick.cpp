// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     timetick.cpp
//
// = DESCRIPTION
// Class implentation for SMI Timeticks class.
//
// = AUTHOR
//   Peter E Mellquist
//   Michael R MacFaden  mrm@cisco.com - rework & ACE port
// ============================================================================
/*===================================================================
  Copyright (c) 1996
  Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software
  and/or its documentation is hereby granted without fee. User agrees
  to display the above copyright notice and this license notice in all
  copies of the software and any documentation of the software. User
  agrees to assume all liability for the use of the software; Hewlett-Packard
  makes no representations about the suitability of this software for any
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.
=====================================================================*/

#include "asnmp/timetick.h"            // include header file for timetick class
#include "ace/OS_NS_stdio.h"

// constructor with a value
TimeTicks::TimeTicks( const unsigned long i):SnmpUInt32(i)
{
  smival.syntax = sNMP_SYNTAX_TIMETICKS;
}

// copy constructor
TimeTicks::TimeTicks( const TimeTicks &t)
  : SnmpUInt32 (t)
{
  smival.value.uNumber = t.smival.value.uNumber;
  smival.syntax = sNMP_SYNTAX_TIMETICKS;
}

// destructor
TimeTicks::~TimeTicks()
{
}

// syntax type
SmiUINT32 TimeTicks::get_syntax()
{
  return sNMP_SYNTAX_TIMETICKS;
}

// create a new instance of this Value
SnmpSyntax *TimeTicks::clone() const
{
  return (SnmpSyntax *) new TimeTicks(*this);
}

// overloaded assignement from ulong
TimeTicks& TimeTicks::operator=( const unsigned long int i)
{
  smival.value.uNumber =i; return *this;
}

// overloaded assignment from TimeTicks
TimeTicks& TimeTicks::operator=( const TimeTicks &uli)
{
 this->smival.value.uNumber = uli.smival.value.uNumber; return *this;
}

// general assignment from any Value
SnmpSyntax& TimeTicks::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignement from itself
      return *this;

  valid_flag = 0;               // will get set true if really valid
  if (in_val.valid()) {
    switch (in_val.get_syntax()) {
      case sNMP_SYNTAX_UINT32:
   // case sNMP_SYNTAX_GAUGE32:         .. indistinquishable from UINT32
      case sNMP_SYNTAX_CNTR32:
      case sNMP_SYNTAX_TIMETICKS:
      case sNMP_SYNTAX_INT32:           // implied cast int -> uint
          this->smival.value.uNumber =
                ((TimeTicks &)in_val).smival.value.uNumber;
          valid_flag = 1;
          break;
    }
  }
  return *this;
}

// otherwise, behave like an unsigned long
TimeTicks::operator unsigned long()
{
  return smival.value.uNumber;
}


// ASCII format return
const char * TimeTicks::to_string()
     /* Should do something nicer like days:hours:minutes... */
{
  unsigned long tt, hseconds, seconds, minutes, hours, days;
  tt = this->smival.value.uNumber;

  // days
  days = tt / 8640000;
  tt %= 8640000;

  // hours
  hours = tt / 360000;
  tt %= 360000;

  // minutes
  minutes = tt / 6000;
  tt %= 6000;

  seconds = tt / 100;
  tt %= 100;

  hseconds = tt;

  if ( days ==0)
    ACE_OS::sprintf( output_buffer,"%ld:%02ld:%02ld.%02ld", hours,
                     minutes,seconds,hseconds);
  else if ( days==1)
    ACE_OS::sprintf( output_buffer,"1 day %ld:%02ld:%02ld.%02ld", hours,
                     minutes,seconds,hseconds);
  else
    ACE_OS::sprintf( output_buffer,"%ld days, %ld:%02ld:%02ld.%02ld",
                     days,hours, minutes,seconds, hseconds);

  return output_buffer;
}
