// -*- C++ -*-

//=============================================================================
/**
 *  @file   corbafwd.h
 *
 *  $Id$
 *
 * Forward declare the basic types used in the ORB
 * implementation. Regular users of the ORB should only include
 * tao/corba.h (generally through the stub/skeleton headers), but the
 * implementation of the ORB itself should include this file.  That
 * reduces interdependency and speeds up compilations and
 * recompilations.
 *
 *  @author Carlos O'Ryan
 *  @author Chris Cleeland
 *  @author Douglas C. Schmidt
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef TAO_CORBAFWD_H
#define TAO_CORBAFWD_H

#include "ace/pre.h"

#include "ace/CDR_Base.h"

// @@ Why is this here?  It should be moved to IOPC.h.
//       -Ossama
#if defined (HPUX)
#  if defined (IOR)
   /* HP-UX 11.11 defines IOR in /usr/include/pa/inline.h
      and we don't want that definition, see IOPC.h */
#  undef IOR
#  endif /* IOR */
#endif /* HPUX */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"

#include "ace/OS_Memory.h"


#if defined (_MSC_VER) || defined (__BORLANDC__)
# ifdef   _DEBUG                  /* convert from VC++ convention ... */
#   define  TAO_DEBUG             /* ... to normal convention */
# endif
#endif  /* _MSC_VER || __BORLANDC__ */

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Define symbolic names for the ORB collocation strategies.
namespace TAO_Collocation_Strategies
{
  enum {
    /// i.e no collocation.
    CS_REMOTE_STRATEGY,

    /// Calls to the collocated object are forwarded by the POA.
    CS_THRU_POA_STRATEGY,

    /// Calls to the collocated object are made directly to its
    /// servant.
    CS_DIRECT_STRATEGY,

    /// This value should always be the last value in the enumeration.
    /// It provides the count for the number of collocation
    /// strategies.
    CS_LAST
  };
}

// forward declare sequences.
template <class T> class TAO_Unbounded_Sequence;
/**
 * @class CORBA
 *
 * @brief Provides the CORBA namespace.
 *
 * Declarations of CORBA primitive types, in addition to forward
 * declarations of other types (e.g. interfaces, etc).
 */
namespace CORBA
{
  /**
   * @name CORBA Primitive Types
   *
   * Declarations of all CORBA primitive types.
   */
  //@{
  typedef ACE_CDR::Boolean Boolean;
  typedef Boolean &Boolean_out;

  typedef ACE_CDR::Octet Octet;
  typedef Octet  &Octet_out;

  typedef ACE_CDR::Short Short;
  typedef Short &Short_out;

  typedef ACE_CDR::UShort UShort;
  typedef UShort &UShort_out;

  typedef ACE_CDR::Long Long;
  typedef Long &Long_out;

  typedef ACE_CDR::ULong ULong;
  typedef ULong &ULong_out;

  typedef ACE_CDR::LongLong LongLong;
  typedef LongLong &LongLong_out;

  typedef ACE_CDR::ULongLong ULongLong;
  typedef ULongLong &ULongLong_out;

  typedef ACE_CDR::Float Float;
  typedef Float &Float_out;

  typedef ACE_CDR::Double Double;
  typedef Double &Double_out;

  typedef ACE_CDR::LongDouble LongDouble;
  typedef LongDouble &LongDouble_out;

  typedef ACE_CDR::Char Char;
  typedef Char &Char_out;

  typedef ACE_CDR::WChar WChar;
  typedef WChar &WChar_out;
  //@}

  // enum values defined in tao/NVList.h, bitwise ORed.
  typedef ULong Flags;

  // CORBA::TypeCode typedefs.
  class TypeCode;
  typedef TypeCode * TypeCode_ptr;
  class TypeCode_var;
  class TypeCode_out;

  /**
   * @name CORBA String Memory Management
   *
   * CORBA string memory management functions.
   */
  //@{
  TAO_NAMESPACE_INLINE_FUNCTION char * string_alloc (ULong len);
  extern TAO_Export  char * string_dup (const char *);
  TAO_NAMESPACE_INLINE_FUNCTION void string_free (char *);
  //@}

  class String_var;
  class String_out;

  /**
   * @name CORBA Wide String Memory Management
   *
   * CORBA wide string memory management functions.
   */
  //@{
  TAO_NAMESPACE_INLINE_FUNCTION WChar * wstring_alloc (ULong len);
  extern TAO_Export  WChar * wstring_dup (const WChar * const);
  TAO_NAMESPACE_INLINE_FUNCTION void wstring_free (WChar * const);
  //@}

  class WString_var;
  class WString_out;

  class StringSeq;
  class StringSeq_var;
  class StringSeq_out;
  extern TAO_Export TypeCode_ptr _tc_StringSeq;

  class WStringSeq;
  class WStringSeq_var;
  class WStringSeq_out;
  extern TAO_Export TypeCode_ptr _tc_WStringSeq;

  class OctetSeq;
  class OctetSeq_var;
  class OctetSeq_out;
  extern TAO_Export TypeCode_ptr _tc_OctetSeq;

  class Any;
  class Any_var;
  class Any_out;
  typedef Any * Any_ptr;

  class Environment;
  class Environment_var;
  class Environment_out;
  typedef Environment * Environment_ptr;

  class Exception;
  typedef Exception * Exception_ptr;
  class SystemException;
  class UserException;

  class Request;
  typedef Request * Request_ptr;
  class Request_var;
  class Request_out;

  class NamedValue;
  typedef NamedValue * NamedValue_ptr;
  class NamedValue_var;
  class NamedValue_out;

  class NVList;
  typedef NVList * NVList_ptr;
  class NVList_var;
  class NVList_out;

  class Context;
  typedef Context * Context_ptr;
  class Context_var;
  class Context_out;

  class ContextList;
  typedef ContextList *ContextList_ptr;
  class ContextList_var;
  class ContextList_out;

  class Object;
  typedef Object *Object_ptr;
  class Object_var;
  class Object_out;

  class LocalObject;
  typedef LocalObject *LocalObject_ptr;
  class LocalObject_var;
  class LocalObject_out;

  class Principal;
  typedef Principal *Principal_ptr;
  class Principal_var;
  class Principal_out;

  class ORB;
  typedef ORB *ORB_ptr;
  class ORB_var;
  class ORB_out;

  class ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;

  class ExceptionList;
  class ExceptionList_var;
  typedef ExceptionList *ExceptionList_ptr;

  class ImplementationDef;
  typedef ImplementationDef *ImplementationDef_ptr;

  class Current;
  typedef Current *Current_ptr;
  class Current_var;
  class Current_out;

  class TypeCodeFactory;
  typedef TypeCodeFactory *TypeCodeFactory_ptr;
  class TypeCodeFactory_out;
  class TypeCodeFactory_var;

  class ValueBase;
  typedef ValueBase *ValueBase_ptr;
  class ValueBase_var;
  class ValueBase_out;
  class ValueFactoryBase;
  class ValueFactoryBase_var;
  typedef ValueFactoryBase *ValueFactory;
  typedef ValueFactoryBase_var ValueFactory_var;
  class DefaultValueRefCountBase;

  typedef CORBA::Short Visibility;
  typedef CORBA::Short_out Visibility_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_Visibility;

  extern TAO_Export const CORBA::Short PRIVATE_MEMBER;
  extern TAO_Export const CORBA::Short PUBLIC_MEMBER;

  typedef CORBA::Short ValueModifier;
  typedef CORBA::Short_out ValueModifier_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ValueModifier;

  extern TAO_Export const CORBA::Short VM_NONE;
  extern TAO_Export const CORBA::Short VM_CUSTOM;
  extern TAO_Export const CORBA::Short VM_ABSTRACT;
  extern TAO_Export const CORBA::Short VM_TRUNCATABLE;

  class AbstractBase;
  typedef AbstractBase * AbstractBase_ptr;
  class AbstractBase_var;
  class AbstractBase_out;

  enum CompletionStatus
  {
    // = Completion Status for System exceptions

    COMPLETED_YES,     // successful or exceptional completion
    COMPLETED_NO,      // didn't change any state; retry is OK
    COMPLETED_MAYBE    // can't say what happened; retry unsafe
  };

  enum exception_type
  {
    // = Exception type.

    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION
  };

  extern TAO_Export TypeCode_ptr _tc_exception_type;

  /**
   * @name CORBA System Exceptions
   *
   * Forward Declarations of all CORBA System Exceptions.
   */
  //@{
  class UNKNOWN;
  class BAD_PARAM;
  class NO_MEMORY;
  class IMP_LIMIT;
  class COMM_FAILURE;
  class INV_OBJREF;
  class OBJECT_NOT_EXIST;
  class NO_PERMISSION;
  class INTERNAL;
  class MARSHAL;
  class INITIALIZE;
  class NO_IMPLEMENT;
  class BAD_TYPECODE;
  class BAD_OPERATION;
  class NO_RESOURCES;
  class NO_RESPONSE;
  class PERSIST_STORE;
  class BAD_INV_ORDER;
  class TRANSIENT;
  class FREE_MEM;
  class INV_IDENT;
  class INV_FLAG;
  class INTF_REPOS;
  class BAD_CONTEXT;
  class OBJ_ADAPTER;
  class DATA_CONVERSION;
  class INV_POLICY;
  class REBIND;
  class TIMEOUT;
  class TRANSACTION_UNAVAILABLE;
  class TRANSACTION_MODE;
  class TRANSACTION_REQUIRED;
  class TRANSACTION_ROLLEDBACK;
  class INVALID_TRANSACTION;
  class CODESET_INCOMPATIBLE;
  class BAD_QOS;
  class INVALID_ACTIVITY;
  class ACTIVITY_COMPLETED;
  class ACTIVITY_REQUIRED;
  //@}

  class UnknownUserException;

  /**
   * @name CORBA::is_nil Methods
   *
   * Overloaded CORBA::is_nil() methods.
   */
  //@{
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Object_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Environment_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (TypeCode_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ORB_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Principal_ptr);
  extern TAO_Export Boolean is_nil (Request_ptr);
  extern TAO_Export Boolean is_nil (ServerRequest_ptr req);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NVList_ptr);
  extern TAO_Export Boolean is_nil (Context_ptr);
  //@}

  /**
   * @name CORBA::release Methods
   *
   * Overloaded CORBA::release() methods.
   */
  //@{
  TAO_NAMESPACE_INLINE_FUNCTION void release (Object_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Environment_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Principal_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (TypeCode_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (ORB_ptr);
  extern TAO_Export void release (Request_ptr);
  extern TAO_Export void release (ServerRequest_ptr req);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NVList_ptr);
  extern TAO_Export void release (Context_ptr);
  //@}

  enum TCKind
  {
    // = Kinds of typecodes.

    // Do not change these enum values, or duplicate them if you need
    // to add values.  They are used to index tables, and if you
    // change the values you'll need to find and update all of those
    // tables.  The values are also part of the Common Data
    // Representation, and hence are part of IIOP and other ORB
    // protocols.

    tk_null               = 0,
    tk_void               = 1,
    tk_short              = 2,
    tk_long               = 3,
    tk_ushort             = 4,
    tk_ulong              = 5,
    tk_float              = 6,
    tk_double             = 7,
    tk_boolean            = 8,
    tk_char               = 9,
    tk_octet              = 10,
    tk_any                = 11,
    tk_TypeCode           = 12,
    tk_Principal          = 13,
    tk_objref             = 14,
    tk_struct             = 15,
    tk_union              = 16,
    tk_enum               = 17,
    tk_string             = 18,
    tk_sequence           = 19,
    tk_array              = 20,
    tk_alias              = 21,
    tk_except             = 22,

    tk_longlong           = 23,
    tk_ulonglong          = 24,
    tk_longdouble         = 25,
    tk_wchar              = 26,
    tk_wstring            = 27,
    tk_fixed              = 28,
    tk_value              = 29,
    tk_value_box          = 30,
    tk_native             = 31,
    tk_abstract_interface = 32,
    tk_local_interface    = 33,
    tk_component          = 34,
    tk_home               = 35,
    tk_event              = 36,

    // This symbol is not defined by CORBA 3.0.  It's used to speed up
    // dispatch based on TCKind values, and lets many important ones
    // just be table lookups.  It must always be the last enum value!!

    TC_KIND_COUNT
  };
  typedef TCKind &TCKind_out;

  /**
   * @name TypeCode Constants
   *
   * All the TypeCode constants
   */
  //@{
  extern TAO_Export TypeCode_ptr _tc_null;
  extern TAO_Export TypeCode_ptr _tc_void;
  extern TAO_Export TypeCode_ptr _tc_short;
  extern TAO_Export TypeCode_ptr _tc_long;
  extern TAO_Export TypeCode_ptr _tc_ushort;
  extern TAO_Export TypeCode_ptr _tc_ulong;
  extern TAO_Export TypeCode_ptr _tc_float;
  extern TAO_Export TypeCode_ptr _tc_double;
  extern TAO_Export TypeCode_ptr _tc_boolean;
  extern TAO_Export TypeCode_ptr _tc_char;
  extern TAO_Export TypeCode_ptr _tc_octet;
  extern TAO_Export TypeCode_ptr _tc_any;
  extern TAO_Export TypeCode_ptr _tc_TypeCode;
  extern TAO_Export TypeCode_ptr _tc_Principal;
  extern TAO_Export TypeCode_ptr _tc_Object;
  extern TAO_Export TypeCode_ptr _tc_string;
  extern TAO_Export TypeCode_ptr _tc_longlong;
  extern TAO_Export TypeCode_ptr _tc_ulonglong;
  extern TAO_Export TypeCode_ptr _tc_longdouble;
  extern TAO_Export TypeCode_ptr _tc_wchar;
  extern TAO_Export TypeCode_ptr _tc_wstring;

#define TAO_SYSTEM_EXCEPTION_LIST \
  TAO_SYSTEM_EXCEPTION(UNKNOWN); \
  TAO_SYSTEM_EXCEPTION(BAD_PARAM); \
  TAO_SYSTEM_EXCEPTION(NO_MEMORY); \
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT); \
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE); \
  TAO_SYSTEM_EXCEPTION(INV_OBJREF); \
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST); \
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION); \
  TAO_SYSTEM_EXCEPTION(INTERNAL); \
  TAO_SYSTEM_EXCEPTION(MARSHAL); \
  TAO_SYSTEM_EXCEPTION(INITIALIZE); \
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT); \
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE); \
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION); \
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES); \
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE); \
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE); \
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER); \
  TAO_SYSTEM_EXCEPTION(TRANSIENT); \
  TAO_SYSTEM_EXCEPTION(FREE_MEM); \
  TAO_SYSTEM_EXCEPTION(INV_IDENT); \
  TAO_SYSTEM_EXCEPTION(INV_FLAG); \
  TAO_SYSTEM_EXCEPTION(INTF_REPOS); \
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT); \
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER); \
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION); \
  TAO_SYSTEM_EXCEPTION(INV_POLICY); \
  TAO_SYSTEM_EXCEPTION(REBIND); \
  TAO_SYSTEM_EXCEPTION(TIMEOUT); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED); \
  TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK); \
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION); \
  TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE); \
  TAO_SYSTEM_EXCEPTION(BAD_QOS); \
  TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY); \
  TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED); \
  TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED);

  // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
  extern TAO_Export TypeCode_ptr _tc_ ## name
  TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION
  //@}

  extern TAO_Export TypeCode_ptr _tc_UnknownUserException;

  extern TAO_Export TypeCode_ptr _tc_Current;

  extern TAO_Export TypeCode_ptr _tc_NamedValue;

  enum
  {
    // = Flags for NVList add methods
    ARG_IN                  = 0x01,
    ARG_OUT                 = 0x02,
    ARG_INOUT               = 0x04,
    IN_COPY_VALUE           = 0x08,
    OUT_LIST_MEMORY         = 0x10,
    DEPENDENT_LIST          = 0x20,

    // = Flags for Context methods
    CTX_RESTRICT_SCOPE      = 0x40,
    CTX_DELETE_DESCENDENTS  = 0x80,

    // = Flags for deferred synchronous methods
    INV_NO_RESPONSE         = 0x100,
    INV_TERM_ON_ERR         = 0x200,
    RESP_NO_WAIT            = 0x400
  };

  typedef char * ORBid;
  typedef CORBA::String_var ORBid_var;
  typedef CORBA::String_out ORBid_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ORBid;

  // = Service definitions
  typedef CORBA::UShort ServiceType;
  typedef CORBA::UShort_out ServiceType_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ServiceType;

  typedef CORBA::ULong ServiceOption;
  typedef CORBA::ULong_out ServiceOption_out;
  extern TAO_Export  CORBA::TypeCode_ptr _tc_ServiceOption;

  typedef CORBA::ULong ServiceDetailType;
  typedef CORBA::ULong_out ServiceDetailType_out;
  extern TAO_Export  CORBA::TypeCode_ptr _tc_ServiceDetailType;

  struct ServiceDetail;
  class ServiceDetail_var;
  class ServiceDetail_out;
  typedef ServiceDetail * ServiceDetail_ptr;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ServiceDetail;

  struct ServiceInformation;
  class ServiceInformation_var;
  class ServiceInformation_out;
  typedef ServiceInformation * ServiceInformation_ptr;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ServiceInformation;

  // ****************************************************************

  enum ParameterMode
    {
      PARAM_IN,
      PARAM_OUT,
      PARAM_INOUT
    };

#if (TAO_HAS_MINIMUM_CORBA == 0)
  class ConstructionPolicy;
  typedef ConstructionPolicy * ConstructionPolicy_ptr;
#endif /* ! TAO_HAS_MINIMUM_CORBA */

  /**
   * @name ORB Initialization
   *
   * There could be a single version of these methods, but g++ 2.7.2
   * gets horribly confused if we used CORBA::default_environment() at
   * this point.
   */
  //@{
  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      char * argv[],
                                      const char * orb_name = 0);
  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      char * argv[],
                                      const char * orb_name,
                                      CORBA::Environment &ACE_TRY_ENV);
  //@}

  // = TAO extensions...

  /// Obtain the thread-specific default environment.  This is
  /// obsolete and only left here for backwards compatibility.
  TAO_NAMESPACE_INLINE_FUNCTION CORBA::Environment& default_environment (void);

}
// end of namespace CORBA

// ****************************************************************

/// Provide a simple function to access the TSS default environment.
/**
 * @note We tried with CORBA::default_environment() and others.
 */
TAO_Export CORBA::Environment& TAO_default_environment (void);

// ****************************************************************

// Several OMG assigned numbers, for a complete list check:
//
// ftp://ftp.omg.org/pub/docs/ptc/99-05-02.txt

//
// For details on how to ask more numbers check:
//
// http://www.omg.org/cgi-bin/doc?ptc/99-02-01
//

/**
 * @name TAO-Specific Profile IDs
 *
 * The TAO @c IOP::ProfileId range 0x54414f00 - 0x54414f0f has been
 * reserved with the OMG.
 *
 * @note It is *NOT* necessary to list your own protocols here.
 *
 * @note The values below are subject to change at any point.
 */
//@{
/// Local IPC (Unix Domain)
const CORBA::ULong TAO_TAG_UIOP_PROFILE = 0x54414f00U;

/// ATM/AAL5
// const CORBA::ULong TAO_TAG_AIOP_PROFILE = 0x54414f01U;

/// Shared memory
const CORBA::ULong TAO_TAG_SHMEM_PROFILE = 0x54414f02U;

/// Message Queue
// const CORBA::ULong TAO_TAG_MSGQ_PROFILE = 0x54414f03U;

/// UDP
const CORBA::ULong TAO_TAG_UDP_PROFILE = 0x54414f04U;

/// IP/Multicast
// const CORBA::ULong TAO_TAG_MCAST_PROFILE = 0x54414f05U;

/// Compact/PCI
// const CORBA::ULong TAO_TAG_CPCI_PROFILE = 0x54414f06U;

/// VME Bus
// const CORBA::ULong TAO_TAG_VME_PROFILE = 0x54414f07U;

/// NT Named Pipes
// const CORBA::ULong TAO_TAG_NTNP_PROFILE = 0x54414f08U;

/// HTTP-NG
// const CORBA::ULong TAO_TAG_HTTPNG_PROFILE = 0x54414f09U;

/// Pipe
// const CORBA::ULong TAO_TAG_PIPE_PROFILE = 0x54414f0AU;

/// ????
// const CORBA::ULong TAO_TAG_XXXX_PROFILE = 0x54414f0BU;

/// Unreliable IP Multicast
const CORBA::ULong TAO_TAG_UIPMC_PROFILE = 0x54414f0CU;

/// Boeing Custom Protocol
const CORBA::ULong TAO_TAG_BIOP_PROFILE = 0x54414f0FU;
//@}

/// The TAO-specific OMG assigned value for the TAG_ORB_TYPE tagged
/// component.
/**
 * This number was assigned by the OMG.  Do *NOT* change.  The ASCII
 * representation is "TA\x00".  If necessary, we can request more ORB
 * types later.
 */
const CORBA::ULong TAO_ORB_TYPE = 0x54414f00U;

/**
 * @name TAO-Specific Profile Tagged Component IDs
 *
 * The TAO @c IOP::TaggedComponent range 0x54414f00 - 0x54414f0f has
 * been reserved with the OMG.
 */
//@{

// This tag is no longer used in TAO.  It will not be reused for a
// period of time to preserve backward compatibility with servers
// running older versions of TAO.
// #define TAO_TAG_PRIORITY 0x54414f00U

//  Tag 0x54414f01U is defined/used by SSLIOP.

/// Tag for storing multiple endpoints within a single profile.
/**
 * This is TAO-specific, and is mostly used by TAO's RTCORBA support.
 * The standard TAG_ALTERNATE_IIOP_ADDRESSES tagged component is the
 * portable alternative.
 */
const CORBA::ULong TAO_TAG_ENDPOINTS =  0x54414f02U;

//const CORBA::ULong TAO_TAG_SOME_OTHER_TAG = 0x54414f03U;

//@}

/**
 * @name TAO-Specific @c IOP::ServiceId Values
 *
 * The TAO @c IOP::ServiceId range 0x54414f00 - 0x54414f0f has been
 * reserved with the OMG.
 * @par
 * These values are used in service contexts that are inserted into a
 * request or reply @c IOP::ServiceContextList.
 */
//@{

/// A dummy service context that is inserted in the service context
/// list to preserve the alignment in DSI based gateways, so no
/// marshaling/demarshaling is required.
/**
 * @note This is *extremely* brittle but works.
 */
const CORBA::ULong TAO_SVC_CONTEXT_ALIGN = 0x54414f00U;

namespace CORBA
{
  /// The OMG Vendor Minor Codeset ID.
  /**
   * This number is reserved by the OMG as a prefix to all the
   * standard system exception minor codes.  Check the CORBA/IIOP
   * specification for details.
   */
  const CORBA::ULong OMGVMCID = 0x4f4d0000U;
}

// This number was assigned by the OMG.  Do *NOT* change at random.
// The ASCII representation is TA0xxxx, close enough since they only
// take 20 bits, the first 16 are TA, the next 4 are 0000.  Remember
// that we can only play with the last 12 bits, TAO_MAX_MINOR_CODE is
// there to remind us of that.
const CORBA::ULong TAO_DEFAULT_MINOR_CODE = 0x54410000U;
const CORBA::ULong TAO_MAX_MINOR_CODE = 0x54410fffU;

/**
 * @name Minor Code Encoding
 *
 * Encode the location in 5 bits, and the errno in 7 bits:
 *
 * @verbatim
 * 0x   0101 0100   0100 0001   0000   ____ _     ___ ____
 *          T           A        0    location      errno
 * @endverbatim
 **/
//@{
/**
 * @name Location Encoding
 *
 * The location encoding is the 5 bits, after the @ errno encoding.
 */
//@{
const CORBA::ULong TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE = (0x01U << 7);
const CORBA::ULong TAO_INVOCATION_SEND_REQUEST_MINOR_CODE     = (0x02U << 7);
const CORBA::ULong TAO_POA_DISCARDING                         = (0x03U << 7);
const CORBA::ULong TAO_POA_HOLDING                            = (0x04U << 7);
const CORBA::ULong TAO_UNHANDLED_SERVER_CXX_EXCEPTION         = (0x05U << 7);
const CORBA::ULong TAO_INVOCATION_RECV_REQUEST_MINOR_CODE     = (0x06U << 7);
const CORBA::ULong TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL  = (0x07U << 7);
const CORBA::ULong TAO_MPROFILE_CREATION_ERROR                = (0x08U << 7);
const CORBA::ULong TAO_TIMEOUT_CONNECT_MINOR_CODE             = (0x09U << 7);
const CORBA::ULong TAO_TIMEOUT_SEND_MINOR_CODE                = (0x0AU << 7);
const CORBA::ULong TAO_TIMEOUT_RECV_MINOR_CODE                = (0x0BU << 7);
const CORBA::ULong TAO_IMPLREPO_MINOR_CODE                    = (0x0CU << 7);
const CORBA::ULong TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE   = (0x0DU << 7);
const CORBA::ULong TAO_ORB_CORE_INIT_LOCATION_CODE            = (0x0EU << 7);
const CORBA::ULong TAO_POLICY_NARROW_CODE                     = (0x0FU << 7);
const CORBA::ULong TAO_GUARD_FAILURE                          = (0x10U << 7);
const CORBA::ULong TAO_POA_BEING_DESTROYED                    = (0x11U << 7);
const CORBA::ULong TAO_POA_INACTIVE                           = (0x12U << 7);
const CORBA::ULong TAO_CONNECTOR_REGISTRY_INIT_LOCATION_CODE  = (0x13U << 7);
const CORBA::ULong TAO_AMH_REPLY_LOCATION_CODE                = (0x14U << 7);
// *Don't* use TAO_<location>_MINOR_CODE greater than 0x1FU!
//@}

/**
 * @name @c errno Encoding
 *
 * The @c errno encoding is located in the bottom 7 bits.
 */
//@{
const CORBA::ULong TAO_UNSPECIFIED_MINOR_CODE        = 0x0U;
const CORBA::ULong TAO_ETIMEDOUT_MINOR_CODE          = 0x1U;
const CORBA::ULong TAO_ENFILE_MINOR_CODE             = 0x2U;
const CORBA::ULong TAO_EMFILE_MINOR_CODE             = 0x3U;
const CORBA::ULong TAO_EPIPE_MINOR_CODE              = 0x4U;
const CORBA::ULong TAO_ECONNREFUSED_MINOR_CODE       = 0x5U;
const CORBA::ULong TAO_ENOENT_MINOR_CODE             = 0x6U;
const CORBA::ULong TAO_EBADF_MINOR_CODE              = 0x7U;
const CORBA::ULong TAO_ENOSYS_MINOR_CODE             = 0x8U;
const CORBA::ULong TAO_EPERM_MINOR_CODE              = 0x9U;
const CORBA::ULong TAO_EAFNOSUPPORT_MINOR_CODE       = 0xAU;
const CORBA::ULong TAO_EAGAIN_MINOR_CODE             = 0xBU;
const CORBA::ULong TAO_ENOMEM_MINOR_CODE             = 0xCU;
const CORBA::ULong TAO_EACCES_MINOR_CODE             = 0xDU;
const CORBA::ULong TAO_EFAULT_MINOR_CODE             = 0xEU;
const CORBA::ULong TAO_EBUSY_MINOR_CODE              = 0xFU;
const CORBA::ULong TAO_EEXIST_MINOR_CODE             = 0x10U;
const CORBA::ULong TAO_EINVAL_MINOR_CODE             = 0x11U;
const CORBA::ULong TAO_ECOMM_MINOR_CODE              = 0x12U;
const CORBA::ULong TAO_ECONNRESET_MINOR_CODE         = 0x13U;
const CORBA::ULong TAO_ENOTSUP_MINOR_CODE            = 0x14U;
// *Don't* use TAO_<errno>_MINOR_CODE greater than 0x7FU!
//@}
//@}

// These numbers are assigned by the OpenGroup, a database is
// available at
//
// ftp://ftp.opengroup.org/pub/code_set_registry/
//
#define TAO_CODESET_ID_ISO8859_1 0x00010001U
#define TAO_CODESET_ID_UNICODE   0x00010109U

// These are the default codesets that TAO declares, of course they
// will be different on each platform, once the complete support for
// character sets is implemented
#define TAO_DEFAULT_CHAR_CODESET_ID  TAO_CODESET_ID_ISO8859_1
#define TAO_DEFAULT_WCHAR_CODESET_ID TAO_CODESET_ID_UNICODE

#define TAO_INVALID_PRIORITY -1

// An hash define for the regular two way operation
#define TAO_TWOWAY_RESPONSE_FLAG 255
// ****************************************************************

class TAO_InputCDR;
class TAO_OutputCDR;

// A helper class to handle the various kinds of octet sequences used
// inside the ORB.

typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;
extern TAO_Export CORBA::TypeCode_ptr TC_opaque;

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_opaque&);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_opaque&);

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, const CORBA::TCKind &);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA::TCKind &);


#if defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_CORBAFWD_H */
