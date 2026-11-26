#ifndef OID_CLS_
#define OID_CLS_
/**
 *  @file    oid.h
 *
 * This class is fully contained and does not rely on or any other
 * SNMP libraries. This class is portable across any platform
 * which supports C++. Ported to ACE by Mike MacFaden mrm@cisco.com
 * and modified for MT. Some API cleanup: collapse constructors,
 * add suboid() method remove unnecessary overload, and update a few
 * names and types to improve clarity.
 *
 *  @author Peter E Mellquist
 */

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

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "asnmp/smival.h"                // derived class for all values
#include <memory>

/**
 * @class Oid
 *
 * @brief Implement RFC 1155 Object Identifier (OID) datatype
 */
class ASNMP_Export Oid : public  SnmpSyntax
{
public:
  Oid (char const * dotted_oid_string = "", size_t size = unsigned(-1));

  Oid ( Oid const &oid);
  Oid (Oid &&oid) noexcept;

  Oid(unsigned long const *raw_oid, size_t oid_len);

  ~Oid();

  /// syntax type
  SmiUINT32 get_syntax() override;

  /// assignment to another oid object overloaded
  Oid& operator=( Oid const &oid);
  Oid &operator= (Oid &&oid) noexcept;

  /// assignment from a dotted string
  Oid& operator= (char const *dotted_oid_string);

  /// equal operator overloaded
  friend ASNMP_Export bool operator==( Oid const &lhs,Oid const &rhs);

  /// not equal operator overloaded
  friend ASNMP_Export bool operator!=( Oid const &lhs,Oid const &rhs);

  /// less than < overloaded
  friend ASNMP_Export bool operator<( Oid const &lhs,Oid const &rhs);

  /// less than <= overloaded
  friend ASNMP_Export bool operator<=( Oid const &lhs,Oid const &rhs);

  /// greater than > overloaded
  friend ASNMP_Export bool operator>( Oid const &lhs,Oid const &rhs);

  /// greater than >= overloaded
  friend ASNMP_Export bool operator>=( Oid const &lhs,Oid const &rhs);

  /// append operator, appends a string
  Oid& operator+=( char const *a);

  /// appends an int
  Oid& operator+=( const unsigned long i);

  /// appends an Oid
  Oid& operator+=( Oid const &o);

  /// allows element access
  unsigned long & operator[]( size_t position);

  /// return the WinSnmp oid part
  SmiLPOID oidval();

  /// reset the data from raw 1 byte format
  void set_data( char const *dotted_oid_string);

  /// reset the data from raw 4byte integer format
  void set_data( unsigned long const *raw_oid, const size_t oid_len);

  /// return the len of the oid
  size_t length() const;

  /// trim off the n rightmost values of an oid
  void trim( const size_t how_many = 1);

  /// the equivalent of a substr(new_str, 0, 1)
  int suboid(Oid& oid, size_t start = 0, size_t how_many = size_t (-1));

  /**
   * compare the n leftmost bytes (left-to-right)
   * returns 0, equal
   * returns -1, <
   * returns 1 , >
   */
  int left_comparison( const unsigned long n, Oid const &o) const;

  /**
   * compare the n rightmost bytes (right-to-left)
   * returns 0, equal
   * returns -1, <
   * returns 1 , >
   */
  int right_comparison( const unsigned long n, Oid const &o) const;

  /// is the Oid object valid
  int valid() const override;

  /// return dotted string value from the right
  /// where the user specifies how many positions to print
  char const *to_string() override;

  /// create a new instance of this Value
  SnmpSyntax *clone() const override;

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val) override;

protected:
  /// used for returning oid string
  std::unique_ptr<char[]> iv_str {};

  /// convert a string to an smi oid
  int StrToOid( char const *string, SmiLPOID dstOid, size_t& how_many_bytes);

  /// clone an smi oid
  int OidCopy(  SmiLPOID srcOid, SmiLPOID dstOid, size_t& how_many_bytes);

  /// convert an smi oid to its string representation
  int OidToStr(SmiLPOID srcOid, SmiUINT32 size, char *string,  size_t& how_many_bytes);

  private:
    /// release memory, re-init data members
    void set_invalid();

    /// initialize object data members
    void set_null();
    void init_value( const SmiLPOID srcOid, size_t len);
    void init_value(unsigned long const *raw_oid, size_t oid_len);
};

#endif //OID_CLS_
