#ifndef OCTET_CLS_
#define OCTET_CLS_
//=============================================================================
/**
 *  @file    octet.h
 *
 * This class is fully contained and does not rely on or any other
 * SNMP libraries. This class is portable across any platform
 * which supports C++.
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
#include <memory>

/**
 * @class OctetStr
 *
 * @brief Implement RFC1155 Octet SMI data object
 */
class ASNMP_Export OctetStr : public  SnmpSyntax

{
public:
  /// construct octet from byte buffer, assume c style string if size == -1
  OctetStr (char const *string = "", long size = -1);

  /// constructor using another octet object
  OctetStr (OctetStr const &octet);
  OctetStr (OctetStr &&) = delete;

  ~OctetStr ();

  OctetStr &operator= (OctetStr &&) = delete;

  /// syntax type
  SmiUINT32 get_syntax() override;

  /// set octet from byte buffer, assume c style string if size == -1
  void set_data (SmiBYTE const *string, long size = -1);

  /// assignment to a string operator overloaded
  OctetStr &operator= (char const *string);

  /// assignment to another oid object overloaded
  OctetStr &operator= (OctetStr const &octet);

  /// equivlence operator overloaded
  friend ASNMP_Export bool operator== (OctetStr const &lhs, OctetStr const &rhs);

  /// not equivlence operator overloaded
  friend ASNMP_Export bool operator!= (OctetStr const &lhs, OctetStr const &rhs);

  /// less than < overloaded
  friend ASNMP_Export bool operator< (OctetStr const &lhs, OctetStr const &rhs);

  /// less than <= overloaded
  friend ASNMP_Export bool operator<= (OctetStr const &lhs, OctetStr const &rhs);

  /// greater than > overloaded
  friend ASNMP_Export bool operator> (OctetStr const &lhs, OctetStr const &rhs);

  /// greater than >= overloaded
  friend ASNMP_Export bool operator>= (OctetStr const &lhs, OctetStr const &rhs);

  /// equivlence operator overloaded
  friend ASNMP_Export bool operator== (OctetStr const &lhs, char const *rhs);

  /// not equivlence operator overloaded
  friend ASNMP_Export bool operator!= (OctetStr const &lhs, char const *rhs);

  /// less than < operator overloaded
  friend ASNMP_Export bool operator< (OctetStr const &lhs, char const *rhs);

  /// less than <= operator overloaded
  friend ASNMP_Export bool operator<= (OctetStr const &lhs, char *rhs);

  /// greater than > operator overloaded
  friend ASNMP_Export bool operator> (OctetStr const &lhs, char const *rhs);

  /// greater than >= operator overloaded
  friend ASNMP_Export bool operator>= (OctetStr const &lhs, char const *rhs);

  /// append operator, appends a string
  OctetStr &operator+= (char const *a);

  /// appends an int
  OctetStr &operator+= (char c);

  /// append one octetStr to another
  OctetStr &operator+= (OctetStr const &octetstr);

  /// for non const [], allows reading and writing
  SmiBYTE& operator[]( int position);

  /**
   * compare the n leftmost bytes (left-to-right)
   * returns 0, equal
   * returns -1, <
   * returns 1 , >
   */
  int left_comparison (long n, OctetStr const &o) const;

  /// return the len of the oid
  size_t length () const;

  /// returns validity
  int valid() const override;

  /// returns pointer to internal data
  SmiBYTE *data () const;

  /// get a printable ASCII value
  char const *to_string () override;

  /// get an ASCII formattted hex dump of the contents
  char const *to_string_hex ();

  /// create a new instance of this Value
  SnmpSyntax *clone () const override;

  /// copy an instance of this Value
  SnmpSyntax &operator= (SnmpSyntax &val) override;

protected:
  // The SmiVALUE C structure owns the octet array through its raw pointer.
  std::unique_ptr<char[]> output_buffer {};
  int validity {};
};

#endif // OCTET_CLS_
