// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Typecode_Constants.cpp
//
// = DESCRIPTION
//   All the CORBA-specified typecode constants.
//
//   NOTE:  IFR TypeCode constants aren't here; they're left for an IDL
//   compiler to generate from the appropriate IDL source.
//
//   NOTE:  it'd be nice to have these not use init sections.  Most can easily
//   be in readonly data (e.g. text segment, ROM) rather than writable data;
//   that speeds program startup and page sharing in shared libraries.
//
//   THREADING NOTE:  no issues, these are immutable constants
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and Aniruddha Gokhale
//
// ============================================================================

#include "tao/Typecode_Constants.h"
#include "tao/Typecode.h"
#include "tao/NVList.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

#if defined (TAO_HAS_AMI_POLLER) && (TAO_HAS_AMI_POLLER == 1)
#include "tao/PollableC.h"
#endif /* TAO_HAS_AMI_POLLER == 1 */

ACE_RCSID (tao,
           Typecode_Constants,
           "$Id$")

// Declare all the standard typecodes owned by the ORB

// Null and void
namespace CORBA
{
  TypeCode_ptr  _tc_null = 0;
  TypeCode_ptr  _tc_void = 0;

  // Basic numeric types:  short, long, longlong, and unsigned
  // variants

  TypeCode_ptr  _tc_short = 0;
  TypeCode_ptr  _tc_long = 0;
  TypeCode_ptr  _tc_longlong = 0;
  TypeCode_ptr  _tc_ushort = 0;
  TypeCode_ptr  _tc_ulong = 0;
  TypeCode_ptr  _tc_ulonglong = 0;

  // Floating point types: single, double, quad precision
  TypeCode_ptr  _tc_float = 0;
  TypeCode_ptr  _tc_double = 0;
  TypeCode_ptr  _tc_longdouble = 0;

  // Various simple quantities.
  TypeCode_ptr  _tc_boolean = 0;
  TypeCode_ptr  _tc_octet = 0;

  // Internationalization-related data types: ISO Latin/1 and "wide"
  // characters, and strings of each.  "wchar" is probably Unicode 1.1,
  // "wstring" being null-terminated sets thereof.
  TypeCode_ptr  _tc_char = 0;
  TypeCode_ptr  _tc_wchar = 0;

  // a string/wstring have a simple parameter list that indicates the
  // length
  TypeCode_ptr  _tc_string = 0;
  TypeCode_ptr  _tc_wstring = 0;

  //
  // Various things that can be passed as "general" parameters:
  // Any, TypeCode_ptr  Principal_ptr, Object_ptr
  //
  TypeCode_ptr  _tc_any = 0;
  TypeCode_ptr  _tc_TypeCode = 0;
  TypeCode_ptr  _tc_Principal = 0;
  TypeCode_ptr  _tc_Object = 0;

  // Two typecodes for exceptions
  TypeCode_ptr CORBA::TypeCode::_tc_Bounds = 0;
  TypeCode_ptr CORBA::TypeCode::_tc_BadKind = 0;
  TypeCode_ptr  _tc_exception_type = 0;

  // Some more typecodes in the CORBA namespace. We keep adding
  // to this list as we find more and more things being introduced
  // to the CORBA namespace.

#if (TAO_HAS_MINIMUM_CORBA == 0)
     TypeCode_ptr  _tc_NamedValue = 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
} // End namespace CORBA

namespace TAO
{
  // Internal to TAO ORB
  CORBA::TypeCode_ptr TC_opaque = 0;
  CORBA::TypeCode_ptr TC_completion_status = 0;

  // Flag that denotes that the TAO TypeCode constants have been
  // initialized.
  int TypeCode_Constants::initialized_ = 0;

  // Initialize all the ORB owned TypeCode constants. This routine will
  // be invoked by the ORB_init method.
  void
  TypeCode_Constants::init (void)
  {
    // Initialize all the standard typecodes owned by the ORB

    // Not thread safe.  Caller must provide synchronization.

    // Do not execute code after this point more than once.
    if (initialized_ != 0)
      {
        return;
      }

    initialized_ = 1;

    // Null and void.
    ACE_NEW (CORBA::_tc_null,
             CORBA::TypeCode (CORBA::tk_null));

    ACE_NEW (CORBA::_tc_void,
             CORBA::TypeCode (CORBA::tk_void));

    // Basic numeric types:  short, long, longlong, and unsigned variants.
    ACE_NEW (CORBA::_tc_short,
             CORBA::TypeCode (CORBA::tk_short));

    ACE_NEW (CORBA::_tc_long,
             CORBA::TypeCode (CORBA::tk_long));

    ACE_NEW (CORBA::_tc_longlong,
             CORBA::TypeCode (CORBA::tk_longlong));

    ACE_NEW (CORBA::_tc_ushort,
             CORBA::TypeCode (CORBA::tk_ushort));

    ACE_NEW (CORBA::_tc_ulong,
             CORBA::TypeCode (CORBA::tk_ulong));

    ACE_NEW (CORBA::_tc_ulonglong,
             CORBA::TypeCode (CORBA::tk_ulonglong));

    // Floating point types: single, double, quad precision.
    ACE_NEW (CORBA::_tc_float,
             CORBA::TypeCode (CORBA::tk_float));

    ACE_NEW (CORBA::_tc_double,
             CORBA::TypeCode (CORBA::tk_double));

    ACE_NEW (CORBA::_tc_longdouble,
             CORBA::TypeCode (CORBA::tk_longdouble));

    // Various simple quantities.
    ACE_NEW (CORBA::_tc_boolean,
             CORBA::TypeCode (CORBA::tk_boolean));

    ACE_NEW (CORBA::_tc_octet,
             CORBA::TypeCode (CORBA::tk_octet));

    // Internationalization-related data types: ISO Latin/1 and "wide"
    // characters, and strings of each.  "wchar" is probably Unicode 1.1,
    // "wstring" being null-terminated sets thereof.
    ACE_NEW (CORBA::_tc_char,
             CORBA::TypeCode (CORBA::tk_char));

    ACE_NEW (CORBA::_tc_wchar,
             CORBA::TypeCode (CORBA::tk_wchar));

    // A string/wstring have a simple parameter list that
    // indicates the length.
    static const CORBA::Long _oc_string [] =
    {
      // CDR typecode octets
      TAO_ENCAP_BYTE_ORDER, // native endian + padding; "tricky"
      0                     // ... unbounded string
    };

    ACE_NEW (CORBA::_tc_string,
             CORBA::TypeCode (CORBA::tk_string,
                              sizeof _oc_string,
                              (char*)&_oc_string,
                              1,
                              sizeof (CORBA::String_var)));

    static const CORBA::Long _oc_wstring [] =
    {
      // CDR typecode octets
      TAO_ENCAP_BYTE_ORDER, // native endian + padding; "tricky"
      0                     // ... unbounded string
    };

    ACE_NEW (CORBA::_tc_wstring,
             CORBA::TypeCode (CORBA::tk_wstring,
                              sizeof _oc_wstring,
                              (char *) &_oc_wstring,
                              1,
                              sizeof (CORBA::WChar*)));

    //
    // Various things that can be passed as "general" parameters:
    // Any, TypeCode_ptr, Principal_ptr, Object_ptr
    //
    ACE_NEW (CORBA::_tc_any,
             CORBA::TypeCode (CORBA::tk_any));

    ACE_NEW (CORBA::_tc_TypeCode,
             CORBA::TypeCode (CORBA::tk_TypeCode));

    ACE_NEW (CORBA::_tc_Principal,
             CORBA::TypeCode (CORBA::tk_Principal));

    // typecode for objref is complex, has two string parameters
    //
    // NOTE:  Must be four-byte aligned

    static const CORBA::Long _oc_CORBA_Object[] =
    {
      TAO_ENCAP_BYTE_ORDER, // byte order
      29,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4f62),
      ACE_NTOHL (0x6a656374),
      ACE_NTOHL (0x3a312e30),
      ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Object:1.0
      7,
      ACE_NTOHL (0x4f626a65),
      ACE_NTOHL (0x63740000),  // name = Object
    };

    ACE_NEW (CORBA::_tc_Object,
             CORBA::TypeCode (CORBA::tk_objref,
                              sizeof (_oc_CORBA_Object),
                              (char *) &_oc_CORBA_Object,
                              1,
                              sizeof (CORBA::Object)));

    // Static initialization of the two user-defined exceptions that
    // are part of the ORB.

    static const CORBA::Long _oc_CORBA_TypeCode_Bounds[] =
    {
      TAO_ENCAP_BYTE_ORDER, // byte order
      38,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5479),
      ACE_NTOHL (0x7065436f),
      ACE_NTOHL (0x64652f42),
      ACE_NTOHL (0x6f756e64),
      ACE_NTOHL (0x733a312e),
      ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/TypeCode/Bounds:1.0
      7,
      ACE_NTOHL (0x426f756e),
      ACE_NTOHL (0x64730000),  // name = Bounds
      0, // member count
    };

    ACE_NEW (CORBA::TypeCode::_tc_Bounds,
             CORBA::TypeCode (CORBA::tk_except,
                              sizeof (_oc_CORBA_TypeCode_Bounds),
                              (char*) &_oc_CORBA_TypeCode_Bounds,
                              1,
                              sizeof (CORBA::TypeCode::Bounds)));


    static const CORBA::Long _oc_CORBA_TypeCode_BadKind[] =
    {
      TAO_ENCAP_BYTE_ORDER, // byte order
      39,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5479),
      ACE_NTOHL (0x7065436f),
      ACE_NTOHL (0x64652f42),
      ACE_NTOHL (0x61644b69),
      ACE_NTOHL (0x6e643a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/TypeCode/BadKind:1.0
      8,
      ACE_NTOHL (0x4261644b),
      ACE_NTOHL (0x696e6400),  // name = BadKind
      0, // member count
    };

    ACE_NEW (CORBA::TypeCode::_tc_BadKind,
             CORBA::TypeCode (CORBA::tk_except,
                              sizeof (_oc_CORBA_TypeCode_BadKind),
                              (char*) &_oc_CORBA_TypeCode_BadKind,
                              1,
                              sizeof (CORBA::TypeCode::BadKind)));

  #if (TAO_HAS_MINIMUM_CORBA == 0)

    static const CORBA::Long _oc_corba_NamedValue[] =
    {
      TAO_ENCAP_BYTE_ORDER,     // byte order
      33,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x636f7262),
      ACE_NTOHL (0x612f4e61),
      ACE_NTOHL (0x6d656456),
      ACE_NTOHL (0x616c7565),
      ACE_NTOHL (0x3a312e30),
      ACE_NTOHL (0x0),          // repository ID =
                                //   IDL:omg.org/corba/NamedValue:1.0
      11,
      ACE_NTOHL (0x4e616d65),
      ACE_NTOHL (0x6456616c),
      ACE_NTOHL (0x75650000),  // name = NamedValue,
    };

    ACE_NEW (CORBA::_tc_NamedValue,
             CORBA::TypeCode (CORBA::tk_objref,
                              sizeof (_oc_corba_NamedValue),
                              (char *) &_oc_corba_NamedValue,
                              0,
                              sizeof (CORBA::NamedValue)));

  #endif /* TAO_HAS_MINIMUM_CORBA */

   // ****************************************************************

   // The following are internal to the TAO ORB

    // Octet codes for the parameters of the "Opaque" (sequence of octet)
    // data type used various places internally ... a CDR encapsulation
    // holding two parameters (like all sequence TypeCodes).
    //
    // NOTE: this **MUST** be longword aligned, which is why it's coded as
    // a longword array not an octet array.  Just sticking a long in for
    // padding won't work with compilers that optimize unused data out of
    // existence.

    // CDR typecode octets.

    typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;

    static const CORBA::Long _oc_opaque [] =
    {

      TAO_ENCAP_BYTE_ORDER,    // native endian + padding; "tricky"
      10,                      // ... (sequence of) octets
      0                        // ... unbounded
    };

    ACE_NEW (TC_opaque,
             CORBA::TypeCode (CORBA::tk_sequence,
                              sizeof _oc_opaque,
                              (char *) &_oc_opaque,
                              1,
                              sizeof (TAO_opaque)));

    // Octet codes for the parameters of the ServiceContextList TypeCode
    // ...  this is a CDR encapsulation holding two parameters (like all
    // sequences): a TypeCode, and the bounds of the sequence (zero in
    // this case).
    //
    // This is complicated since the Typecode for the data type for the
    // sequence members is complex, a structure that nests two further
    // typecodes (one is a sequence).
    //
    // NOTE:  this must be longword aligned!

    static const CORBA::ULong oc_completion_status [] =
    {
      TAO_ENCAP_BYTE_ORDER, // byte order flag, tricky
      0, 0,                 // type ID omitted
      3,                    // three members
      0, 0,                 // ... whose names are all omitted
      0, 0,
      0, 0
    };

    ACE_NEW (TC_completion_status,
             CORBA::TypeCode (CORBA::tk_enum,
                              sizeof oc_completion_status,
                              (char *) &oc_completion_status,
                              1,
                              sizeof (CORBA::CompletionStatus)));
  }

  // Destroy all the typecodes owned by the ORB.
  void
  TypeCode_Constants::fini (void)
  {
    // Release all the standard typecodes owned by the ORB.

    // Null and void.
    CORBA::release (CORBA::_tc_null);

    CORBA::release (CORBA::_tc_void);

    // Basic numeric types:  short, long, longlong, and unsigned variants
    CORBA::release (CORBA::_tc_short);

    CORBA::release (CORBA::_tc_long);

    CORBA::release (CORBA::_tc_longlong);

    CORBA::release (CORBA::_tc_ushort);

    CORBA::release (CORBA::_tc_ulong);

    CORBA::release (CORBA::_tc_ulonglong);

    // Floating point types: single, double, quad precision
    CORBA::release (CORBA::_tc_float);

    CORBA::release (CORBA::_tc_double);

    CORBA::release (CORBA::_tc_longdouble);

    // Various simple quantities.
    CORBA::release (CORBA::_tc_boolean);

    CORBA::release (CORBA::_tc_octet);

    // Internationalization-related data types: ISO Latin/1 and "wide"
    // characters, and strings of each.  "wchar" is probably Unicode 1.1,
    // "wstring" being null-terminated sets thereof.
    CORBA::release (CORBA::_tc_char);

    CORBA::release (CORBA::_tc_wchar);

    // A string/wstring have a simple parameter list that
    // indicates the length.
    CORBA::release (CORBA::_tc_string);

    CORBA::release (CORBA::_tc_wstring);

    //
    // Various things that can be passed as "general" parameters:
    // Any, TypeCode_ptr, Principal_ptr, Object_ptr
    //
    CORBA::release (CORBA::_tc_any);

    CORBA::release (CORBA::_tc_TypeCode);

    CORBA::release (CORBA::_tc_Principal);

    // typecode for objref is complex, has two string parameters
    //
    CORBA::release (CORBA::_tc_Object);

    // other ORB owned typecodes
    CORBA::release (CORBA::TypeCode::_tc_Bounds);

    CORBA::release (CORBA::TypeCode::_tc_BadKind);

  #if (TAO_HAS_MINIMUM_CORBA == 0)

    CORBA::release (CORBA::_tc_NamedValue);

  #endif /* TAO_HAS_MINIMUM_CORBA */

    // TAO specific
    CORBA::release (TC_opaque);

    CORBA::release (TC_completion_status);
  }
} // end namespace TAO
