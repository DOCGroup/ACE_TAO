// @(#)tc_const.cpp	1.3 95/09/12
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		All the CORBA-specified typecode constants.
//
// NOTE:  IFR TypeCode constants aren't here; they're left for an IDL
// compiler to generate from the appropriate IDL source.
//
// NOTE:  it'd be nice to have these not use init sections.  Most can easily
// be in readonly data (e.g. text segment, ROM) rather than writable data;
// that speeds program startup and page sharing in shared libraries.
//
// THREADING NOTE:  no issues, these are immutable constants
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.h>

//
// Null and void
//
static CORBA_TypeCode tc_null (tk_null);
const CORBA_TypeCode_ptr _tc_CORBA_Null = (CORBA_TypeCode_ptr) &tc_null;

CORBA_TypeCode_ptr
CORBA_TypeCode::_nil ()
{
  return &tc_null;
}

static CORBA_TypeCode tc_void (tk_void);
const CORBA_TypeCode_ptr _tc_CORBA_Void = &tc_void;

//
// Basic numeric types:  short, long, longlong, and unsigned variants
//
static CORBA_TypeCode tc_short (tk_short);
const CORBA_TypeCode_ptr _tc_CORBA_Short = &tc_short;

static CORBA_TypeCode tc_long (tk_long);
const CORBA_TypeCode_ptr _tc_CORBA_Long = &tc_long;

static CORBA_TypeCode tc_longlong (tk_longlong);
const CORBA_TypeCode_ptr _tc_CORBA_LongLong = &tc_longlong;

static CORBA_TypeCode tc_ushort (tk_ushort);
const CORBA_TypeCode_ptr _tc_CORBA_UShort = &tc_ushort;

static CORBA_TypeCode tc_ulong (tk_ulong);
const CORBA_TypeCode_ptr _tc_CORBA_ULong = &tc_ulong;

static CORBA_TypeCode tc_ulonglong (tk_ulonglong);
const CORBA_TypeCode_ptr _tc_CORBA_ULongLong = &tc_ulonglong;

//
// Floating point types:  single, double, quad precision
//
static CORBA_TypeCode tc_float (tk_float);
const CORBA_TypeCode_ptr _tc_CORBA_Float = &tc_float;

static CORBA_TypeCode tc_double (tk_double);
const CORBA_TypeCode_ptr _tc_CORBA_Double = &tc_double;

static CORBA_TypeCode tc_longdouble (tk_longdouble);
const CORBA_TypeCode_ptr _tc_CORBA_LongDouble = &tc_longdouble;

//
// Various simple quantities
//
static CORBA_TypeCode tc_boolean (tk_boolean);
const CORBA_TypeCode_ptr _tc_CORBA_Boolean = &tc_boolean;

static CORBA_TypeCode tc_octet (tk_octet);
const CORBA_TypeCode_ptr _tc_CORBA_Octet = &tc_octet;

//
// Internationalization-related data types:  ISO Latin/1 and "wide"
// characters, and strings of each.  "wchar" is probably Unicode 1.1,
// "wstring" being null-terminated sets thereof.
//
static CORBA_TypeCode tc_char (tk_char);
const CORBA_TypeCode_ptr _tc_CORBA_Char = &tc_char;

static CORBA_TypeCode tc_wchar (tk_wchar);
const CORBA_TypeCode_ptr _tc_CORBA_WChar = &tc_wchar;

static CORBA_TypeCode tc_string (tk_string);
const CORBA_TypeCode_ptr _tc_CORBA_String = &tc_string;

static CORBA_TypeCode tc_wstring (tk_wstring);
const CORBA_TypeCode_ptr _tc_CORBA_WString = &tc_wstring;

//
// Various things that can be passed as "general" parameters:
// Any, TypeCode_ptr, Principal_ptr, Object_ptr
//
static CORBA_TypeCode tc_any (tk_any);
const CORBA_TypeCode_ptr _tc_CORBA_Any = &tc_any;

static CORBA_TypeCode tc_typecode (tk_TypeCode);
const CORBA_TypeCode_ptr _tc_CORBA_TypeCode = &tc_typecode;

static CORBA_TypeCode tc_principal (tk_Principal);
const CORBA_TypeCode_ptr _tc_CORBA_Principal = &tc_principal;

//
// typecode for objref is complex, has two string parameters
//
// NOTE:  Must be four-byte aligned
//
static const unsigned char oc_objref [] = {
    0, 0, 0, 0,			// big endian encoding (+ padding)
    0, 0, 0, 29,		// 29 char string + 3 pad bytes
    'I', 'D', 'L', ':',
    'o', 'm', 'g', '.',
    'o', 'r', 'g', '/',
    'C', 'O', 'R', 'B',
    'A', '/', 'O', 'b',
    'j', 'e', 'c', 't',
    ':', '1', '.', '0',
    '\0', 0, 0, 0,
    0, 0, 0, 7,			// 7 chars "Object" + 1 pad byte
    'O', 'b', 'j', 'e',
    'c', 't', '\0', 0,
};
static CORBA_TypeCode
    tc_objref (tk_objref, sizeof oc_objref,
	    (unsigned char *)&oc_objref, CORBA_B_FALSE);
const CORBA_TypeCode_ptr _tc_CORBA_Object = &tc_objref;

