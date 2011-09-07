/* -*-C++-*- */
#ifndef SNMPINTEGER_
#define SNMPINTEGER_
//=============================================================================
/**
 *  @file    integer.h
 *
 *  $Id$
 *
 *  Class definition for Integer classes convertable to SMI.
 *
 *
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

// TODO get rid of this constant
#define INTOUTBUF 15  // largest ASCII formatted integer

//------------[ Integer Classes ]------------------------------------------
// The integer class allows all the functionality of the various
// integers but is contained in a Value object for consistency
// among the various types.
// class objects may be set or get into Vb objects.
//

// 32 bit unsigned integer class
/**
 * @class SnmpUInt32
 *
 * @brief Implement RFC 1920 Unsigned Integer SMI datatype
 */
class ASNMP_Export SnmpUInt32 : public SnmpSyntax
{
public:
  /// default constructor
  SnmpUInt32 (const unsigned long i = 0);

  /// copy constructor
  SnmpUInt32( const SnmpUInt32 &c);

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  virtual ~SnmpUInt32();

  /// syntax type
  virtual SmiUINT32 get_syntax();

  /// overloaded assignment
  SnmpUInt32& operator=( const unsigned long i);

  /// overloaded assignment
  SnmpUInt32& operator=( const SnmpUInt32 &uli);

  /// otherwise, behave like an unsigned long int
  operator unsigned long();

  /// get a printable ASCII value
  virtual const char *to_string();

  /// create a new instance of this Value
  virtual SnmpSyntax *clone() const;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// did object construct properly
  int valid() const;

protected:
  /// contain string representation of object
  int valid_flag;
  char output_buffer[INTOUTBUF];
};


// 32 bit signed integer class
  /**
   * @class SnmpInt32
   *
   * @brief Implement RFC 1902 32 bit Integer SMI data object
   */
class ASNMP_Export SnmpInt32 : public SnmpSyntax
{
public:
  /// constructor with value
  SnmpInt32 (const long i = 0);

  /// constructor with value
  SnmpInt32 (const SnmpInt32 &c);

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  virtual ~SnmpInt32();

  /// syntax type
  virtual SmiUINT32 get_syntax();

  /// overloaded assignment
  SnmpInt32& operator=( const long i);

  /// overloaded assignment
  SnmpInt32& operator=( const SnmpInt32 &li);

  /// otherwise, behave like a long int
  operator long();

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// get a printable ASCII value
  const char *to_string();

  /// logical state of object
  int valid() const;

rotected:
  /// contain string representation of object
  int valid_flag;
  char output_buffer[INTOUTBUF];
};

#endif
