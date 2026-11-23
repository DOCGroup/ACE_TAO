#ifndef SMIDEF_
#define SMIDEF_
/**
 *  @file    smi.h
 *
 * Address class definition. Encapsulates various network
 * addresses into easy to use, safe and portable classes.
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

/// @name ASN/BER base types.
/// @{
/// Basic Encoding Rules (BER), used in forming syntax values and certain
/// SNMP types and values.
#define aSN_UNIVERSAL    (0x00)
#define aSN_APPLICATION  (0x40)
#define aSN_CONTEXT      (0x80)
#define aSN_PRIVATE      (0xC0)
#define aSN_PRIMITIVE    (0x00)
#define aSN_CONSTRUCTOR  (0x20)
/// @}

/// @name SNMP ObjectSyntax values.
/// @{
#define sNMP_SYNTAX_SEQUENCE  (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x10)

/// These values are used in the syntax member of the following smiVALUE
/// structure. UInt32 is indistinguishable from Gauge32 per SNMPv2; NsapAddr
/// is obsolete as a unique SMI type.
#define sNMP_SYNTAX_INT         (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x02)
#define sNMP_SYNTAX_BITS        (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x03)
#define sNMP_SYNTAX_OCTETS      (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x04)
#define sNMP_SYNTAX_NULL        (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x05)
#define sNMP_SYNTAX_OID         (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x06)
#define sNMP_SYNTAX_INT32       sNMP_SYNTAX_INT
#define sNMP_SYNTAX_IPADDR      (aSN_APPLICATION | aSN_PRIMITIVE | 0x00)
#define sNMP_SYNTAX_CNTR32      (aSN_APPLICATION | aSN_PRIMITIVE | 0x01)
#define sNMP_SYNTAX_GAUGE32     (aSN_APPLICATION | aSN_PRIMITIVE | 0x02)
#define sNMP_SYNTAX_TIMETICKS   (aSN_APPLICATION | aSN_PRIMITIVE | 0x03)
#define sNMP_SYNTAX_OPAQUE      (aSN_APPLICATION | aSN_PRIMITIVE | 0x04)
#define sNMP_SYNTAX_CNTR64      (aSN_APPLICATION | aSN_PRIMITIVE | 0x06)
#define sNMP_SYNTAX_UINT32      (aSN_APPLICATION | aSN_PRIMITIVE | 0x07)
/// @}

/// @name SNMPv2 exception conditions.
/// @{
/// Syntax values for SNMPv2 exception conditions.
#define sNMP_SYNTAX_NOSUCHOBJECT    (aSN_CONTEXT | aSN_PRIMITIVE | 0x00)
#define sNMP_SYNTAX_NOSUCHINSTANCE  (aSN_CONTEXT | aSN_PRIMITIVE | 0x01)
#define sNMP_SYNTAX_ENDOFMIBVIEW    (aSN_CONTEXT | aSN_PRIMITIVE | 0x02)
/// @}

/// @name PDU types.
/// @{
#define sNMP_PDU_GET            (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x0)
#define sNMP_PDU_GETNEXT        (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x1)
#define sNMP_PDU_RESPONSE       (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x2)
#define sNMP_PDU_SET            (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x3)
#define sNMP_PDU_V1TRAP     (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x4)
#define sNMP_PDU_GETBULK    (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x5)
#define sNMP_PDU_INFORM     (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x6)
#define sNMP_PDU_TRAP       (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x7)
/// @}

/// @name SMI types.
/// @{
/// SNMP-related types from RFC 1442 (SMI).

typedef unsigned char    SmiBYTE,        *SmiLPBYTE;

/// Integer type.
typedef long             SmiINT,         *SmiLPINT;

/// 32-bit integer type.
typedef SmiINT           SmiINT32,       *SmiLPINT32;

/// Unsigned 32-bit integer type.
typedef unsigned long    SmiUINT32,      *SmiLPUINT32;

/// Octet string type.
typedef struct {
     SmiUINT32 len;
     SmiLPBYTE ptr;}     SmiOCTETS,      *SmiLPOCTETS;

/// Bit string type.
typedef SmiOCTETS        SmiBITS,        *SmiLPBITS;

/// SMI object identifier type.
typedef struct {
     SmiUINT32   len;
     SmiLPUINT32 ptr;}   SmiOID,         *SmiLPOID;

/// IP address type.
typedef SmiOCTETS        SmiIPADDR,      *SmiLPIPADDR;

/// 32-bit counter type.
typedef SmiUINT32        SmiCNTR32,      *SmiLPCNTR32;

/// Gauge type.
typedef SmiUINT32        SmiGAUGE32,     *SmiLPGAUGE32;

/// TimeTicks type.
typedef SmiUINT32        SmiTIMETICKS,   *SmiLPTIMETICKS;

/// Opaque type.
typedef SmiOCTETS        SmiOPAQUE,      *SmiLPOPAQUE;

/// NSAP address type.
typedef SmiOCTETS        SmiNSAPADDR,    *SmiLPNSAPADDR;

/// 64-bit counter type.
typedef struct {
        SmiUINT32 hipart;
        SmiUINT32 lopart;
} SmiCNTR64,    *SmiLPCNTR64;
/// @}

#endif // SMIDEF_

