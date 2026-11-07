/* -*-C++-*- */
#ifndef SNMPINTEGER_
#define SNMPINTEGER_
//=============================================================================
/**
 *  @file    integer.h
 *
 *  Class definition for Integer classes convertable to SMI.
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

constexpr size_t INTOUTBUF = 3 * sizeof (long) + 2;

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
  explicit SnmpUInt32 (unsigned long const i = 0);

  /// copy constructor
  SnmpUInt32 (SnmpUInt32 const &c);
  SnmpUInt32 (SnmpUInt32 &&) = delete;

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  ~SnmpUInt32 () override = default;

  SnmpUInt32 &operator= (SnmpUInt32 &&) = delete;

  /// syntax type
  SmiUINT32 get_syntax() override;

  /// overloaded assignment
  SnmpUInt32 &operator= (unsigned long const i);

  /// overloaded assignment
  SnmpUInt32 &operator= (SnmpUInt32 const &uli);

  /// otherwise, behave like an unsigned long int
  operator unsigned long() const;

  /// get a printable ASCII value
  char const *to_string () override;

  /// create a new instance of this Value
  SnmpSyntax *clone() const override;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val) override;

  /// did object construct properly
  int valid() const override;

protected:
  /// contain string representation of object
  bool valid_flag {};
  char output_buffer[INTOUTBUF] {};
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
  explicit SnmpInt32 (long const i = 0);

  /// constructor with value
  SnmpInt32 (SnmpInt32 const &c);
  SnmpInt32 (SnmpInt32 &&) = delete;

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  ~SnmpInt32 () override = default;

  SnmpInt32 &operator= (SnmpInt32 &&) = delete;

  /// syntax type
  SmiUINT32 get_syntax() override;

  /// overloaded assignment
  SnmpInt32 &operator= (long const i);

  /// overloaded assignment
  SnmpInt32 &operator= (SnmpInt32 const &li);

  /// otherwise, behave like a long int
  operator long() const;

  /// create a new instance of this Value
  SnmpSyntax *clone() const override;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val) override;

  /// get a printable ASCII value
  char const *to_string () override;

  /// logical state of object
  int valid() const override;

protected:
  /// contain string representation of object
  bool valid_flag {};
  char output_buffer[INTOUTBUF] {};
};

#endif
