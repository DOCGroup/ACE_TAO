/* -*-C++-*- */
#ifndef GAUGE_
#define GAUGE_
//=============================================================================
/**
 *  @file    gauge.h
 *
 *  $Id$
 *
 *  Class definition for SMI Gauge32 class.
 *  This class currently does not behave as defined by the abstract model
 *  in RFC 1155 section 3.2.3.4 (ie the value doesn't latch on the max value)
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

#include "asnmp/integer.h"

//------------[ Gauge32 Class ]------------------------------------------
// The gauge class allows all the functionality of unsigned
// integers but is recognized as a distinct SMI type. Gauge32
// objects may be set or get into Vb objects.
//

/**
 * @class Gauge32:
 *
 * @brief Define RFC1902 Gauge datatype from RFC 1155 section 3.2.3.4
 * Note that this datatype does not behave exactly (acts like a long)
 */
class ASNMP_Export Gauge32: public SnmpUInt32
{
public:
  /// constructor with a value
  Gauge32( const unsigned long i = 0);

  /// copy constructor
  Gauge32 ( const Gauge32 &g);

  /// destructor for a Gauge32 (ensure that Value::~Value() is overridden)
  ~Gauge32();

  /// syntax type
  SmiUINT32 get_syntax();

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// overloaded assignment
  Gauge32& operator=( const Gauge32 &uli);

  /// overloaded assignment
  Gauge32& operator=( const unsigned long i);

  /// otherwise, behave like an unsigned int
  operator unsigned long();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);
};
#endif //GAUGE_
