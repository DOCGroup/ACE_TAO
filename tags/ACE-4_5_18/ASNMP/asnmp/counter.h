/* -*-C++-*- */
// $Id$
#ifndef COUNTER_
#define COUNTER_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    counter.h
//
// = DESCRIPTION
//  Class definition for SMI Counter32 class.
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
#include "asnmp/integer.h"

//------------[ Counter32 Class ]------------------------------------------
// The counter class allows all the functionality of unsigned
// 32 bit integers but is recognized as a distinct SMI type. Counter32
// class objects may be set or get into Vb objects.
//
// This class does not behave as defined by the abstract model
// in RFC 1155 section 3.2.3.3

class ACE_Export Counter32: public SnmpUInt32
  // = TITLE
  //     Defines the member functions for the Counter32 concrete class 
  //     This class implements RFC 1155 Counter Object and is named
  //     Counter32 after the RFC1902 redefinition of V1 SMI datatypes 
{
  public:
     Counter32( const unsigned long i = 0);
     // constructor with a value

     Counter32( const Counter32 &c);
     // copy constructor

     SmiUINT32 get_syntax();
     // syntax type

     SnmpSyntax *clone() const;
     // create a new instance of this Value

     SnmpSyntax& operator=( SnmpSyntax &val);
     // copy an instance of this Value

     Counter32& operator=( const Counter32 &uli);
     // overloaded assignment

     Counter32& operator=( const unsigned long i);
     // overloaded assignment

     operator unsigned long();
     // otherwise, behave like an unsigned long int
};

#endif // COUNTER_
