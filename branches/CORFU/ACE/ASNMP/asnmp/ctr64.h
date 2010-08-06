/* -*-C++-*- */
#ifndef CTR64_
#define CTR64_
//=============================================================================
/**
 *  @file    ctr64.h
 *
 *  $Id$
 *
 *  SNMP Counter64 class definition.
 *
 *
 *  @author Peter E Mellquist
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

#include "asnmp/smival.h"
#include "ace/Basic_Types.h"

#define CTR64OUTBUF 30  // maximum ascii string for a 64-bit ctr


//---------[ 64 bit Counter Class ]--------------------------------
// Counter64 Class encapsulates two unsigned integers into a
// a single entity. This type has is available in SNMPv2 but
// may be used anywhere where needed.
//
/**
 * @class Counter64
 *
 * @brief Defines the member functions for the Counter64 concrete class
 * This class implements RFC 1902 64 bit Counter Object.
 */
class ASNMP_Export Counter64 : public  SnmpSyntax
{
public:
  /// default constructor
  Counter64( ACE_UINT64 llw = 0);

  /// constructor with values
  Counter64( unsigned long hiparm, unsigned long loparm);

  /// copy constructor
  Counter64( const Counter64 &ctr64);

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  ~Counter64();

  /// syntax type
  SmiUINT32 get_syntax();

  /// return a long double representation
  long double to_long_double() const;

  /// assign a long double to a counter64
  Counter64& assign( long double ld);

  /// return the high part
  unsigned long high() const;

  /// return the low part
  unsigned long low() const;

  /// set the high part
  void set_high( const unsigned long h);

  /// set the low part
  void set_low( const unsigned long l);

  /// overloaded assignment
  Counter64& operator=( const ACE_UINT64 rhs);

  /// overloaded assignment
  Counter64& operator=( const Counter64 &rhs);

  /// get a printable ASCII representation
  const char *to_string();

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// general validity test, always true
  int valid() const;

  /// otherwise, behave like an ACE_UINT64
  operator ACE_UINT64();

protected:
  char output_buffer[CTR64OUTBUF];
};

#endif  // CTR64_
