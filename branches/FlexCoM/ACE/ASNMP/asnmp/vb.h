

#ifndef VB_CLS_
#define VB_CLS_
//=============================================================================
/**
 *  @file     vb.h
 *
 *  $Id$
 *
 * This module contains the class definition for the variable binding (VB)
 * class. The VB class is an encapsulation of a SNMP VB. A VB object is
 * composed of one SNMP++ Oid and one SMI value. The Vb class utilizes Oid
 * objects and thus requires the Oid class. To use this class,
 * set oid, value then call valid() to be sure object was constructed correctly.
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

//----[ external calls ]----------------------------------------------
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "asnmp/smival.h"
#include "asnmp/snmperrs.h"            // V2c error def
#include "asnmp/oid.h"                 // oid class def
#include "asnmp/timetick.h"            // time ticks
#include "asnmp/counter.h"             // counter
#include "asnmp/gauge.h"               // gauge class
#include "asnmp/ctr64.h"               // 64 bit counters
#include "asnmp/octet.h"               // octet class
#include "asnmp/address.h"             // address class def
#include "asnmp/integer.h"             // integer class


//------------[ VB Class Def ]-------------------------------------
// The Vb class is the encapsulation of the SNMP variable binding.
// Variable binding lists in SNMP++ are represented as arrays of
// Vb objects. Vb objects are passed to and from SNMP objects to
// provide getting or setting MIB values.
// The vb class keeps its own memory for objects and does not
// utilize pointers to external data structures.
//
/**
 * @class Vb
 *
 * @brief Implement the concrete Variable Bindings aka Varbind
 * composite type. Varbinds hold 1 Oid and 1 Value (Any SMI value)
 */
class ASNMP_Export Vb
{
public:
  /// constructor with no arguments
  /// makes an vb, unitialized (does not make object valid)
  Vb( void);

  /// constructor to initialize the oid
  /// makes a vb with oid portion initialized (does not make object valid)
  Vb( const Oid &oid);

  /// constructor to initialize the oid
  /// makes a vb with oid portion and value portion initialized, (valid)
  Vb( const Oid& vb, const SnmpSyntax &val, const SmiUINT32=SNMP_CLASS_SUCCESS);

  /// copy constructor
  Vb( const Vb &vb);

  /**
   * destructor
   * if the vb has a oid or an octect string then
   * the associated memory needs to be freed
   */
  ~Vb();

  /// return validity of Vb object (both oid and value set return 1 else 0)
  int valid() const;

  /// assignment to another Vb object overloaded
  Vb& operator=( const Vb &vb);

  /// equivlence operator overloaded
  friend ASNMP_Export bool operator==( const Vb &lhs, const Vb &rhs);

  //-----[ set oid / get oid part]------------------------------------------

  /// set value oid only with another oid
  void set_oid( const Oid& oid);

  /// get oid portion
  void get_oid( Oid &oid) const;

  //-----[ set value part]--------------------------------------------------

  /// set a Vb null, if its not already
  void set_null();

  /// returns 0 on success and a value
  void set_value( const TimeTicks& ticks);

  /// returns 0 on success and a value
  void set_value( const Oid& oid);

  /// returns 0 on success and a value
  void set_value( const Counter32& ctr);

  /// returns 0 on success and a value
  void set_value( const Counter64& ctr);

  /// returns 0 on success and a value
  void set_value( const Gauge32& ctr);

  /// returns 0 on success and a value
  void set_value( const SnmpUInt32& ctr);

  /// returns 0 on success and a value
  void set_value( const SnmpInt32& ctr);

  /// get an octet string object
  void set_value( const OctetStr& oct_str);

  //----[ get value ]------------------------------------------------

  /// returns 0 on success and a value
  int get_value( TimeTicks& ticks);

  /// returns 0 on success and a value
  int get_value( Oid& oid);

  /// returns 0 on success and a value
  int get_value( Counter32& ctr);

  /// returns 0 on success and a value
  int get_value( Counter64& ctr);

  /// returns 0 on success and a value
  int get_value( Gauge32& ctr);

  /// returns 0 on success and a value
  int get_value( SnmpUInt32& ctr);

  /// returns 0 on success and a value
  int get_value( SnmpInt32& ctr);

  /// get an octet string object
  int get_value( OctetStr& oct_str);

  // escape hatch
  /// for other derived types  that can be casted
  void set_value( const SnmpSyntax &val);

  /// gets a general value
  int get_value( SnmpSyntax &val);

  /// return the current syntax
  /// Or.. if a V2 VB exception is present then return the exception value
  SmiUINT32 get_syntax();

  /// set the exception status
  friend ASNMP_Export void set_exception_status( Vb *vb, const SmiUINT32 status);

  /// return fomatted version of this object
  const char *to_string();

  /// returns a formatted version of the value
  const char *to_string_value();

  /// returns a formatted version of the value
  const char *to_string_oid();

protected:
  /// display vb as  [ oid / value ]
  char *output_;

  /// a vb is made up of a oid
  Oid iv_vb_oid_;

  /// and a value...
  SnmpSyntax *iv_vb_value_;

  /// are there any vb exceptions??
  SmiUINT32 exception_status_;

  void free_vb();
};

#endif  // VB_CLS_
