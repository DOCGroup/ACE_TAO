/* -*-C++-*- */
// $Id$
#ifndef SNMPERRS_H_
#define SNMPERRS_H_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    snmperrs.h
//
// = DESCRIPTION
// Definition of error macros and error strings
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
============================================================================*/


//-------[ Positive ASNMP Error Return Codes ]------------------------------
// These values are error status values from RFC 1905
//
// The values can be returned via Pdu::get_error_status()
//
#define SNMP_ERROR_SUCCESS           0   // Success Status
#define SNMP_ERROR_TOO_BIG           1   // Pdu encoding too big
#define SNMP_ERROR_NO_SUCH_NAME      2   // No such VB name, see error index
#define SNMP_ERROR_BAD_VALUE         3   // Bad Vb
#define SNMP_ERROR_READ_ONLY         4   // VB is read only, see error index
#define SNMP_ERROR_GENERAL_VB_ERR    5   // General VB error, see error index
#define SNMP_ERROR_NO_ACCESS         6   // No access to MIBs data
#define SNMP_ERROR_WRONG_TYPE        7   // Requested type was incorrect
#define SNMP_ERROR_WRONG_LENGTH      8   // Request Pdu has inccorect length
#define SNMP_ERROR_WRONG_ENCODING    9   // Request Pdu has wrong encoding
#define SNMP_ERROR_WRONG_VALUE      10   // Request Pdu has wrong value
#define SNMP_ERROR_NO_CREATION      11   // Unable to create object specified
#define SNMP_ERROR_INCONSIST_VAL    12   // Inconsistent value in request
#define SNMP_ERROR_RESOURCE_UNAVAIL 13   // Resources unavailable
#define SNMP_ERROR_COMITFAIL        14   // Unable to comit
#define SNMP_ERROR_UNDO_FAIL        15   // Unable to undo
#define SNMP_ERROR_AUTH_ERR         16   // Authentication failure
#define SNMP_ERROR_NOT_WRITEABLE    17   // Mib Object not writeable
#define SNMP_ERROR_INCONSIS_NAME    18   // Inconsistent naming used


//-------[ Negative ASNMP Result/Error Return Codes ]-------------------

// General
#define SNMP_CLASS_SUCCESS                 0  // success
#define SNMP_CLASS_ERROR                  -1  // general error
#define SNMP_CLASS_RESOURCE_UNAVAIL       -2  // e.g., malloc failed
#define SNMP_CLASS_INTERNAL_ERROR         -3  // unexpected / internal error
#define SNMP_CLASS_UNSUPPORTED            -4  // unsupported function

// Callback reasons:
#define SNMP_CLASS_TIMEOUT                -5  // outstanding request timed out
#define SNMP_CLASS_ASYNC_RESPONSE         -6  // received response for outstd request
#define SNMP_CLASS_NOTIFICATION           -7  // received notification (trap/inform)
#define SNMP_CLASS_SESSION_DESTROYED      -8  // snmp::destroyed with oustanding reqs pending

// Snmp Class:
#define SNMP_CLASS_INVALID                -10 // snmp::mf called on invalid instance
#define SNMP_CLASS_INVALID_PDU            -11 // invalid pdu passed to mf
#define SNMP_CLASS_INVALID_TARGET         -12 // invalid target passed to mf
#define SNMP_CLASS_INVALID_CALLBACK       -13 // invalid callback to mf
#define SNMP_CLASS_INVALID_REQID          -14 // invalid request id to cancel
#define SNMP_CLASS_INVALID_NOTIFYID       -15 // missing trap/inform oid
#define SNMP_CLASS_INVALID_OPERATION      -16 // snmp operation not allowed for specified target
#define SNMP_CLASS_INVALID_OID            -17 // invalid oid passed to mf
#define SNMP_CLASS_INVALID_ADDRESS        -18 // invalid address passed to mf
#define SNMP_CLASS_ERR_STATUS_SET         -19 // agent returned response pdu with error_status set

// Transport Errors:
#define SNMP_CLASS_TL_UNSUPPORTED         -20 // transport unsupported
#define SNMP_CLASS_TL_IN_USE              -21 // transport in use
#define SNMP_CLASS_TL_FAILED              -22 // transport operation failed

// extras
#define SNMP_CLASS_SHUTDOWN               -23 // used for back door shutdown
#define SNMP_INVALID_ARGS                 -24  // invalid args passed

#ifdef INCLUDE_SNMP_ERR_STRINGS_

// ASCII strings returned through Snmp::error() function.  NOTE: altering
// the strings in this header file will not affect the return values of
// Snmp::error(), unless you rebuild the ASNMP library from source.

#define MAX_POS_ERROR                    SNMP_ERROR_INCONSIS_NAME
#define MAX_NEG_ERROR                    SNMP_CLASS_SHUTDOWN

static char * pErrs[] = {
  "Success",                                                  // 0
  "SNMP: Response PDU Too Big",                               // 1
  "SNMP: Variable does not exist",                            // 2
  "SNMP: Cannot modify variable: Bad Value",                  // 3
  "SNMP: Cannot modify object, Read Only",                    // 4
  "SNMP: Cannot perform operation, General Error",            // 5
  "SNMP: Cannot access variable, No Access",                  // 6
  "SNMP: Cannot create/modify variable, Wrong Type",          // 7
  "SNMP: Cannot create/set variable, Wrong Length",           // 8
  "SNMP: Cannot create/set variable, Wrong Encoding",         // 9
  "SNMP: Cannot create/set variable, Wrong Value",            // 10
  "SNMP: Cannot create variable, Creation Not Allowed",       // 11
  "SNMP: Cannot create/set variable, Inconsistent Value",     // 12
  "SNMP: Cannot create/set variable, Resource Unavailable",   // 13
  "SNMP: Cannot create/set variable, Commit Failed",          // 14
  "SNMP: Cannot create/set variable, Undo Failed",            // 15
  "SNMP: Cannot perform operation, Authorization Error",      // 16
  "SNMP: Cannot create/set variable, Not Writable",           // 17
  "SNMP: Cannot create variable, Inconsistent Name",          // 18
};


static char * nErrs[] = {
  // General:
  // 0  SNMP_CLASS_SUCCESS
  "ASNMP: Success",

  // 1  SNMP_CLASS_ERROR
  "ASNMP: Operation failed",

  // 2  SNMP_CLASS_RESOURCE_UNAVAIL
  "ASNMP: Resource unavailable",

  // 3  SNMP_CLASS_INTERNAL_ERROR
  "ASNMP: Internal error",

  // 4  SNMP_CLASS_UNSUPPORTED
  "ASNMP: Unsupported function",

  // Callback reasons:
  // 5  SNMP_CLASS_TIMEOUT
  "ASNMP: SNMP request timed out",

  // 6  SNMP_CLASS_ASYNC_RESPONSE
  "ASNMP: Received SNMP Response",

  // 7  SNMP_CLASS_NOTIFICATION
  "ASNMP: Received SNMP Notification (trap or inform)",

  // 8  SNMP_CLASS_SESSION_DESTROYED
  "ASNMP: Closing session with outstanding requests",

  // 9  reserved for future
  "Unknown error code",

  // Snmp Class errors:
  // 10 SNMP_CLASS_INVALID
  "ASNMP: Class not valid",

  // 11 SNMP_CLASS_INVALID_PDU
  "ASNMP: Invalid Pdu",

  // 12 SNMP_CLASS_INVALID_TARGET
  "ASNMP: Invalid Target",

  // 13 SNMP_CLASS_INVALID_CALLBACK
  "ASNMP: Invalid (null) Callback Function",

  // 14 SNMP_CLASS_INVALID_REQID
  "ASNMP: Invalid Request Id",

  // 15 SNMP_CLASS_INVALID_NOTIFYID
  "ASNMP: Invalid Notification Id",

  // 16 SNMP_CLASS_INVALID_OPERATION
  "ASNMP: SNMP Operation not supported on specified Target",

  // 17 SNMP_CLASS_INVALID_OID
  "ASNMP: Invalid Object Identifier",

  // 18 SNMP_CLASS_INVALID_ADDRESS
  "ASNMP: Invalid Address",

  // 19 SNMP_CLASS_ERR_STATUS_SET
  "ASNMP: Agent indicates error in SNMP request",

  // Transport Errors:
  // 20 SNMP_CLASS_TL_UNSUPPORTED
  "ASNMP: Transport is not supported",

  // 21 SNMP_CLASS_TL_IN_USE
  "ASNMP: Transport is in use",

  // 22 SNMP_CLASS_TL_FAILED
  "ASNMP: Transport operation failed",

  // 23 SNMP_CLASS_SHUTDOWN
  "ASNMP: Blocked Mode Shutdown",

  // unknown error code
  "Unknown error code",
};

#endif //INCLUDE_SNMP_ERR_STRINGS_

#endif //SNMPERRS_H_
