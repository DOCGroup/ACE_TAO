/* -*-C++-*- */
// $Id$
#ifndef OCTET_CLS_
#define OCTET_CLS_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    octet.h
//
// = DESCRIPTION
// This class is fully contained and does not rely on or any other
// SNMP libraries. This class is portable across any platform
// which supports C++.
//
// = AUTHOR
//    Peter E Mellquist  / design/original code
//    Michael R. MacFaden / modified for ACE, added suboid(),collapse ctor's
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

//------------------------------------------------------------------------
#include "asnmp/smival.h"

class ACE_Export OctetStr: public  SnmpSyntax
  // = TITLE
  //      Implement RFC1155 Octet SMI data object

{
public:
  OctetStr( const char *string = "", long size = -1);
  // construct octet from byte buffer, assume c style string if size == -1

  OctetStr ( const OctetStr &octet);
  // constructor using another octet object

  ~OctetStr();
  // destructor

  SmiUINT32 get_syntax();
  // syntax type

  void set_data( const SmiBYTE* string, long int size = -1);
  // set octet from byte buffer, assume c style string if size == -1

  OctetStr& operator=( const char *string);
  // assignment to a string operator overloaded

  OctetStr& operator=( const OctetStr &octet);
  // assignment to another oid object overloaded

  friend int operator==( const OctetStr &lhs, const OctetStr &rhs);
  // equivlence operator overloaded

  friend int operator!=( const OctetStr &lhs, const OctetStr &rhs);
  // not equivlence operator overloaded

  friend int operator<( const OctetStr &lhs, const OctetStr &rhs);
  // less than < overloaded

  friend int operator<=( const OctetStr &lhs,const OctetStr &rhs);
  // less than <= overloaded

  friend int operator>( const OctetStr &lhs, const OctetStr &rhs);
  // greater than > overloaded

  friend int operator>=( const OctetStr &lhs, const OctetStr &rhs);
  // greater than >= overloaded

  friend int operator==( const OctetStr &lhs,const char *rhs);
  // equivlence operator overloaded

  friend int operator!=( const OctetStr &lhs,const char  *rhs);
  // not equivlence operator overloaded

  friend int operator<( const OctetStr &lhs,const char  *rhs);
  // less than < operator overloaded

  friend int operator<=( const OctetStr &lhs,char  *rhs);
  // less than <= operator overloaded

  friend int operator>( const OctetStr &lhs,const char  *rhs);
  // greater than > operator overloaded

  friend int operator>=( const OctetStr &lhs,const char  *rhs);
  // greater than >= operator overloaded

  OctetStr& operator+=( const char  *a);
  // append operator, appends a string

  OctetStr& operator+=( const char c);
  // appends an int

  OctetStr& operator+=( const OctetStr& octetstr);
  // append one octetStr to another

  SmiBYTE& operator[]( int position);
  // for non const [], allows reading and writing

  int left_comparison( const long n, const OctetStr &o) const;
  // compare the n leftmost bytes (left-to-right)
  // returns 0, equal
  // returns -1, <
  // returns 1 , >

  size_t length() const ;
  // return the len of the oid

  int valid() const;
  // returns validity

  SmiBYTE *data() const;
  // returns pointer to internal data

  char *to_string();
  // get a printable ASCII value

  char *to_string_hex();
  // get an ASCII formattted hex dump of the contents

  SnmpSyntax  *clone() const;
  // create a new instance of this Value

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

protected:
  //----[ instance variables ]
  char *output_buffer;     // formatted Octet value
  int validity;                // validity boolean
};

#endif // OCTET_CLS_
