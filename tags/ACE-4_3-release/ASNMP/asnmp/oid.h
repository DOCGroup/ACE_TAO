/* -*-C++-*- */
#ifndef OID_CLS_
#define OID_CLS_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    oid.h 
//
// = DESCRIPTION
// This class is fully contained and does not rely on or any other
// SNMP libraries. This class is portable across any platform
// which supports C++. Ported to ACE by Mike MacFaden mrm@cisco.com 
// and modified for MT. Some API cleanup: collapse constructors,
// add suboid() method remove unnecessary overload, and update a few 
// names and types to improve clarity. 
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

//----------[ extern C libraries Needed ]---------------------------------
#include "ace/OS.h"
#include "asnmp/smival.h"                // derived class for all values

class ACE_Export Oid: public  SnmpSyntax 
  // = TITLE
  //      Implement RFC 1155 Object Identifier (OID) datatype 
{
public:
  Oid( const char * dotted_oid_string = "", size_t size = -1);
  // constructor using a dotted string

  Oid ( const Oid &oid);
  // constructor using another oid object

  Oid(const unsigned long *raw_oid, size_t oid_len);
  // constructor from raw form

  ~Oid();
  // destructor

  SmiUINT32 get_syntax();
  // syntax type

  Oid& operator=( const Oid &oid);
  // assignment to another oid object overloaded

  friend int operator==( const Oid &lhs,const Oid &rhs);
  // equal operator overloaded

  friend int operator!=( const Oid &lhs,const Oid &rhs);
  // not equal operator overloaded

  friend int operator<( const Oid &lhs,const Oid &rhs);
  // less than < overloaded

  friend int operator<=( const Oid &lhs,const Oid &rhs);
  // less than <= overloaded

  friend int operator>( const Oid &lhs,const Oid &rhs);
  // greater than > overloaded

  friend int operator>=( const Oid &lhs,const Oid &rhs);
  // greater than >= overloaded

  Oid& operator+=( const char *a);
  // append operator, appends a string

  Oid& operator+=( const unsigned long i);
  // appends an int

  Oid& operator+=( const Oid &o);
  // appends an Oid

  unsigned long & operator[]( size_t position);
  // allows element access
  
  SmiLPOID oidval();
  // return the WinSnmp oid part

  void set_data( const char *dotted_oid_string);
  // reset the data from raw 1 byte format

  void set_data( const unsigned long *raw_oid, const size_t oid_len);
  // reset the data from raw 4byte integer format
 
  size_t length() const;
  // return the len of the oid

  void trim( const size_t how_many = 1);
  // trim off the n rightmost values of an oid

  int suboid(Oid& oid, size_t start = 0, size_t how_many = -1);
  // the equivalent of a substr(new_str, 0, 1) 

  int left_comparison( const unsigned long n, const Oid &o) const; 
  // compare the n leftmost bytes (left-to-right)
  // returns 0, equal
  // returns -1, <
  // returns 1 , >

  
  int right_comparison( const unsigned long n, const Oid &o) const; 
  // compare the n rightmost bytes (right-to-left)
  // returns 0, equal
  // returns -1, <
  // returns 1 , >

  int valid() const;
  // is the Oid object valid

  char *to_string(); 
  // return dotted string value from the right
  // where the user specifies how many positions to print

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

protected:
  char *iv_str;             
  // used for returning oid string

  int StrToOid( const char *string, SmiLPOID dstOid, size_t& how_many_bytes);
  // convert a string to an smi oid

  int OidCopy(  SmiLPOID srcOid, SmiLPOID dstOid, size_t& how_many_bytes);
  // clone an smi oid

  int OidToStr(SmiLPOID srcOid, SmiUINT32 size, char *string,  size_t& how_many_bytes);
  // convert an smi oid to its string representation

  private:
    void set_invalid();  
    // release memory, re-init data members 

    void set_null();  
    // initialize object data members
    void init_value( const SmiLPOID srcOid, size_t len);
    void init_value(const unsigned long *raw_oid, size_t oid_len);
};

#endif //OID_CLS_
