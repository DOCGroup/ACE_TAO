/* -*-C++-*- */
#ifndef SNMPINTEGER_
#define SNMPINTEGER_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    integer.cpp
//
// = DESCRIPTION
//  Class definition for Integer classes convertable to SMI.
//
// = AUTHOR
//  Jeff Meyer
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

// TODO get rid of this constant
#define INTOUTBUF 15  // largest ASCII formatted integer

//------------[ Integer Classes ]------------------------------------------
// The integer class allows all the functionality of the various
// integers but is contained in a Value object for consistency
// among the various types.
// class objects may be set or get into Vb objects.
// 

// 32 bit unsigned integer class
class ACE_Export SnmpUInt32: public SnmpSyntax
  // = TITLE
  //      Implement RFC 1920 Unsigned Integer SMI datatype 
{
     
  public: 
     SnmpUInt32 (const unsigned long i = 0);
     // default constructor 

     SnmpUInt32( const SnmpUInt32 &c);
     // copy constructor
        
     virtual ~SnmpUInt32();
     // destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)

     virtual SmiUINT32 get_syntax();
     // syntax type

     SnmpUInt32& operator=( const unsigned long i); 
     // overloaded assignment   
        
     SnmpUInt32& operator=( const SnmpUInt32 &uli); 
     // overloaded assignment   
        
     operator unsigned long();
     // otherwise, behave like an unsigned long int    

     virtual char *to_string();
     // get a printable ASCII value

     virtual SnmpSyntax *clone() const;
     // create a new instance of this Value

     SnmpSyntax& operator=( SnmpSyntax &val);
     // copy an instance of this Value

     int valid() const;
     // did object construct properly

  protected:
    int valid_flag;
    char output_buffer[INTOUTBUF];
    // contain string representation of object
}; 


// 32 bit signed integer class
class ACE_Export SnmpInt32: public SnmpSyntax
{
  // = TITLE
  //      Implement RFC 1902 32 bit Integer SMI data object 
  public:
     SnmpInt32 (const long i = 0);
     // constructor with value

     SnmpInt32 (const SnmpInt32 &c);
     // constructor with value

     virtual ~SnmpInt32();
     // destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)

     virtual SmiUINT32 get_syntax();
     // syntax type

     SnmpInt32& operator=( const long i); 
     // overloaded assignment   
        
     SnmpInt32& operator=( const SnmpInt32 &li); 
     // overloaded assignment   
        
     operator long();
     // otherwise, behave like a long int    

     SnmpSyntax *clone() const;
     // create a new instance of this Value

     SnmpSyntax& operator=( SnmpSyntax &val);
     // copy an instance of this Value

     char *to_string();
     // get a printable ASCII value

     int valid() const;
     // logical state of object 

 protected:
    int valid_flag;
    char output_buffer[INTOUTBUF];
    // contain string representation of object  
};

#endif       
