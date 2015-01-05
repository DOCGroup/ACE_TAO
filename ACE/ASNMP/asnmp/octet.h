

#ifndef OCTET_CLS_
#define OCTET_CLS_
//=============================================================================
/**
 *  @file    octet.h
 *
 *  $Id$
 *
 * This class is fully contained and does not rely on or any other
 * SNMP libraries. This class is portable across any platform
 * which supports C++.
 *
 *
 *  @author Peter E Mellquist  / design/original code Michael R. MacFaden / modified for ACE
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

//------------------------------------------------------------------------
#include "asnmp/smival.h"

/**
 * @class OctetStr
 *
 * @brief Implement RFC1155 Octet SMI data object
 */
class ASNMP_Export OctetStr : public  SnmpSyntax

{
public:
  /// construct octet from byte buffer, assume c style string if size == -1
  OctetStr( const char *string = "", long size = -1);

  /// constructor using another octet object
  OctetStr ( const OctetStr &octet);

  /// destructor
  ~OctetStr();

  /// syntax type
  SmiUINT32 get_syntax();

  /// set octet from byte buffer, assume c style string if size == -1
  void set_data( const SmiBYTE* string, long int size = -1);

  /// assignment to a string operator overloaded
  OctetStr& operator=( const char *string);

  /// assignment to another oid object overloaded
  OctetStr& operator=( const OctetStr &octet);

  /// equivlence operator overloaded
  friend ASNMP_Export bool operator==( const OctetStr &lhs, const OctetStr &rhs);

  /// not equivlence operator overloaded
  friend ASNMP_Export bool operator!=( const OctetStr &lhs, const OctetStr &rhs);

  /// less than < overloaded
  friend ASNMP_Export bool operator<( const OctetStr &lhs, const OctetStr &rhs);

  /// less than <= overloaded
  friend ASNMP_Export bool operator<=( const OctetStr &lhs,const OctetStr &rhs);

  /// greater than > overloaded
  friend ASNMP_Export bool operator>( const OctetStr &lhs, const OctetStr &rhs);

  /// greater than >= overloaded
  friend ASNMP_Export bool operator>=( const OctetStr &lhs, const OctetStr &rhs);

  /// equivlence operator overloaded
  friend ASNMP_Export bool operator==( const OctetStr &lhs,const char *rhs);

  /// not equivlence operator overloaded
  friend ASNMP_Export bool operator!=( const OctetStr &lhs,const char  *rhs);

  /// less than < operator overloaded
  friend ASNMP_Export bool operator<( const OctetStr &lhs,const char  *rhs);

  /// less than <= operator overloaded
  friend ASNMP_Export bool operator<=( const OctetStr &lhs,char  *rhs);

  /// greater than > operator overloaded
  friend ASNMP_Export bool operator>( const OctetStr &lhs,const char  *rhs);

  /// greater than >= operator overloaded
  friend ASNMP_Export bool operator>=( const OctetStr &lhs,const char  *rhs);

  /// append operator, appends a string
  OctetStr& operator+=( const char  *a);

  /// appends an int
  OctetStr& operator+=( const char c);

  /// append one octetStr to another
  OctetStr& operator+=( const OctetStr& octetstr);

  /// for non const [], allows reading and writing
  SmiBYTE& operator[]( int position);

  /**
   * compare the n leftmost bytes (left-to-right)
   * returns 0, equal
   * returns -1, <
   * returns 1 , >
   */
  int left_comparison( const long n, const OctetStr &o) const;

  /// return the len of the oid
  size_t length() const ;

  /// returns validity
  int valid() const;

  /// returns pointer to internal data
  SmiBYTE *data() const;

  /// get a printable ASCII value
  const char *to_string();

  /// get an ASCII formattted hex dump of the contents
  const char *to_string_hex();

  /// create a new instance of this Value
  SnmpSyntax  *clone() const;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

protected:
  //----[ instance variables ]
  char *output_buffer;     // formatted Octet value
  int validity;                // validity boolean
};

#endif // OCTET_CLS_
