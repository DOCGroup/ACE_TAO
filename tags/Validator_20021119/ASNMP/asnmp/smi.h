/* -*-C++-*- */
// $Id$
#ifndef SMIDEF_
#define SMIDEF_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    smi.h
//
// = DESCRIPTION
// Address class definition. Encapsulates various network
// addresses into easy to use, safe and portable classes.
//
// = AUTHOR
//    ??
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

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//----------[ ASN/BER Base Types ]-----------------------------------------
// Basic Encoding Rules (BER)
// (used in forming SYNTAXes and certain SNMP types/values)
#define aSN_UNIVERSAL    (0x00)
#define aSN_APPLICATION  (0x40)
#define aSN_CONTEXT      (0x80)
#define aSN_PRIVATE      (0xC0)
#define aSN_PRIMITIVE    (0x00)
#define aSN_CONSTRUCTOR  (0x20)

//------[ SNMP ObjectSyntax Values ]---------------------------------------
#define sNMP_SYNTAX_SEQUENCE  (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x10)


//-------------------------------------------------------------------------
// These values are used in the "syntax" member
// of the smiVALUE structure which follows.
// Note: UInt32 is indistinguishable from Gauge32 per SNMPv2 Draft Standard
//       NsapAddr is obsoleted as unique SMI type per SNMPv2 Draft Standard
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

//-------------------------------------------------------------------------

//---------------[ Exception conditions for SNMPv2 ]-----------------------
// support SNMP v2
#define sNMP_SYNTAX_NOSUCHOBJECT    (aSN_CONTEXT | aSN_PRIMITIVE | 0x00)
#define sNMP_SYNTAX_NOSUCHINSTANCE  (aSN_CONTEXT | aSN_PRIMITIVE | 0x01)
#define sNMP_SYNTAX_ENDOFMIBVIEW    (aSN_CONTEXT | aSN_PRIMITIVE | 0x02)


//--------------[ different types of PDU's ]-------------------------------
#define sNMP_PDU_GET            (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x0)
#define sNMP_PDU_GETNEXT        (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x1)
#define sNMP_PDU_RESPONSE       (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x2)
#define sNMP_PDU_SET            (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x3)
#define sNMP_PDU_V1TRAP     (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x4)
#define sNMP_PDU_GETBULK    (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x5)
#define sNMP_PDU_INFORM     (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x6)
#define sNMP_PDU_TRAP       (aSN_CONTEXT | aSN_CONSTRUCTOR | 0x7)


//------[ smi typedefs ]---------------------------------------------------
// SNMP-related types from RFC1442 (SMI)

typedef unsigned char    SmiBYTE,        *SmiLPBYTE;

//-------------------------------------------------------------------------
// SNMP-related types from RFC1442 (SMI)

// int
typedef long             SmiINT,         *SmiLPINT;

// int 32
typedef SmiINT           SmiINT32,       *SmiLPINT32;

// unit32
typedef unsigned long    SmiUINT32,      *SmiLPUINT32;

// octet struct
typedef struct {
     SmiUINT32 len;
     SmiLPBYTE ptr;}     SmiOCTETS,      *SmiLPOCTETS;

// bits
typedef SmiOCTETS        SmiBITS,        *SmiLPBITS;

// SMI oid struct
typedef struct {
     SmiUINT32   len;
     SmiLPUINT32 ptr;}   SmiOID,         *SmiLPOID;

// ipaddr
typedef SmiOCTETS        SmiIPADDR,      *SmiLPIPADDR;

// 32bit counter
typedef SmiUINT32        SmiCNTR32,      *SmiLPCNTR32;

// gauge
typedef SmiUINT32        SmiGAUGE32,     *SmiLPGAUGE32;

// timeticks
typedef SmiUINT32        SmiTIMETICKS,   *SmiLPTIMETICKS;

// opaque
typedef SmiOCTETS        SmiOPAQUE,      *SmiLPOPAQUE;

// nsapaddr
typedef SmiOCTETS        SmiNSAPADDR,    *SmiLPNSAPADDR;

// 64 bit counter
typedef struct {
        SmiUINT32 hipart;
        SmiUINT32 lopart;
} SmiCNTR64,    *SmiLPCNTR64;

#endif // SMIDEF_


