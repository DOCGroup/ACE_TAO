/* -*-C++-*- */
// $Id$
#ifndef CTR64_
#define CTR64_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    ctr64.h
//
// = DESCRIPTION
//  SNMP Counter64 class definition.
//
// = AUTHOR
//    Peter E Mellquist
//
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

#include "asnmp/smival.h"

#define CTR64OUTBUF 30  // maximum ascii string for a 64-bit ctr


//---------[ 64 bit Counter Class ]--------------------------------
// Counter64 Class encapsulates two unsigned integers into a
// a single entity. This type has is available in SNMPv2 but
// may be used anywhere where needed.
//
class ACE_Export Counter64: public  SnmpSyntax
  // = TITLE
  //     Defines the member functions for the Counter64 concrete class
  //     This class implements RFC 1902 64 bit Counter Object.
{
public:
  Counter64( unsigned long long llw = 0);
  // default constructor

  Counter64( unsigned long hiparm, unsigned long loparm);
  // constructor with values

  Counter64( const Counter64 &ctr64);
  // copy constructor

  ~Counter64();
  // destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)

  SmiUINT32 get_syntax();
  // syntax type

  long double to_long_double() const;
  // return a long double representation

  Counter64& assign( long double ld);
  // assign a long double to a counter64

  unsigned long high() const;
  // return the high part

  unsigned long low() const;
  // return the low part

  void set_high( const unsigned long h);
  // set the high part

  void set_low( const unsigned long l);
  // set the low part

  Counter64& operator=( const unsigned long long rhs);
  // overloaded assignment

  Counter64& operator=( const Counter64 &rhs);
  // overloaded assignment

  char *to_string();
  // get a printable ASCII representation

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

  int valid() const;
  // general validity test, always true

  operator unsigned long long();
  // otherwise, behave like an unsigned long long

protected:
  char output_buffer[CTR64OUTBUF];
};

#endif  // CTR64_
