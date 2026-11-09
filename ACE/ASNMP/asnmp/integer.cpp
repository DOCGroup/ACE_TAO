
//=============================================================================
/**
 *  @file     integer.cpp
 *
 * Class implemtation for SMI Integer classes.
 *
 *  @author Jeff MeyerMichael R MacFaden  mrm@cisco.com - rework & ACE port
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

#include "asnmp/integer.h"        // header file for gauge class
#include <cstdio>

// constructor with value
SnmpUInt32::SnmpUInt32 (unsigned long const i)
{
  smival.value.uNumber = i;
  smival.syntax = sNMP_SYNTAX_UINT32;
  valid_flag = true;
}

// copy constructor
SnmpUInt32::SnmpUInt32 (SnmpUInt32 const &c)
  : SnmpSyntax (c)
{
  smival.value.uNumber = c.smival.value.uNumber;
  smival.syntax = sNMP_SYNTAX_UINT32;
  valid_flag = true;
}

// syntax type
SmiUINT32 SnmpUInt32::get_syntax()
{
 return sNMP_SYNTAX_UINT32;
}

// object validity
int SnmpUInt32::valid() const
{
 return valid_flag;
}

// overloaded assignment
SnmpUInt32 &SnmpUInt32::operator= (unsigned long const i)
{
  smival.value.uNumber = i;
  valid_flag = true;
  return *this;
}

// general assignment from any Value
SnmpSyntax& SnmpUInt32::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignment from itself
      return *this;

  valid_flag = false;
  if (in_val.valid())
  {
    switch (in_val.get_syntax())
    {
      case sNMP_SYNTAX_UINT32:
   // case sNMP_SYNTAX_GAUGE32:         .. indistinquishable from UINT32
      case sNMP_SYNTAX_CNTR32:
      case sNMP_SYNTAX_TIMETICKS:
      case sNMP_SYNTAX_INT32:           // implied cast int -> uint
          this->smival.value.uNumber =
              ((SnmpUInt32 &)in_val).smival.value.uNumber;
          valid_flag = true;
          break;
    }
  }
  return *this;
}

// overloaded assignment
SnmpUInt32 &SnmpUInt32::operator= (SnmpUInt32 const &uli)
{
  this->smival.value.uNumber = uli.smival.value.uNumber;
  return *this;
}

// otherwise, behave like an unsigned long int
SnmpUInt32::operator unsigned long() const
{
  return smival.value.uNumber;
}

// create a new instance of this Value
SnmpSyntax* SnmpUInt32::clone() const
{
  return new SnmpUInt32 (*this);
}

// ASCII format return
char const *SnmpUInt32::to_string ()
{
  std::snprintf (output_buffer, sizeof output_buffer, "%lu", smival.value.uNumber);
  return output_buffer;
}

//====================================================================
//  INT 32 Implementation
//====================================================================

SnmpInt32::SnmpInt32 (long const i)
{
  smival.value.sNumber = i;
  smival.syntax = sNMP_SYNTAX_INT32;
  valid_flag = true;
}

// constructor with value
SnmpInt32::SnmpInt32 (SnmpInt32 const &c)
  : SnmpSyntax (c)
{
  smival.value.sNumber = c.smival.value.sNumber;
  smival.syntax = sNMP_SYNTAX_INT32;
  valid_flag = true;
}

// syntax type
SmiUINT32 SnmpInt32::get_syntax()
{
 return sNMP_SYNTAX_INT32;
}

// object validity
int SnmpInt32::valid() const
{
  return valid_flag;
}

// overloaded assignment
SnmpInt32 &SnmpInt32::operator= (long const i)
{
  this->smival.value.sNumber = i;
  valid_flag = true;
  return *this;
}

// overloaded assignment
SnmpInt32 &SnmpInt32::operator= (SnmpInt32 const &uli)
{
  this->smival.value.sNumber = uli.smival.value.sNumber;
  valid_flag = true;
  return *this;
}

// general assignment from any Value
SnmpSyntax& SnmpInt32::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignment from itself
      return *this;

  valid_flag = false;
  if (in_val.valid())
  {
    switch (in_val.get_syntax())
    {
      case sNMP_SYNTAX_INT32:
      case sNMP_SYNTAX_UINT32:          // implied cast uint -> int
   // case sNMP_SYNTAX_GAUGE32:         .. indistinquishable from UINT32
      case sNMP_SYNTAX_CNTR32:          // implied cast uint -> int
      case sNMP_SYNTAX_TIMETICKS:       // implied cast uint -> int
          this->smival.value.sNumber =
                ((SnmpInt32 &)in_val).smival.value.sNumber;
          valid_flag = true;
          break;
    }
  }
  return *this;
}

// otherwise, behave like a long int
SnmpInt32::operator long() const
{
  return smival.value.sNumber;
}

// clone - create a new instance of this Value
SnmpSyntax* SnmpInt32::clone() const
{
  return new SnmpInt32 (*this);
}

// ASCII format return

char const *SnmpInt32::to_string ()
{
  std::snprintf (output_buffer, sizeof output_buffer, "%ld", smival.value.sNumber);
  return output_buffer;
}
