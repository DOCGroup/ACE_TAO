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

#include "tao/Typecode.h"
#include "tao/ORB.h"
#include "tao/GIOP.h"
#include "tao/Object.h"
#include "tao/Object_KeyC.h"
#include "tao/PolicyC.h"
#include "tao/CurrentC.h"
#include "tao/InconsistentTypeCodeC.h"
#include "tao/DomainC.h"
#include "tao/WrongTransactionC.h"
#include "tao/Services.h"
#include "tao/NVList.h"
#if defined(TAO_POLLER)
#include "tao/PollableC.h"
#endif /* TAO_POLLER */

ACE_RCSID(tao, Typecode_Constants, "$Id$")

// Declare all the standard typecodes owned by the ORB

// Null and void
CORBA::TypeCode_ptr CORBA::_tc_null = 0;
CORBA::TypeCode_ptr CORBA::_tc_void = 0;
// Basic numeric types:  short, long, longlong, and unsigned variants
CORBA::TypeCode_ptr CORBA::_tc_short = 0;
CORBA::TypeCode_ptr CORBA::_tc_long = 0;
CORBA::TypeCode_ptr CORBA::_tc_longlong = 0;
CORBA::TypeCode_ptr CORBA::_tc_ushort = 0;
CORBA::TypeCode_ptr CORBA::_tc_ulong = 0;
CORBA::TypeCode_ptr CORBA::_tc_ulonglong = 0;
// Floating point types: single, double, quad precision
CORBA::TypeCode_ptr CORBA::_tc_float = 0;
CORBA::TypeCode_ptr CORBA::_tc_double = 0;
CORBA::TypeCode_ptr CORBA::_tc_longdouble = 0;
// Various simple quantities.
CORBA::TypeCode_ptr CORBA::_tc_boolean = 0;
CORBA::TypeCode_ptr CORBA::_tc_octet = 0;
// Internationalization-related data types: ISO Latin/1 and "wide"
// characters, and strings of each.  "wchar" is probably Unicode 1.1,
// "wstring" being null-terminated sets thereof.
CORBA::TypeCode_ptr CORBA::_tc_char = 0;
CORBA::TypeCode_ptr CORBA::_tc_wchar = 0;
// a string/wstring have a simple parameter list that indicates the length
CORBA::TypeCode_ptr CORBA::_tc_string = 0;
CORBA::TypeCode_ptr CORBA::_tc_wstring = 0;
//
// Various things that can be passed as "general" parameters:
// Any, TypeCode_ptr, Principal_ptr, Object_ptr
//
CORBA::TypeCode_ptr CORBA::_tc_any = 0;
CORBA::TypeCode_ptr CORBA::_tc_TypeCode = 0;
CORBA::TypeCode_ptr CORBA::_tc_Principal = 0;
CORBA::TypeCode_ptr CORBA::_tc_Object = 0;
// Two typecodes for exceptions
CORBA::TypeCode_ptr CORBA::TypeCode::_tc_Bounds = 0;
CORBA::TypeCode_ptr CORBA::TypeCode::_tc_BadKind = 0;
// Some more typecodes in the CORBA namespace. We keep adding
// to this list as we find more and more things being introduced
// to the CORBA namespace.

CORBA::TypeCode_ptr CORBA::_tc_Current = 0;
CORBA::TypeCode_ptr CORBA::_tc_Identifier = 0;
CORBA::TypeCode_ptr CORBA::_tc_RepositoryId = 0;
CORBA::TypeCode_ptr CORBA::_tc_ORBid = 0;
CORBA::TypeCode_ptr CORBA_ORB::_tc_ObjectId = 0;

#if !defined(TAO_HAS_MINIMUM_CORBA)
CORBA::TypeCode_ptr CORBA_ORB::_tc_InconsistentTypeCode = 0;
CORBA::TypeCode_ptr CORBA::_tc_ConstructionPolicy = 0;
CORBA::TypeCode_ptr CORBA::_tc_NamedValue = 0;
CORBA::TypeCode_ptr CORBA::_tc_WrongTransaction = 0;
#endif /* TAO_HAS_MINIMUM_CORBA */

CORBA::TypeCode_ptr CORBA::_tc_PolicyErrorCode = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyError = 0;
CORBA::TypeCode_ptr CORBA::_tc_InvalidPolicies = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyType = 0;
CORBA::TypeCode_ptr CORBA::_tc_Policy = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyList = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyTypeSeq = 0;
CORBA::TypeCode_ptr CORBA::_tc_SetOverrideType = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyManager = 0;
CORBA::TypeCode_ptr CORBA::_tc_PolicyCurrent = 0;

CORBA::TypeCode_ptr CORBA::_tc_DomainManager = 0;
CORBA::TypeCode_ptr CORBA::_tc_DomainManagerList = 0;

CORBA::TypeCode_ptr CORBA::_tc_PollableSet = 0;
CORBA::TypeCode_ptr CORBA::_tc_Pollable = 0;
CORBA::TypeCode_ptr CORBA::_tc_DIIPollable = 0;

// Service definitions
CORBA::TypeCode_ptr CORBA::_tc_ServiceType = 0;
CORBA::TypeCode_ptr CORBA::_tc_ServiceOption = 0;
CORBA::TypeCode_ptr CORBA::_tc_ServiceDetailType = 0;
CORBA::TypeCode_ptr CORBA::_tc_ServiceDetail = 0;
CORBA::TypeCode_ptr CORBA::_tc_ServiceInformation = 0;

// Internal to TAO ORB
CORBA::TypeCode_ptr TC_opaque = 0;
CORBA::TypeCode_ptr TC_ServiceContextList = 0;
CORBA::TypeCode_ptr TC_completion_status = 0;

// initialize all the ORB owned TypeCode constants. This
// routine will be invoked by the ORB_init method.
void
TAO_TypeCodes::init (void)
{
  // Initialize all the standard typecodes owned by the ORB

  // Null and void
  CORBA::_tc_null = new CORBA::TypeCode (CORBA::tk_null);

  CORBA::_tc_void = new CORBA::TypeCode (CORBA::tk_void);

  // Basic numeric types:  short, long, longlong, and unsigned variants
  CORBA::_tc_short = new CORBA::TypeCode (CORBA::tk_short);

  CORBA::_tc_long = new CORBA::TypeCode (CORBA::tk_long);

  CORBA::_tc_longlong = new CORBA::TypeCode (CORBA::tk_longlong);

  CORBA::_tc_ushort = new CORBA::TypeCode (CORBA::tk_ushort);

  CORBA::_tc_ulong = new CORBA::TypeCode (CORBA::tk_ulong);

  CORBA::_tc_ulonglong = new CORBA::TypeCode (CORBA::tk_ulonglong);

  // Floating point types: single, double, quad precision
  CORBA::_tc_float = new CORBA::TypeCode (CORBA::tk_float);

  CORBA::_tc_double = new CORBA::TypeCode (CORBA::tk_double);

  CORBA::_tc_longdouble = new CORBA::TypeCode (CORBA::tk_longdouble);

  // Various simple quantities.
  CORBA::_tc_boolean = new CORBA::TypeCode (CORBA::tk_boolean);

  CORBA::_tc_octet = new CORBA::TypeCode (CORBA::tk_octet);

  // Internationalization-related data types: ISO Latin/1 and "wide"
  // characters, and strings of each.  "wchar" is probably Unicode 1.1,
  // "wstring" being null-terminated sets thereof.
  CORBA::_tc_char = new CORBA::TypeCode (CORBA::tk_char);

  CORBA::_tc_wchar = new CORBA::TypeCode (CORBA::tk_wchar);

  // a string/wstring have a simple parameter list that indicates the length
  static const CORBA::Long _oc_string [] =
  {
    // CDR typecode octets
    TAO_ENCAP_BYTE_ORDER, // native endian + padding; "tricky"
    0                     // ... unbounded string
  };
  CORBA::_tc_string = new CORBA::TypeCode (CORBA::tk_string,
                                           sizeof _oc_string,
                                           (char*)&_oc_string,
                                           1,
                                           sizeof (CORBA::String_var));

  static const CORBA::Long _oc_wstring [] =
  {
    // CDR typecode octets
    TAO_ENCAP_BYTE_ORDER,       // native endian + padding; "tricky"
    0                     // ... unbounded string
  };
  CORBA::_tc_wstring = new CORBA::TypeCode (CORBA::tk_wstring,
                                            sizeof _oc_wstring,
                                            (char *) &_oc_wstring,
                                            1,
                                            sizeof (CORBA::WString));

  //
  // Various things that can be passed as "general" parameters:
  // Any, TypeCode_ptr, Principal_ptr, Object_ptr
  //
  CORBA::_tc_any = new CORBA::TypeCode (CORBA::tk_any);

  CORBA::_tc_TypeCode = new CORBA::TypeCode (CORBA::tk_TypeCode);

  CORBA::_tc_Principal = new CORBA::TypeCode (CORBA::tk_Principal);

  // typecode for objref is complex, has two string parameters
  //
  // NOTE:  Must be four-byte aligned

  static const u_char oc_objref [] =
  {
    0, 0, 0, 0,                 // big endian encoding (+ padding)
    0, 0, 0, 29,                  // 29 char string + 3 pad bytes
    'I', 'D', 'L', ':',
    'o', 'm', 'g', '.',
    'o', 'r', 'g', '/',
    'C', 'O', 'R', 'B',
    'A', '/', 'O', 'b',
    'j', 'e', 'c', 't',
    ':', '1', '.', '0',
    '\0', 0, 0, 0,
    0, 0, 0, 7,                 // 7 chars "Object" + 1 pad byte
    'O', 'b', 'j', 'e',
    'c', 't', '\0', 0,
  };

  CORBA::_tc_Object = new CORBA::TypeCode (CORBA::tk_objref,
                                           sizeof oc_objref,
                                           (char *) &oc_objref,
                                           1,
                                           sizeof (CORBA::Object));

  // Static initialization of the two user-defined exceptions that
  // are part of the ORB.

  static char tc_buf_Bounds [] =
  {
    0, 0, 0, 0,           // big endian, padded
    0, 0, 0, 38,  // strlen (id) + 1
    'I', 'D', 'L', ':',
    'o', 'm', 'g', '.',
    'o', 'r', 'g', '/',
    'C', 'O', 'R', 'B',
    'A', '/', 'T', 'y',
    'p', 'e', 'C', 'o',
    'd', 'e', '/', 'B',
    'o', 'u', 'n', 'd',
    's', ':', '1', '.',
    '0', '\0', 0, 0,
    0, 0, 0, 0            // no members to this typecode
  };

  CORBA::TypeCode::_tc_Bounds =
    new CORBA::TypeCode (CORBA::tk_except,
                         sizeof tc_buf_Bounds,
                         tc_buf_Bounds,
                         1,
                         sizeof (CORBA::TypeCode::Bounds));

  static char tc_buf_BadKind [] =
  {
    0, 0, 0, 0,           // big endian, padded
    0, 0, 0, 39,  // strlen (id) + 1
    'I', 'D', 'L', ':',
    'o', 'm', 'g', '.',
    'o', 'r', 'g', '/',
    'C', 'O', 'R', 'B',
    'A', '/', 'T', 'y',
    'p', 'e', 'C', 'o',
    'd', 'e', '/', 'B',
    'a', 'd', 'K', 'i',
    'n', 'd', ':', '1',
    '.', '0', '\0', 0,
    0, 0, 0, 0            // no members to this typecode
  };

  CORBA::TypeCode::_tc_BadKind =
    new CORBA::TypeCode (CORBA::tk_except,
                         sizeof tc_buf_BadKind,
                         tc_buf_BadKind,
                         1,
                         sizeof (CORBA::TypeCode::BadKind));

  static const CORBA::Long _oc_CORBA_Identifier[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    25,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0xfdfdfd),  // repository ID = IDL:CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x657200fd),  // name = Identifier
    CORBA::tk_string,
    0, // string length
  };

  CORBA::_tc_Identifier =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_Identifier),
                         (char *) &_oc_CORBA_Identifier,
                         1,
                         sizeof (CORBA::Identifier));

  static const CORBA::Long _oc_CORBA_RepositoryId[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    27,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e3000fd),  // repository ID = IDL:CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0xfdfdfd),  // name = RepositoryId
    CORBA::tk_string,
    0, // string length
  };
  CORBA::_tc_RepositoryId =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_RepositoryId),
                         (char *) &_oc_CORBA_RepositoryId,
                         1,
                         sizeof (CORBA::RepositoryId));

  static const CORBA::Long _oc_CORBA_ORBid[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    28, ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4f52),
    ACE_NTOHL (0x4269643a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ORBid:1.0
    6,
    ACE_NTOHL (0x4f524269),
    ACE_NTOHL (0x64000000),  // name = ORBid
    CORBA::tk_string,
    0, // string length
  };
  CORBA::_tc_ORBid = new CORBA::TypeCode (CORBA::tk_alias,
                                          sizeof (_oc_CORBA_ORBid),
                                          (char *) &_oc_CORBA_ORBid,
                                          0,
                                          sizeof (CORBA::ORBid));

  static const CORBA::Long _oc_CORBA_ObjectId[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    31,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4f62),
    ACE_NTOHL (0x6a656374),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = 
                             //   IDL:omg.org/CORBA/ObjectId:1.0
    9,
    ACE_NTOHL (0x4f626a65),
    ACE_NTOHL (0x63744964),
    ACE_NTOHL (0x0),        // name = ObjectId
    CORBA::tk_string,
    0,                      // string length
  };
  CORBA_ORB::_tc_ObjectId =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_ObjectId),
                         (char *) &_oc_CORBA_ObjectId,
                         0,
                         sizeof (CORBA_ORB::ObjectId));

#if !defined(TAO_HAS_MINIMUM_CORBA)
  static const CORBA::Long _oc_CORBA_ORB_InconsistentTypeCode[] =
  {
    TAO_ENCAP_BYTE_ORDER,   // byte order
    47,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4f52),
    ACE_NTOHL (0x422f496e),
    ACE_NTOHL (0x636f6e73),
    ACE_NTOHL (0x69737465),
    ACE_NTOHL (0x6e745479),
    ACE_NTOHL (0x7065436f),
    ACE_NTOHL (0x64653a31),
    ACE_NTOHL (0x2e3000fd), // repository ID =
                            //   IDL:omg.org/CORBA/ORB/InconsistentTypeCode:1.0
    21,
    ACE_NTOHL (0x496e636f),
    ACE_NTOHL (0x6e736973),
    ACE_NTOHL (0x74656e74),
    ACE_NTOHL (0x54797065),
    ACE_NTOHL (0x436f6465),
    ACE_NTOHL (0xfdfdfd),   // name = InconsistentTypeCode
    0,                      // member count
  };

  CORBA_ORB::_tc_InconsistentTypeCode =
    new CORBA::TypeCode (CORBA::tk_except,
                         sizeof (_oc_CORBA_ORB_InconsistentTypeCode),
                         (char *) &_oc_CORBA_ORB_InconsistentTypeCode,
                         0,
                         sizeof (CORBA_ORB_InconsistentTypeCode));

  static const CORBA::Long _oc_CORBA_ConstructionPolicy[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    41, 
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f436f), 
    ACE_NTOHL (0x6e737472), 
    ACE_NTOHL (0x75637469), 
    ACE_NTOHL (0x6f6e506f), 
    ACE_NTOHL (0x6c696379), 
    ACE_NTOHL (0x3a312e30), 
    ACE_NTOHL (0x0),          // repository ID = 
                              //   IDL:omg.org/CORBA/ConstructionPolicy:1.0
    19, 
    ACE_NTOHL (0x436f6e73), 
    ACE_NTOHL (0x74727563), 
    ACE_NTOHL (0x74696f6e), 
    ACE_NTOHL (0x506f6c69), 
    ACE_NTOHL (0x63790000),   // name = ConstructionPolicy,
  };
  CORBA::_tc_ConstructionPolicy = 
    new CORBA::TypeCode (CORBA::tk_objref, 
                         sizeof (_oc_CORBA_ConstructionPolicy), 
                         (char *) &_oc_CORBA_ConstructionPolicy, 
                         0, 
                         sizeof (CORBA::ConstructionPolicy));


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

  CORBA::_tc_NamedValue = 
    new CORBA::TypeCode (CORBA::tk_objref,
                         sizeof (_oc_corba_NamedValue),
                         (char *) &_oc_corba_NamedValue,
                         0,
                         sizeof (CORBA::NamedValue));

  static const CORBA::Long _oc_CORBA_WrongTransaction[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    39, 
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e),     
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f5772), 
    ACE_NTOHL (0x6f6e6754), 
    ACE_NTOHL (0x72616e73), 
    ACE_NTOHL (0x61637469), 
    ACE_NTOHL (0x6f6e3a31), 
    ACE_NTOHL (0x2e300000),   // repository ID = 
                              //   IDL:omg.org/CORBA/WrongTransaction:1.0
    17, 
    ACE_NTOHL (0x57726f6e), 
    ACE_NTOHL (0x67547261), 
    ACE_NTOHL (0x6e736163), 
    ACE_NTOHL (0x74696f6e), 
    ACE_NTOHL (0x0),          // name = WrongTransaction
    0,                        // member count
  };
  CORBA::_tc_WrongTransaction =
    new CORBA::TypeCode (CORBA::tk_except, 
                         sizeof (_oc_CORBA_WrongTransaction), 
                         (char *) &_oc_CORBA_WrongTransaction, 
                         0, 
                         sizeof (CORBA::WrongTransaction));

#endif /* TAO_HAS_MINIMUM_CORBA */

 // Service Types
  static const CORBA::Long _oc_CORBA_ServiceOption[] =
  {
   TAO_ENCAP_BYTE_ORDER, // byte order
   36,
   ACE_NTOHL (0x49444c3a),
   ACE_NTOHL (0x6f6d672e),
   ACE_NTOHL (0x6f72672f),
   ACE_NTOHL (0x434f5242),
   ACE_NTOHL (0x412f5365),
   ACE_NTOHL (0x72766963),
   ACE_NTOHL (0x654f7074),
   ACE_NTOHL (0x696f6e3a),
   ACE_NTOHL (0x312e3000),  // repository ID= IDL:omg.org/CORBA/ServiceOption:1.0
   14,
   ACE_NTOHL (0x53657276),
   ACE_NTOHL (0x6963654f),
   ACE_NTOHL (0x7074696f),
   ACE_NTOHL (0x6e000000),  // name = ServiceOption
   CORBA::tk_ulong,
  };

  CORBA::_tc_ServiceOption =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_ServiceOption),
                         (char *) &_oc_CORBA_ServiceOption,
                         0,
                         sizeof (CORBA::ServiceOption));

  static const CORBA::Long _oc_CORBA_ServiceDetailType[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    40,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65446574),
    ACE_NTOHL (0x61696c54),
    ACE_NTOHL (0x7970653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ServiceDetailType:1.0
    18,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636544),
    ACE_NTOHL (0x65746169),
    ACE_NTOHL (0x6c547970),
    ACE_NTOHL (0x65000000),  // name = ServiceDetailType
    CORBA::tk_ulong,
  };

 CORBA::_tc_ServiceDetailType =
   new CORBA::TypeCode (CORBA::tk_alias,
                        sizeof (_oc_CORBA_ServiceDetailType),
                        (char *) &_oc_CORBA_ServiceDetailType,
                        0,
                        sizeof (CORBA::ServiceDetailType));

  static const CORBA::Long _oc_CORBA_ServiceType[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID= IDL:omg.org/CORBA/ServiceType:1.0
    12,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636554),
    ACE_NTOHL (0x79706500),  // name = ServiceType
    CORBA::tk_ushort,
  };

 CORBA::_tc_ServiceType =
   new CORBA::TypeCode (CORBA::tk_alias,
                        sizeof (_oc_CORBA_ServiceType),
                        (char *) &_oc_CORBA_ServiceType,
                        0,
                        sizeof (CORBA::ServiceType));

  static const CORBA::Long _oc_CORBA_ServiceDetail[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65446574),
    ACE_NTOHL (0x61696c3a),
    ACE_NTOHL (0x312e3000),  // repository ID= IDL:omg.org/CORBA/ServiceDetail:1.0
    14,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636544),
    ACE_NTOHL (0x65746169),
    ACE_NTOHL (0x6c000000),  // name = ServiceDetail
    2, // member count
    20,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x64657461),
    ACE_NTOHL (0x696c5f74),
    ACE_NTOHL (0x79706500),  // name = service_detail_type
    CORBA::tk_alias, // typecode kind for typedefs
    76, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    40,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65446574),
    ACE_NTOHL (0x61696c54),
    ACE_NTOHL (0x7970653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ServiceDetailType:1.0
    18,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636544),
    ACE_NTOHL (0x65746169),
    ACE_NTOHL (0x6c547970),
    ACE_NTOHL (0x65000000),  // name = ServiceDetailType
    CORBA::tk_ulong,
    15,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x64657461),
    ACE_NTOHL (0x696c0000),  // name = service_detail
    CORBA::tk_sequence, // typecode kind
    12, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_octet,
    0,
  };

  CORBA::_tc_ServiceDetail =
    new CORBA::TypeCode (CORBA::tk_struct,
                         sizeof (_oc_CORBA_ServiceDetail),
                         (char *) &_oc_CORBA_ServiceDetail,
                         0,
                         sizeof (CORBA::ServiceDetail));


  static const CORBA::Long _oc_CORBA_ServiceInformation[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    41,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65496e66),
    ACE_NTOHL (0x6f726d61),
    ACE_NTOHL (0x74696f6e),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/ServiceInformation:1.0
    19,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636549),
    ACE_NTOHL (0x6e666f72),
    ACE_NTOHL (0x6d617469),
    ACE_NTOHL (0x6f6e0000),  // name = ServiceInformation
    2, // member count
    16,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x6f707469),
    ACE_NTOHL (0x6f6e7300),  // name = service_options
    CORBA::tk_sequence, // typecode kind
    84, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_alias, // typecode kind for typedefs
    68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x654f7074),
    ACE_NTOHL (0x696f6e3a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ServiceOption:1.0
    14,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x6963654f),
    ACE_NTOHL (0x7074696f),
    ACE_NTOHL (0x6e000000),  // name = ServiceOption
    CORBA::tk_ulong,
    0,
    16,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x64657461),
    ACE_NTOHL (0x696c7300),  // name = service_details
    CORBA::tk_sequence, // typecode kind
    232, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    216, // encapsulation length
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65446574),
    ACE_NTOHL (0x61696c3a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ServiceDetail:1.0
    14,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636544),
    ACE_NTOHL (0x65746169),
    ACE_NTOHL (0x6c000000),  // name = ServiceDetail
    2, // member count
    20,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x64657461),
    ACE_NTOHL (0x696c5f74),
    ACE_NTOHL (0x79706500),  // name = service_detail_type
    CORBA::tk_alias, // typecode kind for typedefs
    76, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    40,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x72766963),
    ACE_NTOHL (0x65446574),
    ACE_NTOHL (0x61696c54),
    ACE_NTOHL (0x7970653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ServiceDetailType:1.0
    18,
    ACE_NTOHL (0x53657276),
    ACE_NTOHL (0x69636544),
    ACE_NTOHL (0x65746169),
    ACE_NTOHL (0x6c547970),
    ACE_NTOHL (0x65000000),  // name = ServiceDetailType
    CORBA::tk_ulong,
    15,
    ACE_NTOHL (0x73657276),
    ACE_NTOHL (0x6963655f),
    ACE_NTOHL (0x64657461),
    ACE_NTOHL (0x696c0000),  // name = service_detail
    CORBA::tk_sequence, // typecode kind
    12, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_octet,
    0,
    0,
  };

 CORBA::_tc_ServiceInformation =
    new CORBA::TypeCode (CORBA::tk_struct,
                         sizeof (_oc_CORBA_ServiceInformation),
                         (char *) &_oc_CORBA_ServiceInformation,
                         0,
                         sizeof (CORBA::ServiceInformation)); 

  // ****************************************************************

  static const CORBA::Long _oc_CORBA_PolicyErrorCode[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x4572726f),
    ACE_NTOHL (0x72436f64),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID =
                             //   IDL:omg.org/CORBA/PolicyErrorCode:1.0
    16,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63794572),
    ACE_NTOHL (0x726f7243),
    ACE_NTOHL (0x6f646500),  // name = PolicyErrorCode
    CORBA::tk_short,
  };
  CORBA::_tc_PolicyErrorCode =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_PolicyErrorCode),
                         (char *) &_oc_CORBA_PolicyErrorCode,
                         0,
                         sizeof (CORBA::PolicyErrorCode));

  static const CORBA::Long _oc_CORBA_PolicyError[] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x4572726f),
    ACE_NTOHL (0x723a312e),
    ACE_NTOHL (0x30000000),   // repository ID =
                              //   IDL:omg.org/CORBA/PolicyError:1.0
    12,
    ACE_NTOHL (0x506f6c69),

    ACE_NTOHL (0x63794572),
    ACE_NTOHL (0x726f7200),   // name = PolicyError
    1, // member count
    7,
    ACE_NTOHL (0x72656173),
    ACE_NTOHL (0x6f6e0000),   // name = reason
    CORBA::tk_alias,          // typecode kind for typedefs
    72,                       // encapsulation length
    TAO_ENCAP_BYTE_ORDER,     // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x4572726f),
    ACE_NTOHL (0x72436f64),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),   // repository ID =
                              //   IDL:omg.org/CORBA/PolicyErrorCode:1.0
    16,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63794572),
    ACE_NTOHL (0x726f7243),
    ACE_NTOHL (0x6f646500),   // name = PolicyErrorCode
    CORBA::tk_short,
  };
  CORBA::_tc_PolicyError =
    new CORBA::TypeCode(CORBA::tk_except,
                        sizeof (_oc_CORBA_PolicyError),
                        (char *) &_oc_CORBA_PolicyError,
                        0,
                        sizeof (CORBA::PolicyError));

  static const CORBA::Long _oc_CORBA_InvalidPolicies[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f496e),
    ACE_NTOHL (0x76616c69),
    ACE_NTOHL (0x64506f6c),
    ACE_NTOHL (0x69636965),
    ACE_NTOHL (0x733a312e),
    ACE_NTOHL (0x30000000),   // repository ID = IDL:omg.org/CORBA/InvalidPolicies:1.0
    16,
    ACE_NTOHL (0x496e7661),
    ACE_NTOHL (0x6c696450),
    ACE_NTOHL (0x6f6c6963),
    ACE_NTOHL (0x69657300),   // name = InvalidPolicies
    1,                        // member count
    8,
    ACE_NTOHL (0x696e6469),
    ACE_NTOHL (0x63657300),   // name = indices
    CORBA::tk_sequence,       // typecode kind
    12,                       // encapsulation length
    TAO_ENCAP_BYTE_ORDER,     // byte order
    CORBA::tk_ushort,
    0,
  };
  CORBA::_tc_InvalidPolicies =
    new CORBA::TypeCode (CORBA::tk_except,
                         sizeof (_oc_CORBA_InvalidPolicies),
                         (char *) &_oc_CORBA_InvalidPolicies,
                         0,
                         sizeof (CORBA::InvalidPolicies));

  static const CORBA::Long _oc_CORBA_PolicyType[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x54797065),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),          // repository ID =
                              //   IDL:omg.org/CORBA/PolicyType:1.0
    11,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63795479),
    ACE_NTOHL (0x70650000),   // name = PolicyType
    CORBA::tk_ulong,
  };
  CORBA::_tc_PolicyType =
    new CORBA::TypeCode (CORBA::tk_alias,
                         sizeof (_oc_CORBA_PolicyType),
                         (char *) &_oc_CORBA_PolicyType,
                         0,
                         sizeof (CORBA::PolicyType));

  static const CORBA::Long _oc_CORBA_Policy[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    29,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),          // repository ID = IDL:omg.org/CORBA/Policy:1.0
    7,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63790000),   // name = Policy,
  };
  CORBA::_tc_Policy =
    new CORBA::TypeCode (CORBA::tk_objref,
                         sizeof (_oc_CORBA_Policy),
                         (char *) &_oc_CORBA_Policy,
                         1,
                         sizeof (CORBA::Policy));

  static const CORBA::Long _oc_CORBA_PolicyList[] =
  {
    TAO_ENCAP_BYTE_ORDER,       // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x4c697374),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),            // repository ID =
                                //   IDL:omg.org/CORBA/PolicyList:1.0
    11,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63794c69),
    ACE_NTOHL (0x73740000),     // name = PolicyList
    CORBA::tk_sequence,         // typecode kind
    68,                         // encapsulation length
    TAO_ENCAP_BYTE_ORDER,       // byte order
    CORBA::tk_objref,           // typecode kind
    52,                         // encapsulation length
    TAO_ENCAP_BYTE_ORDER,       // byte order
    29,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),            // repository ID =
                                //   IDL:omg.org/CORBA/Policy:1.0
    7,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63790000),     // name = Policy,
    0,
  };
 CORBA::_tc_PolicyList =
   new CORBA::TypeCode (CORBA::tk_alias,
                        sizeof (_oc_CORBA_PolicyList),
                        (char *) &_oc_CORBA_PolicyList,
                        1,
                        sizeof (CORBA::PolicyList));

  static const CORBA::Long _oc_CORBA_PolicyTypeSeq[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x54797065),
    ACE_NTOHL (0x5365713a),
    ACE_NTOHL (0x312e3000),   // repository ID =
                              //   IDL:omg.org/CORBA/PolicyTypeSeq:1.0
    14,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63795479),
    ACE_NTOHL (0x70655365),
    ACE_NTOHL (0x71000000),   // name = PolicyTypeSeq
    CORBA::tk_sequence,       // typecode kind
    80,                       // encapsulation length
    TAO_ENCAP_BYTE_ORDER,     // byte order
    CORBA::tk_alias,          // typecode kind for typedefs
    64,                       // encapsulation length
    TAO_ENCAP_BYTE_ORDER,     // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x54797065),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),          // repository ID =
                              //   IDL:omg.org/CORBA/PolicyType:1.0
    11,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63795479),
    ACE_NTOHL (0x70650000),   // name = PolicyType
    CORBA::tk_ulong,
    0,
 };
 CORBA::_tc_PolicyTypeSeq =
   new CORBA::TypeCode (CORBA::tk_alias,
                        sizeof (_oc_CORBA_PolicyTypeSeq),
                        (char *) &_oc_CORBA_PolicyTypeSeq,
                        0,
                        sizeof (CORBA::PolicyTypeSeq));

  static const CORBA::Long _oc_CORBA_SetOverrideType[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5365),
    ACE_NTOHL (0x744f7665),
    ACE_NTOHL (0x72726964),
    ACE_NTOHL (0x65547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),   // repository ID =
                              //   IDL:omg.org/CORBA/SetOverrideType:1.0
    16,
    ACE_NTOHL (0x5365744f),
    ACE_NTOHL (0x76657272),
    ACE_NTOHL (0x69646554),
    ACE_NTOHL (0x79706500),   // name = SetOverrideType
    2,                        // member count
    13,
    ACE_NTOHL (0x5345545f),
    ACE_NTOHL (0x4f564552),
    ACE_NTOHL (0x52494445),
    ACE_NTOHL (0x0),          // name = SET_OVERRIDE
    13,
    ACE_NTOHL (0x4144445f),
    ACE_NTOHL (0x4f564552),
    ACE_NTOHL (0x52494445),
    ACE_NTOHL (0x0),          // name = ADD_OVERRIDE
  };
 CORBA::_tc_SetOverrideType =
   new CORBA::TypeCode (CORBA::tk_enum,
                        sizeof (_oc_CORBA_SetOverrideType),
                        (char *) &_oc_CORBA_SetOverrideType,
                        0,
                        sizeof (CORBA::SetOverrideType));

  static const CORBA::Long _oc_CORBA_PolicyManager[] =
 {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x4d616e61),
    ACE_NTOHL (0x6765723a),
    ACE_NTOHL (0x312e3000),   // repository ID =
                              //   IDL:omg.org/CORBA/PolicyManager:1.0
    14,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63794d61),
    ACE_NTOHL (0x6e616765),
    ACE_NTOHL (0x72000000),   // name = PolicyManager,
  };
 CORBA::_tc_PolicyManager =
   new CORBA::TypeCode (CORBA::tk_objref,
                        sizeof (_oc_CORBA_PolicyManager),
                        (char *) &_oc_CORBA_PolicyManager,
                        0,
                        sizeof (CORBA::PolicyManager));

  static const CORBA::Long _oc_CORBA_PolicyCurrent[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c696379),
    ACE_NTOHL (0x43757272),
    ACE_NTOHL (0x656e743a),
    ACE_NTOHL (0x312e3000),   // repository ID =
                              //   IDL:omg.org/CORBA/PolicyCurrent:1.0
    14,
    ACE_NTOHL (0x506f6c69),
    ACE_NTOHL (0x63794375),
    ACE_NTOHL (0x7272656e),
    ACE_NTOHL (0x74000000),   // name = PolicyCurrent,
  };
 CORBA::_tc_PolicyCurrent =
   new CORBA::TypeCode  (CORBA::tk_objref,
                         sizeof (_oc_CORBA_PolicyCurrent),
                         (char *) &_oc_CORBA_PolicyCurrent,
                         0,
                         sizeof (CORBA::PolicyCurrent));

  static const CORBA::Long _oc_CORBA_DomainManager[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    36, 
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f446f), 
    ACE_NTOHL (0x6d61696e), 
    ACE_NTOHL (0x4d616e61), 
    ACE_NTOHL (0x6765723a), 
    ACE_NTOHL (0x312e3000),   // repository ID = 
                              //IDL:omg.org/CORBA/DomainManager:1.0
    14, 
    ACE_NTOHL (0x446f6d61), 
  
    ACE_NTOHL (0x696e4d61), 
    ACE_NTOHL (0x6e616765), 
    ACE_NTOHL (0x72000000),   // name = DomainManager,
  };
  CORBA::_tc_DomainManager = 
    new CORBA::TypeCode (CORBA::tk_objref,
                         sizeof (_oc_CORBA_DomainManager), 
                         (char *) &_oc_CORBA_DomainManager, 
                         0, 
                         sizeof (CORBA_DomainManager));

  static const CORBA::Long _oc_CORBA_DomainManagerList[] =
  {
    TAO_ENCAP_BYTE_ORDER,       // byte order
    40, 
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f446f), 
    ACE_NTOHL (0x6d61696e), 
    ACE_NTOHL (0x4d616e61), 
    ACE_NTOHL (0x6765724c), 
    ACE_NTOHL (0x6973743a), 
    ACE_NTOHL (0x312e3000),     // repository ID = 
                                //IDL:omg.org/CORBA/DomainManagerList:1.0
    18, 
    ACE_NTOHL (0x446f6d61), 
    ACE_NTOHL (0x696e4d61), 
    ACE_NTOHL (0x6e616765), 
    ACE_NTOHL (0x724c6973), 
    ACE_NTOHL (0x74000000),     // name = DomainManagerList
    CORBA::tk_sequence,         // typecode kind
    80,                         // encapsulation length
    TAO_ENCAP_BYTE_ORDER,       // byte order
    CORBA::tk_objref,           // typecode kind
    64,                         // encapsulation length
    TAO_ENCAP_BYTE_ORDER,       // byte order
    36, 
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f446f), 
    ACE_NTOHL (0x6d61696e), 
    ACE_NTOHL (0x4d616e61), 
    ACE_NTOHL (0x6765723a), 
    ACE_NTOHL (0x312e3000),     // repository ID = 
                                //IDL:omg.org/CORBA/DomainManager:1.0
    14, 
    ACE_NTOHL (0x446f6d61), 
    ACE_NTOHL (0x696e4d61), 
    ACE_NTOHL (0x6e616765), 
    ACE_NTOHL (0x72000000),     // name = DomainManager,
    0,
  };
  CORBA::_tc_DomainManagerList = 
    new CORBA::TypeCode (CORBA::tk_alias, 
                         sizeof (_oc_CORBA_DomainManagerList), 
                         (char *) &_oc_CORBA_DomainManagerList, 
                         0, 
                         sizeof (CORBA_DomainManagerList));

 // ****************************************************************

#if defined(TAO_POLLER)
  static const CORBA::Long _oc_CORBA_Pollable[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    31,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c6c6162),
    ACE_NTOHL (0x6c653a31),
    ACE_NTOHL (0x2e300000),   // repository ID =
                              //   IDL:omg.org/CORBA/Pollable:1.0
    9,
    ACE_NTOHL (0x506f6c6c),
    ACE_NTOHL (0x61626c65),
    ACE_NTOHL (0x0),          // name = Pollable,
  };
 CORBA::_tc_Pollable =
   new CORBA::TypeCode (CORBA::tk_objref,
                        sizeof (_oc_CORBA_Pollable),
                        (char *) &_oc_CORBA_Pollable,
                        0,
                        sizeof (CORBA_Pollable));

  static const CORBA::Long _oc_CORBA_DIIPollable[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4449),
    ACE_NTOHL (0x49506f6c),
    ACE_NTOHL (0x6c61626c),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),   // repository ID =
                              //   IDL:omg.org/CORBA/DIIPollable:1.0
    12,
    ACE_NTOHL (0x44494950),
    ACE_NTOHL (0x6f6c6c61),
    ACE_NTOHL (0x626c6500),   // name = DIIPollable,
  };
 CORBA::_tc_DIIPollable =
   new CORBA::TypeCode (CORBA::tk_objref,
                        sizeof (_oc_CORBA_DIIPollable),
                        (char *) &_oc_CORBA_DIIPollable,
                        0,
                        sizeof (CORBA::DIIPollable));

  static const CORBA::Long _oc_CORBA_PollableSet[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f506f),
    ACE_NTOHL (0x6c6c6162),
    ACE_NTOHL (0x6c655365),
    ACE_NTOHL (0x743a312e),
    ACE_NTOHL (0x30000000),   // repository ID =
                              //   IDL:omg.org/CORBA/PollableSet:1.0
    12,
    ACE_NTOHL (0x506f6c6c),
    ACE_NTOHL (0x61626c65),
    ACE_NTOHL (0x53657400),   // name = PollableSet,
  };
 CORBA::_tc_PollableSet =
   new CORBA::TypeCode (CORBA::tk_objref,
                        sizeof (_oc_CORBA_PollableSet),
                        (char *) &_oc_CORBA_PollableSet,
                        0,
                        sizeof (CORBA_PollableSet));

#endif /* TAO_POLLER */

 // ****************************************************************

  static const CORBA::Long _oc_CORBA_Current[] =
  {
    TAO_ENCAP_BYTE_ORDER,     // byte order
    22,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4375),
    ACE_NTOHL (0x7272656e),
    ACE_NTOHL (0x743a312e),
    ACE_NTOHL (0x3000fdfd),   // repository ID = IDL:CORBA/Current:1.0
    8,
    ACE_NTOHL (0x43757272),
    ACE_NTOHL (0x656e7400),   // name = Current,
  };
 CORBA::_tc_Current = new CORBA::TypeCode (CORBA::tk_objref,
                                           sizeof (_oc_CORBA_Current),
                                           (char *) &_oc_CORBA_Current,
                                           1,
                                           sizeof (CORBA::Current));

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

  static const CORBA::Long _oc_opaque [] =
  {

    TAO_ENCAP_BYTE_ORDER,    // native endian + padding; "tricky"
    10,                      // ... (sequence of) octets
    0                        // ... unbounded
  };

  TC_opaque = new CORBA::TypeCode (CORBA::tk_sequence,
                                   sizeof _oc_opaque,
                                   (char *) &_oc_opaque,
                                   1,
                                   sizeof (TAO_opaque));

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

  static const CORBA::Long _oc_svc_ctx_list [] =
  {
    // START bytes of encapsulation 0
    TAO_ENCAP_BYTE_ORDER, // native endian + padding; "tricky"

    //
    // FIRST sequence param:  typecode for struct is complex,
    // and so uses a nested encapsulation.
    //
    CORBA::tk_struct,
    72, // length of encapsulation 1

    // START bytes of encapsulation 1 (struct params)
    1, // native endian + padding; "tricky"
    1, 0, // type ID omitted:  null string
    1, 0, // name omitted "ServiceContext"

    2, // two struct elements

    // First structure element:  name, typecode for ULong
    //
    // NOTE:  to be more strictly correct this could be a CORBA::tk_alias
    // typecode ...

    1, 0, // name omitted:  "context_id"
    CORBA::tk_long,

    // Second structure element: name, typecode for sequence of octet;
    // the typecode for sequence of octet is complex, there's a second
    // level of nested encapuslation here.

    1, 0, // name omitted:  "context_data"
    CORBA::tk_sequence,   // sequence typecode
    16, // length of encapsulation 2

    // START bytes of encapsulation 2 (sequence params)
    1, // native endian + padding; "tricky"
    1, 0, // type ID omitted:  null string
    CORBA::tk_octet, // (sequence of) octet
    0, // ... unbounded length
    // END bytes of encapsulation 2 (sequence params)

    // END bytes of encapsulation 1 (struct params)

    // SECOND sequence param:  bound of sequence (none)
    0 // unbounded seq of ServiceContext
    // END bytes of encapsulation 0 (sequence params)
  };

  TC_ServiceContextList =
    new CORBA::TypeCode (CORBA::tk_sequence,
                         sizeof _oc_svc_ctx_list,
                         (char *) &_oc_svc_ctx_list,
                         1,
                         sizeof (TAO_GIOP_ServiceContextList));

  static const CORBA::ULong oc_completion_status [] =
  {
    TAO_ENCAP_BYTE_ORDER, // byte order flag, tricky
    0, 0,                 // type ID omitted
    3,                    // three members
    0, 0,                 // ... whose names are all omitted
    0, 0,
    0, 0
  };

  TC_completion_status =
    new CORBA::TypeCode (CORBA::tk_enum,
                         sizeof oc_completion_status,
                         (char *) &oc_completion_status,
                         1,
                         sizeof (CORBA::CompletionStatus));
}

// destroy all the typecodes owned by the ORB
void
TAO_TypeCodes::fini (void)
{
  // Initialize all the standard typecodes owned by the ORB

  // Null and void
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

  // a string/wstring have a simple parameter list that indicates the length
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

  // additional typecodes in the CORBA namespace
  CORBA::release (CORBA::_tc_Policy);

  CORBA::release (CORBA::_tc_PolicyList);

  CORBA::release (CORBA::_tc_Current);

  CORBA::release (CORBA::_tc_Identifier);

  CORBA::release (CORBA::_tc_RepositoryId);

  CORBA::release (CORBA::_tc_ORBid);

#if !defined(TAO_HAS_MINIMUM_CORBA)
  CORBA::release (CORBA_ORB::_tc_InconsistentTypeCode);

  CORBA::release (CORBA::_tc_ConstructionPolicy);

  CORBA::release (CORBA::_tc_NamedValue);

  CORBA::release (CORBA::_tc_WrongTransaction);
#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::release (CORBA_ORB::_tc_ObjectId);

  CORBA::release (CORBA::_tc_PolicyType);

  CORBA::release (CORBA::_tc_PolicyErrorCode);

  CORBA::release (CORBA::_tc_PolicyError);

  CORBA::release (CORBA::_tc_InvalidPolicies);

  CORBA::release (CORBA::_tc_PolicyTypeSeq);

  CORBA::release (CORBA::_tc_SetOverrideType);

  CORBA::release (CORBA::_tc_PolicyManager);

  CORBA::release (CORBA::_tc_PolicyCurrent);

  CORBA::release (CORBA::_tc_DomainManager);

  CORBA::release (CORBA::_tc_DomainManagerList);

  // Service types
   //= Service type
   CORBA::release (CORBA::_tc_ServiceType);
   CORBA::release (CORBA::_tc_ServiceOption);
   CORBA::release (CORBA::_tc_ServiceDetailType);
   CORBA::release (CORBA::_tc_ServiceDetail);
   CORBA::release (CORBA::_tc_ServiceInformation);

  // TAO specific
  CORBA::release (TC_opaque);

  CORBA::release (TC_ServiceContextList);

  CORBA::release (TC_completion_status);
}
