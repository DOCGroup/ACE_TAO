// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     integer.cpp
//
// = DESCRIPTION
//   Class implemtation for SMI Integer classes.
//
// = AUTHOR
//   Jeff Meyer
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

#include "asnmp/integer.h"        // header file for gauge class
#include "ace/OS_NS_stdio.h"

// constructor with value
SnmpUInt32::SnmpUInt32 (const unsigned long i)
{
  smival.value.uNumber=i;
  smival.syntax = sNMP_SYNTAX_UINT32;
  valid_flag = 1;
}

// copy constructor
SnmpUInt32::SnmpUInt32( const SnmpUInt32 &c)
  : SnmpSyntax (c)
{
  smival.value.uNumber=c.smival.value.uNumber;
  smival.syntax = sNMP_SYNTAX_UINT32;
  valid_flag = 1;
}

// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
SnmpUInt32::~SnmpUInt32()
{
}

// syntax type
SmiUINT32 SnmpUInt32::get_syntax()
{
 return sNMP_SYNTAX_UINT32;
}

// object validity
int SnmpUInt32::valid( void) const
{
 return valid_flag;
}

// overloaded assignment
SnmpUInt32& SnmpUInt32::operator=( const unsigned long int i)
{
  smival.value.uNumber=i;
  valid_flag = 1;
  return *this;
}

// general assignment from any Value
SnmpSyntax& SnmpUInt32::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignement from itself
      return *this;

  valid_flag = 0;               // will get set true if really valid
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
          valid_flag = 1;
          break;
    }
  }
  return *this;
}

// overloaded assignment
SnmpUInt32& SnmpUInt32::operator=( const SnmpUInt32 &uli)
{
  this->smival.value.uNumber = uli.smival.value.uNumber; return *this;
}

// otherwise, behave like an unsigned long int
SnmpUInt32::operator unsigned long()
{
  return smival.value.uNumber;
}

// create a new instance of this Value
SnmpSyntax* SnmpUInt32::clone() const
{
  return (SnmpSyntax *) new SnmpUInt32(*this);
}

// ASCII format return
const char * SnmpUInt32::to_string()
{
  ACE_OS::sprintf(output_buffer, "%d", (int) (this->smival.value.uNumber));
  return output_buffer;
}


//====================================================================
//  INT 32 Implementation
//====================================================================

// default constructor
SnmpInt32::SnmpInt32 (const long i)
{
  smival.value.sNumber=i;
  smival.syntax = sNMP_SYNTAX_INT32;
  valid_flag = 1;
}

// constructor with value
SnmpInt32::SnmpInt32 (const SnmpInt32 &c)
  : SnmpSyntax (c)
{
  smival.value.sNumber=c.smival.value.sNumber;
  smival.syntax = sNMP_SYNTAX_INT32;
  valid_flag = 1;
}

// destructor
SnmpInt32::~SnmpInt32()
{
}

// syntax type
SmiUINT32 SnmpInt32::get_syntax()
{
 return sNMP_SYNTAX_INT32;
}

// object validity
int SnmpInt32::valid( void) const
{
  return valid_flag;
}

// overloaded assignment
SnmpInt32& SnmpInt32::operator=( const long i)
{
  this->smival.value.sNumber = (unsigned long) i;
  valid_flag = 1;
  return *this;
}

// overloaded assignment
SnmpInt32& SnmpInt32::operator=( const SnmpInt32 &uli)
{
  this->smival.value.sNumber = uli.smival.value.sNumber;
  valid_flag = 1;
  return *this;
}

// general assignment from any Value
SnmpSyntax& SnmpInt32::operator=( SnmpSyntax &in_val)
{
  if ( this == &in_val )        // handle assignement from itself
      return *this;

  valid_flag = 0;               // will get set true if really valid
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
          valid_flag = 1;
          break;
    }
  }
  return *this;
}

// otherwise, behave like a long int
SnmpInt32::operator long()
{
  return (long) smival.value.sNumber;
}

// clone - create a new instance of this Value
SnmpSyntax* SnmpInt32::clone() const
{
  return ( SnmpSyntax *) new SnmpInt32(*this);
}

// ASCII format return

const char *SnmpInt32::to_string()
{
  ACE_OS::sprintf(output_buffer, "%d", (int) (long) this->smival.value.sNumber);
  return output_buffer;
}
