/* -*-C++-*- */
// $Id$
#ifndef GAUGE_
#define GAUGE_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    gauge32.cpp
//
// = DESCRIPTION
//  Class definition for SMI Gauge32 class.
//  This class currently does not behave as defined by the abstract model
//  in RFC 1155 section 3.2.3.4 (ie the value doesn't latch on the max value)
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

#include "asnmp/integer.h"

//------------[ Gauge32 Class ]------------------------------------------
// The gauge class allows all the functionality of unsigned
// integers but is recognized as a distinct SMI type. Gauge32
// objects may be set or get into Vb objects.
//

class ACE_Export Gauge32: public SnmpUInt32
  // = TITLE
  //     Define RFC1902 Gauge datatype from RFC 1155 section 3.2.3.4
  //     Note that this datatype does not behave exactly (acts like a long)
{
  public:
     Gauge32( const unsigned long i = 0);
     // constructor with a value

     Gauge32 ( const Gauge32 &g);
     // copy constructor

     ~Gauge32();
     // destructor for a Gauge32 (ensure that Value::~Value() is overridden)

     SmiUINT32 get_syntax();
     // syntax type

     SnmpSyntax *clone() const;
     // create a new instance of this Value

     Gauge32& operator=( const Gauge32 &uli);
     // overloaded assignment

     Gauge32& operator=( const unsigned long i);
     // overloaded assignment

     operator unsigned long();
     // otherwise, behave like an unsigned int

     SnmpSyntax& operator=( SnmpSyntax &val);
     // copy an instance of this Value
};
#endif //GAUGE_

