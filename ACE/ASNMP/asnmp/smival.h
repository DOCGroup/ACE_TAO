/* -*-C++-*- */
#ifndef SMIVALUE_
#define SMIVALUE_
//=============================================================================
/**
 *  @file    smival.h
 *
 *  $Id$
 *
 * SMIValue class definition. Superclass for the various types
 * of SNMP values (Address, Oid, Octet, etc.).  Provides
 * only a few functions, most info is in subclass.
 *
 *
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

//----[ includes ]-----------------------------------------------------
#include "asnmp/smi.h"
#include "asnmp/ASNMP_Export.h"
#include "ace/os_include/os_stddef.h"

//----[ macros ]-------------------------------------------------------

//======================================================================
// SMI value structure conforming with SMI RFC
//
typedef struct {                /* smiVALUE portion of VarBind */
          SmiUINT32 syntax;     /* Insert SNMP_SYNTAX_<type> */
        union   {
          SmiINT    sNumber;    /* SNMP_SYNTAX_INT
                                   SNMP_SYNTAX_INT32 */
          SmiUINT32 uNumber;    /* SNMP_SYNTAX_UINT32
                                   SNMP_SYNTAX_CNTR32
                                   SNMP_SYNTAX_GAUGE32
                                   SNMP_SYNTAX_TIMETICKS */
          SmiCNTR64 hNumber;    /* SNMP_SYNTAX_CNTR64 */
          SmiOCTETS string;     /* SNMP_SYNTAX_OCTETS
                                   SNMP_SYNTAX_BITS
                                   SNMP_SYNTAX_OPAQUE
                                   SNMP_SYNTAX_IPADDR
                                   SNMP_SYNTAX_NSAPADDR */
          SmiOID    oid;        /* SNMP_SYNTAX_OID */
          SmiBYTE   empty;      /* SNMP_SYNTAX_NULL
                                   SNMP_SYNTAX_NOSUCHOBJECT
                                   SNMP_SYNTAX_NOSUCHINSTANCE
                                   SNMP_SYNTAX_ENDOFMIBVIEW */
                  }   value;
}    SmiVALUE, *SmiLPVALUE;

// An "abstract" (pure virtual) class that serves as the base class
// for all specific SNMP syntax types.
//
class ASNMP_Export SnmpSyntax {
public:
  /// virtual function for getting a printable ASCII value for any SNMP Value
  virtual const char * to_string() = 0;

  /// return the current syntax
  virtual SmiUINT32 get_syntax() = 0;

  /// virtual clone operation for creating a new Value from an existing
  /// value.  The caller MUST use the delete operation on the return
  /// value when done.
  virtual  SnmpSyntax * clone() const = 0;

  /// virtual destructor to ensure deletion of derived classes...
  virtual ~SnmpSyntax() {};

  /// overloaded assignment operator
  /// This should be pure virtual, but WinNT compiler
  /// complains about unresolved reference at link time.
  virtual SnmpSyntax& operator=(SnmpSyntax &/*val*/)
  {
      return *this;
  }

  /// return validity of value object.
  virtual int valid() const = 0;

protected:
  SmiVALUE smival;
};

#endif  // SMIVALUE_
