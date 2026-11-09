
//=============================================================================
/**
 *  @file     ctr64.cpp
 *
 *  Implementation for Counter64 ( 64 bit counter class).
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

#include "asnmp/ctr64.h"
#include <cstdio>

static constexpr ACE_UINT64 counter64_base = 1ULL << 32;

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 Counter64::get_syntax()
{
  return sNMP_SYNTAX_CNTR64;
}

//------------------[ constructor with values ]--------------------------
Counter64::Counter64 (unsigned long hiparm, unsigned long loparm)
{
  smival.syntax = sNMP_SYNTAX_CNTR64;
  smival.value.hNumber.hipart = hiparm;
  smival.value.hNumber.lopart = loparm;
}

//------------------[ constructor with low value only ]------------------
Counter64::Counter64 (ACE_UINT64 llw)
{
  smival.syntax = sNMP_SYNTAX_CNTR64;
  smival.value.hNumber.hipart = static_cast<unsigned long> (llw >> 32);
  smival.value.hNumber.lopart = static_cast<unsigned long> (llw & 0xffffffffULL);
}

//------------------[ copy constructor ]---------------------------------
Counter64::Counter64 (Counter64 const &ctr64)
  : SnmpSyntax (ctr64)
{
  smival.syntax = sNMP_SYNTAX_CNTR64;
  smival.value.hNumber.hipart = ctr64.high();
  smival.value.hNumber.lopart = ctr64.low();
}

//------------------[ Counter64::high() ]------------------------------
// return the high part
unsigned long Counter64::high() const
{
  return smival.value.hNumber.hipart;
}

//------------------[ Counter64::low() ]-------------------------------
// return the low part
unsigned long Counter64::low() const
{
  return smival.value.hNumber.lopart;
}

//------------------[ set_high( const unsigned long h) ]-----------
// set the high part
void Counter64::set_high (unsigned long h)
{
  smival.value.hNumber.hipart = h;
}

//------------------[ set_low( const unsigned long l) ]------------
// set the low part
void Counter64::set_low (unsigned long l)
{
  smival.value.hNumber.lopart = l;
}

//-----------[ to_long_double( Counter64 c64) ]-----------------------------
// convert a Counter 64 to a long double
long double Counter64::to_long_double () const
{
  long double ld = this->high();
  ld *= counter64_base;
  ld += this->low();
  return ld;
}

//-----------[ ld_to_c64( long double ld) ]----------------------------
// convert a long double to a Counter64
// semantics changed from prior version
Counter64 &Counter64::assign (long double ld)
{
  smival.syntax = sNMP_SYNTAX_CNTR64;
  unsigned long const high_word = static_cast<unsigned long> (ld / counter64_base);
  smival.value.hNumber.hipart = high_word;
  smival.value.hNumber.lopart = static_cast<unsigned long> (
    ld - static_cast<long double> (high_word) * counter64_base);
  return *this;
}

//----------------[ general Value = operator ]---------------------
SnmpSyntax& Counter64::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  smival.value.hNumber.lopart = 0;      // pessimsitic - assume no mapping
  smival.value.hNumber.hipart = 0;

  // try to make assignment valid
  if (val.valid()){
    switch (val.get_syntax()){
      case sNMP_SYNTAX_CNTR64:
        smival.value.hNumber.hipart =
                ((Counter64 &)val).smival.value.hNumber.hipart;
        smival.value.hNumber.lopart =
                ((Counter64 &)val).smival.value.hNumber.lopart;
        break;

      case sNMP_SYNTAX_CNTR32:
      case sNMP_SYNTAX_TIMETICKS:
      case sNMP_SYNTAX_GAUGE32:
   // case sNMP_SYNTAX_UINT32:          .. indistinguishable from GAUGE32
      case sNMP_SYNTAX_INT32:
        // take advantage of union...
        smival.value.hNumber.lopart = ((Counter64 &)val).smival.value.uNumber;
        smival.value.hNumber.hipart = 0;
        break;
    }
  }
  return *this;
}

// overloaded assignment
Counter64 &Counter64::operator= (ACE_UINT64 rhs)
{
   smival.value.hNumber.hipart = static_cast<unsigned long> (rhs >> 32);
   smival.value.hNumber.lopart = static_cast<unsigned long> (rhs);
   return *this;
}

// overloaded assignment
Counter64 &Counter64::operator= (Counter64 const &rhs)
{
   smival.value.hNumber.hipart = rhs.high();
   smival.value.hNumber.lopart = rhs.low();
   return *this;
}

// otherwise, behave like an unsigned long int
Counter64::operator ACE_UINT64() const
{
  ACE_UINT64 val = static_cast<ACE_UINT64> (smival.value.hNumber.hipart);
  val = val << 32; // shift right 4 bytes
  val |= smival.value.hNumber.lopart;
  return val;
}

//----------------[ Counter64::clone() ]-----------------------------------
// create a new instance of this Value
SnmpSyntax* Counter64::clone() const
{
  return new Counter64(*this);
}

//----------------[ Counter64::valid() ]-------------------------------------
int Counter64::valid() const
{
  return 1;
}

//----------[ return ASCII format ]-------------------------
// TODO:  Fix up to do real 64bit decimal value printing...
//        For now, print > 32-bit values in hex
char const *Counter64::to_string ()
{
  if ( high() != 0 )
    std::snprintf (output_buffer, sizeof output_buffer, "0x%X%08X",
                    static_cast<unsigned int> (high()),
                    static_cast<unsigned int> (low()));
  else
    std::snprintf (output_buffer, sizeof output_buffer, "%lu", low());
  return output_buffer;
}
