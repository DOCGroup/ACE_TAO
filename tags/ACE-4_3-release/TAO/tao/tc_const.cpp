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

#if 0
#include "tao/orb.h"
#include "tao/typecode.h"
#endif

#include "tao/corba.h"

// Null and void

static CORBA::TypeCode tc_null (CORBA::tk_null);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_null = (CORBA::TypeCode_ptr) &tc_null;

CORBA::TypeCode_ptr
CORBA::TypeCode::_nil (void)
{
  return &tc_null;
}

static CORBA::TypeCode tc_void (CORBA::tk_void);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_void = &tc_void;


// Basic numeric types:  short, long, longlong, and unsigned variants

static CORBA::TypeCode tc_short (CORBA::tk_short);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_short = &tc_short;

static CORBA::TypeCode tc_long (CORBA::tk_long);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_long = &tc_long;

static CORBA::TypeCode tc_longlong (CORBA::tk_longlong);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_longlong = &tc_longlong;

static CORBA::TypeCode tc_ushort (CORBA::tk_ushort);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_ushort = &tc_ushort;

static CORBA::TypeCode tc_ulong (CORBA::tk_ulong);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_ulong = &tc_ulong;

static CORBA::TypeCode tc_ulonglong (CORBA::tk_ulonglong);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_ulonglong = &tc_ulonglong;

// Floating point types:  single, double, quad precision

static CORBA::TypeCode tc_float (CORBA::tk_float);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_float = &tc_float;

static CORBA::TypeCode tc_double (CORBA::tk_double);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_double = &tc_double;

static CORBA::TypeCode tc_longdouble (CORBA::tk_longdouble);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_longdouble = &tc_longdouble;

// Various simple quantities

static CORBA::TypeCode tc_boolean (CORBA::tk_boolean);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_boolean = &tc_boolean;

static CORBA::TypeCode tc_octet (CORBA::tk_octet);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_octet = &tc_octet;

// Internationalization-related data types: ISO Latin/1 and "wide"
// characters, and strings of each.  "wchar" is probably Unicode 1.1,
// "wstring" being null-terminated sets thereof.

static CORBA::TypeCode tc_char (CORBA::tk_char);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_char = &tc_char;

static CORBA::TypeCode tc_wchar (CORBA::tk_wchar);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_wchar = &tc_wchar;

// a string/wstring have a simple parameter list that indicates the length 
static const CORBA::Long _oc_string [] = 
{	// CDR typecode octets
  1,				// native endian + padding; "tricky"
  0				// ... unbounded string
};
static CORBA::TypeCode tc_string (CORBA::tk_string,
			  sizeof _oc_string,
			  (u_char *) &_oc_string,
			  CORBA::B_FALSE);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_string = &tc_string;

static const CORBA::Long _oc_wstring [] =
{	// CDR typecode octets
  1,				// native endian + padding; "tricky"
  0				// ... unbounded string
};
static CORBA::TypeCode tc_wstring (CORBA::tk_wstring,
			  sizeof _oc_wstring,
			  (u_char *) &_oc_wstring,
			  CORBA::B_FALSE);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_wstring = &tc_wstring;

//
// Various things that can be passed as "general" parameters:
// Any, TypeCode_ptr, Principal_ptr, Object_ptr
//
static CORBA::TypeCode tc_any (CORBA::tk_any);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_any = &tc_any;

static CORBA::TypeCode tc_typecode (CORBA::tk_TypeCode);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_TypeCode = &tc_typecode;

static CORBA::TypeCode tc_principal (CORBA::tk_Principal);
ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_Principal = &tc_principal;

// typecode for objref is complex, has two string parameters
//
// NOTE:  Must be four-byte aligned

static const u_char oc_objref [] = 
{ 
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

static CORBA::TypeCode tc_objref (CORBA::tk_objref,
				 sizeof oc_objref,
				 (u_char *) &oc_objref,
				 CORBA::B_FALSE);

ACE_Svc_Export CORBA::TypeCode_ptr CORBA::_tc_Object = &tc_objref;

