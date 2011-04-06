
//=============================================================================
/**
 *  @file     gauge.cpp
 *
 *  $Id$
 *
 * Class implemtation for SMI Gauge32 class.
 * NOTES: This does not behave exactly as a Gauge described in RFC 1155
 *
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

#include "asnmp/gauge.h"          // header file for gauge class

// constructor with a value
Gauge32::Gauge32( const unsigned long i):SnmpUInt32(i)
{
  smival.syntax = sNMP_SYNTAX_GAUGE32;
}

// copy constructor
Gauge32::Gauge32 ( const Gauge32 &g)
  : SnmpUInt32 (g)
{ this->smival.value.uNumber = g.smival.value.uNumber;
     smival.syntax = sNMP_SYNTAX_GAUGE32;
     valid_flag = 1;
}

// destructor
Gauge32::~Gauge32()
{
}

// syntax type
SmiUINT32 Gauge32::get_syntax()
{
  return sNMP_SYNTAX_GAUGE32;
}

// overloaded assignment
Gauge32& Gauge32::operator=( const Gauge32 &uli)
{
  this->smival.value.uNumber = uli.smival.value.uNumber;
     return *this;
}

// overloaded assignment
Gauge32& Gauge32::operator=( const unsigned long int i)
{
  smival.value.uNumber=i; return *this;
}

// general assignment from any Value
// TODO: this is broken if not inherited from UInt32 (see UInt32 code).
SnmpSyntax& Gauge32::operator=( SnmpSyntax &in_val)
{

  if ( this == &in_val )        // handle assignement from itself
      return *this;

  valid_flag = 0;               // will get set true if really valid
  if (in_val.valid())
  {
    switch (in_val.get_syntax())
    {
      case sNMP_SYNTAX_UINT32:
      case sNMP_SYNTAX_GAUGE32:
      case sNMP_SYNTAX_CNTR32:
      case sNMP_SYNTAX_TIMETICKS:
      case sNMP_SYNTAX_INT32:           // implied cast int -> uint
          this->smival.value.uNumber =
                ((Gauge32 &)in_val).smival.value.uNumber;
          valid_flag = 1;
          break;
    }
  }
  return *this;
}

// otherwise, act as unsigned long
Gauge32::operator unsigned long()
{
  return smival.value.uNumber;
}

// clone - create a new instance of this Value
SnmpSyntax* Gauge32::clone() const
{
  return ( SnmpSyntax *) new Gauge32(*this);
}
