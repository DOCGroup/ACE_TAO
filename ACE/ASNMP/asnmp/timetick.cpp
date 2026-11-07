
//=============================================================================
/**
 *  @file     timetick.cpp
 *
 * Class implentation for SMI Timeticks class.
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
 */
//=============================================================================

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
#include <cstdio>

// constructor with a value
TimeTicks::TimeTicks (unsigned long const i)
  : SnmpUInt32 (i)
{
  smival.syntax = sNMP_SYNTAX_TIMETICKS;
}

// copy constructor
TimeTicks::TimeTicks (TimeTicks const &t)
  : SnmpUInt32 (t)
{
  smival.value.uNumber = t.smival.value.uNumber;
  smival.syntax = sNMP_SYNTAX_TIMETICKS;
}

// syntax type
SmiUINT32 TimeTicks::get_syntax()
{
  return sNMP_SYNTAX_TIMETICKS;
}

// create a new instance of this Value
SnmpSyntax *TimeTicks::clone() const
{
  return new TimeTicks (*this);
}

// overloaded assignment from ulong
TimeTicks &TimeTicks::operator= (unsigned long const i)
{
  smival.value.uNumber = i;
  return *this;
}

// overloaded assignment from TimeTicks
TimeTicks &TimeTicks::operator= (TimeTicks const &uli)
{
  this->smival.value.uNumber = uli.smival.value.uNumber;
  return *this;
}

// general assignment from any Value
SnmpSyntax& TimeTicks::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignment from itself
      return *this;

  valid_flag = false;
  if (in_val.valid()) {
    switch (in_val.get_syntax()) {
      case sNMP_SYNTAX_UINT32:
   // case sNMP_SYNTAX_GAUGE32:         .. indistinquishable from UINT32
      case sNMP_SYNTAX_CNTR32:
      case sNMP_SYNTAX_TIMETICKS:
      case sNMP_SYNTAX_INT32:           // implied cast int -> uint
          this->smival.value.uNumber =
                ((TimeTicks &)in_val).smival.value.uNumber;
          valid_flag = true;
          break;
    }
  }
  return *this;
}

// otherwise, behave like an unsigned long
TimeTicks::operator unsigned long() const
{
  return smival.value.uNumber;
}

// ASCII format return
char const *TimeTicks::to_string ()
  /* Should do something nicer like days:hours:minutes... */
{
  unsigned long remaining = smival.value.uNumber;

  // days
  unsigned long const days = remaining / 8640000;
  remaining %= 8640000;

  // hours
  unsigned long const hours = remaining / 360000;
  remaining %= 360000;

  // minutes
  unsigned long const minutes = remaining / 6000;
  remaining %= 6000;

  unsigned long const seconds = remaining / 100;
  remaining %= 100;

  unsigned long const hseconds = remaining;

  if (days == 0)
    std::snprintf (output_buffer, sizeof output_buffer, "%lu:%02lu:%02lu.%02lu",
                   hours, minutes, seconds, hseconds);
  else if (days == 1)
    std::snprintf (output_buffer, sizeof output_buffer, "1 day %lu:%02lu:%02lu.%02lu",
                   hours, minutes, seconds, hseconds);
  else
    std::snprintf (output_buffer, sizeof output_buffer, "%lu days, %lu:%02lu:%02lu.%02lu",
                   days, hours, minutes, seconds, hseconds);

  return output_buffer;
}
