// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//   corbafwd.h
//
// = DESCRIPTION
//   Forward declare the basic types used in the ORB
//   implementation. Regular users of the ORB should only include
//   tao/corba.h, but the implementation of the ORB itself should
//   include this file.  That reduces interdependency and speed up
//   compilations and recompilations.
//
// = AUTHOR
//   Carlos O'Ryan
//   Chris Cleeland
//   Douglas C. Schmidt
//   based on work  Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#ifndef TAO_CORBAFWD_H
#define TAO_CORBAFWD_H

#include "ace/pre.h"

#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/try_macros.h"
#include "tao/varbase.h"
#include "tao/TAO_Export.h"

// @@ NW: Disable messing with the alignment for now.
// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if defined (_MSC_VER) || defined (__BORLANDC__)
# ifdef   _DEBUG                  // convert from VC++ convention ...
#   define  TAO_DEBUG             // ... to normal convention
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

// Forward declarations of some data types are needed.

class CORBA_Any;
class CORBA_Any_var;
class CORBA_Any_out;
typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_TypeCode;
class CORBA_TypeCode_var;
class CORBA_TypeCode_out;
typedef class CORBA_TypeCode *CORBA_TypeCode_ptr;

class CORBA_Exception;
class CORBA_UserException;
class CORBA_SystemException;
class CORBA_UnknownUserException;
class CORBA_ExceptionList;
class CORBA_ExceptionList_var;
typedef class CORBA_Exception *CORBA_Exception_ptr;

// all the system exception classes, the list is used as follows:
// - Define TAO_SYSTEM_EXCEPTION
// - Use the list to expand that macro over all the system exceptions.
// - Undefine TAO_SYSTEM_EXCEPTION
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
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION);

#define TAO_SYSTEM_EXCEPTION(name) \
  class CORBA_ ## name
TAO_SYSTEM_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#if (TAO_HAS_MINIMUM_CORBA == 0)

class CORBA_WrongTransaction;
typedef CORBA_WrongTransaction *CORBA_WrongTransaction_ptr;

class CORBA_Request;
class CORBA_Request_var;
class CORBA_Request_out;
typedef class CORBA_Request *CORBA_Request_ptr;

class CORBA_ORB_RequestSeq;
class CORBA_ORB_RequestSeq_var;
class CORBA_ORB_RequestSeq_out;

class CORBA_NamedValue;
class CORBA_NamedValue_var;
class CORBA_NamedValue_out;
typedef class CORBA_NamedValue *CORBA_NamedValue_ptr;


class CORBA_NVList;
class CORBA_NVList_var;
class CORBA_NVList_out;
typedef class CORBA_NVList *CORBA_NVList_ptr;

class CORBA_Context;
class CORBA_Context_var;
class CORBA_Context_out;
typedef class CORBA_Context *CORBA_Context_ptr;

class CORBA_ContextList;
class CORBA_ContextList_var;
class CORBA_ContextList_out;
typedef class CORBA_ContextList *CORBA_ContextList_ptr;

class CORBA_ConstructionPolicy;
class CORBA_ConstructionPolicy_var;
class CORBA_ConstructionPolicy_out;
typedef class CORBA_ConstructionPolicy *CORBA_ConstructionPolicy_ptr;

#endif /* ! TAO_HAS_MINIMUM_CORBA */

class CORBA_ORB;
class CORBA_ORB_var;
class CORBA_ORB_out;
typedef class CORBA_ORB *CORBA_ORB_ptr;

class CORBA_Object;
class CORBA_Object_var;
class CORBA_Object_out;
typedef class CORBA_Object *CORBA_Object_ptr;

class CORBA_LocalObject;
class CORBA_LocalObject_var;
class CORBA_LocalObject_out;
typedef class CORBA_LocalObject *CORBA_LocalObject_ptr;

class CORBA_Current;
class CORBA_Current_var;
class CORBA_Current_out;
typedef class CORBA_Current *CORBA_Current_ptr;

class TAO_ServerRequest;
class CORBA_ServerRequest;
typedef class CORBA_ServerRequest *CORBA_ServerRequest_ptr;

class CORBA_Environment;
class CORBA_Environment_var;
class CORBA_Environment_out;
typedef class CORBA_Environment *CORBA_Environment_ptr;

// = Service Forward declaration
struct CORBA_ServiceDetail;
class CORBA_ServiceDetail_var;
class CORBA_ServiceDetail_out;
typedef CORBA_ServiceDetail * CORBA_ServiceDetail_ptr;

struct CORBA_ServiceInformation;
class CORBA_ServiceInformation_var;
class CORBA_ServiceInformation_out;
typedef CORBA_ServiceInformation * CORBA_ServiceInformation_ptr;

class CORBA_Principal;
class CORBA_Principal_var;
class CORBA_Principal_out;
typedef class CORBA_Principal *CORBA_Principal_ptr;

typedef class CORBA_ImplementationDef *CORBA_ImplementationDef_ptr;

class CORBA_InterfaceDef;
class CORBA_InterfaceDef_var;
class CORBA_InterfaceDef_out;
typedef class CORBA_InterfaceDef *CORBA_InterfaceDef_ptr;

#ifdef TAO_HAS_VALUETYPE
class CORBA_ValueBase;
class CORBA_ValueFactoryBase;
typedef CORBA_ValueFactoryBase *CORBA_ValueFactory_ptr;
class CORBA_DefaultValueRefCountBase;
#endif /* TAO_HAS_VALUETYPE */

class CORBA_String_var;
class CORBA_String_out;
class CORBA_WString_var;
class CORBA_WString_out;

class CORBA_ExceptionList;

class CORBA_PolicyError;
typedef CORBA_PolicyError *CORBA_PolicyError_ptr;

class CORBA_InvalidPolicies;
typedef CORBA_InvalidPolicies* CORBA_InvalidPolicies_ptr;

class CORBA_PolicyTypeSeq;
class CORBA_PolicyTypeSeq_var;
class CORBA_PolicyTypeSeq_out;

class CORBA_PolicyManager;
class CORBA_PolicyManager_var;
class CORBA_PolicyManager_out;
typedef CORBA_PolicyManager *CORBA_PolicyManager_ptr;

class CORBA_PolicyCurrent;
class CORBA_PolicyCurrent_var;
class CORBA_PolicyCurrent_out;
typedef CORBA_PolicyCurrent *CORBA_PolicyCurrent_ptr;

class CORBA_Policy;
class CORBA_Policy_var;
class CORBA_Policy_out;
typedef CORBA_Policy *CORBA_Policy_ptr;

class CORBA_PolicyList;
class CORBA_PolicyList_var;
class CORBA_PolicyList_out;

class CORBA_DomainManager;
class CORBA_DomainManager_var;
class CORBA_DomainManager_out;
typedef CORBA_DomainManager *CORBA_DomainManager_ptr;

class CORBA_DomainManagerList;
class CORBA_DomainManagerList_var;
class CORBA_DomainManagerList_out;
typedef CORBA_DomainManagerList *CORBA_DomainManagerList_ptr;

class CORBA_PollableSet;
class CORBA_PollableSet_var;
class CORBA_PollableSet_out;
typedef CORBA_PollableSet *CORBA_PollableSet_ptr;

class CORBA_Pollable;
class CORBA_Pollable_var;
class CORBA_Pollable_out;
typedef CORBA_Pollable *CORBA_Pollable_ptr;

class CORBA_DIIPollable;
class CORBA_DIIPollable_var;
class CORBA_DIIPollable_out;
typedef CORBA_DIIPollable *CORBA_DIIPollable_ptr;

class TAO_InputCDR;
class TAO_OutputCDR;
// @@ TODO This should be TAO_TAO_Stub
class TAO_Stub;

class CORBA_ORB_ObjectIdList;
class CORBA_ORB_ObjectIdList_var;
class CORBA_ORB_ObjectIdList_out;
typedef CORBA_ORB_ObjectIdList *CORBA_ORB_ObjectIdList_ptr;

// Forward declarations for INTERFACE REPO..Bala
#if (TAO_HAS_INTERFACE_REPOSITORY == 1)

class CORBA_IRObject;
class CORBA_IRObject_var;
class CORBA_IRObject_out;
typedef CORBA_IRObject *CORBA_IRObject_ptr;

class CORBA_TypeCodeFactory;
class CORBA_TypeCodeFactory_var;
class CORBA_TypeCodeFactory_out;
typedef CORBA_TypeCodeFactory *CORBA_TypeCodeFactory_ptr;

#endif /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

class CORBA_Bounds;
typedef CORBA_Bounds *CORBA_Bounds_ptr;

// enum values defined in tao/NVList.h, bitwise ORed.
typedef u_int CORBA_Flags;

typedef void (*TAO_Skeleton)(
    TAO_ServerRequest &,
    void *,
    void *,
    CORBA_Environment &ACE_TRY_ENV
  );

// forward declare sequences.
template <class T> class TAO_Unbounded_Sequence;
template <class T,class T_var> class TAO_Unbounded_Object_Sequence;

// Provide a simple function to access the TSS default environment.
// We tried with CORBA_Environment::default_environment (),
// CORBA::default_environment() and others.
TAO_Export CORBA_Environment& TAO_default_environment (void);

enum TAO_SERVANT_LOCATION
{
  TAO_SERVANT_FOUND,
  TAO_DEFAULT_SERVANT,
  TAO_SERVANT_MANAGER,
  TAO_SERVANT_NOT_FOUND
};

TAO_NAMESPACE CORBA
{
  // = TITLE
  //    Provides the CORBA namespace.
  //
  // = DESCRIPTION
  //    This class allows the use of CORBA::Foo, as well as CORBA_Foo
  //    for all standard types Foo.

  typedef ACE_CDR::Boolean Boolean;
  typedef Boolean &Boolean_out;
  // Out type for boolean.

  // This and the other CORBA IDL types below are
  // typedef'd to the corresponding CDR types
  // already defined in ACE.
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

  // = String memory management.
  TAO_NAMESPACE_INLINE_FUNCTION char* string_alloc (ULong len);
  TAO_NAMESPACE_STORAGE_CLASS  char* string_dup (const char *);
  TAO_NAMESPACE_INLINE_FUNCTION void string_free (char *);

  typedef CORBA_String_var String_var;
  typedef CORBA_String_out String_out;

  // WChar was standarized in CORBA 2.2, but is still a bit unstable.

  typedef ACE_CDR::WChar WChar;
  typedef WChar &WChar_out;

  // = String memory management routines.
  TAO_NAMESPACE_INLINE_FUNCTION WChar* wstring_alloc (ULong len);
  TAO_NAMESPACE_STORAGE_CLASS  WChar* wstring_dup (const WChar *const);
  TAO_NAMESPACE_INLINE_FUNCTION void wstring_free (WChar *const);

  typedef CORBA_WString_var WString_var;
  typedef CORBA_WString_out WString_out;

  typedef TAO_Unbounded_Sequence<Octet> OctetSeq;

  // = various CORBA defined classes.
  typedef CORBA_Any Any;
  typedef CORBA_Any_var Any_var;
  typedef CORBA_Any_out Any_out;
  typedef Any *Any_ptr;

  typedef CORBA_Environment Environment;
  typedef CORBA_Environment_var Environment_var;
  typedef CORBA_Environment_out Environment_out;
  typedef Environment *Environment_ptr;

  typedef CORBA_Exception Exception;
  typedef CORBA_SystemException SystemException;
  typedef CORBA_UserException  UserException;

#if (TAO_HAS_MINIMUM_CORBA == 0)

  typedef CORBA_Request Request;
  typedef Request *Request_ptr;
  typedef CORBA_Request_var Request_var;
  typedef CORBA_Request_out Request_out;

  typedef CORBA_NamedValue NamedValue;
  typedef NamedValue *NamedValue_ptr;
  typedef CORBA_NamedValue_var NamedValue_var;
  typedef CORBA_NamedValue_out NamedValue_out;

  typedef CORBA_NVList NVList;
  typedef NVList *NVList_ptr;
  typedef CORBA_NVList_var NVList_var;
  typedef CORBA_NVList_out NVList_out;

  typedef CORBA_Context Context;
  typedef CORBA_Context *Context_ptr;
  typedef CORBA_Context_var Context_var;
  typedef CORBA_Context_out Context_out;

  typedef CORBA_ContextList ContextList;
  typedef CORBA_ContextList *ContextList_ptr;
  typedef CORBA_ContextList_var ContextList_var;
  typedef CORBA_ContextList_out ContextList_out;

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef CORBA_Object Object;
  typedef CORBA_Object *Object_ptr;
  typedef CORBA_Object_var Object_var;
  typedef CORBA_Object_out Object_out;

  typedef CORBA_LocalObject LocalObject;
  typedef CORBA_LocalObject *LocalObject_ptr;
  typedef CORBA_LocalObject_var LocalObject_var;
  typedef CORBA_LocalObject_out LocalObject_out;

  typedef CORBA_Principal Principal;
  typedef CORBA_Principal *Principal_ptr;
  typedef CORBA_Principal_var Principal_var;
  typedef CORBA_Principal_out Principal_out;

  typedef CORBA_ORB ORB;
  typedef CORBA_ORB *ORB_ptr;
  typedef CORBA_ORB_var ORB_var;
  typedef CORBA_ORB_out ORB_out;

  typedef CORBA_ServerRequest ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;

  typedef CORBA_TypeCode TypeCode;
  typedef CORBA_TypeCode *TypeCode_ptr;
  typedef CORBA_TypeCode_var TypeCode_var;
  typedef CORBA_TypeCode_out TypeCode_out;

  typedef CORBA_ExceptionList ExceptionList;
  typedef CORBA_ExceptionList_var ExceptionList_var;
  typedef CORBA_ExceptionList *ExceptionList_ptr;

  typedef CORBA_ImplementationDef *ImplementationDef_ptr;

  typedef CORBA_Current Current;
  typedef CORBA_Current *Current_ptr;
  typedef CORBA_Current_var Current_var;
  typedef CORBA_Current_out Current_out;

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
  // Interface Repository Bala===========
  typedef CORBA_IRObject IRObject;
  typedef CORBA_IRObject *IRObject_ptr;
  typedef CORBA_IRObject_out IRObject_out;
  typedef CORBA_IRObject_var IRObject_var;

  typedef CORBA_TypeCodeFactory TypeCodeFactory;
  typedef CORBA_TypeCodeFactory *TypeCodeFactory_ptr;
  typedef CORBA_TypeCodeFactory_out TypeCodeFactory_out;
  typedef CORBA_TypeCodeFactory_var TypeCodeFactory_var;

#endif /* TAO_HAS_INTERFACE_REPOSITORY == 1 */

  typedef CORBA_Bounds Bounds;
  typedef CORBA_Bounds *Bounds_ptr;
#ifdef TAO_HAS_VALUETYPE
  typedef CORBA_ValueBase ValueBase;
  typedef CORBA_ValueBase *ValueBase_ptr;
  typedef CORBA_ValueFactoryBase ValueFactoryBase;
  typedef CORBA_ValueFactoryBase *ValueFactory;
  // as CORBA 2.3a C++ map. 20.17.10 says
  typedef CORBA_ValueFactoryBase *ValueFactory_ptr;
  // own invention, more readable
  typedef CORBA_DefaultValueRefCountBase  DefaultValueRefCountBase;
  TAO_NAMESPACE_INLINE_FUNCTION void add_ref (ValueBase *);
  TAO_NAMESPACE_INLINE_FUNCTION void remove_ref (ValueBase *);
#endif /* TAO_HAS_VALUETYPE */

  // enum values defined in nvlist.hh, bitwise ORed.
  typedef u_int Flags;

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

  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_exception_type;

  // all the system exception classes
#define TAO_SYSTEM_EXCEPTION(name) \
  typedef CORBA_ ## name      name
  TAO_SYSTEM_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

    typedef CORBA_UnknownUserException UnknownUserException;

  // = all the CORBA::is_nil methods.
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Object_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Environment_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (TypeCode_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ORB_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Principal_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ServerRequest_ptr req);

#if (TAO_HAS_MINIMUM_CORBA == 0)

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Request_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NVList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Context_ptr);

#endif /* TAO_HAS_MINIMUM_CORBA */

  // = all the CORBA release methods.
  TAO_NAMESPACE_INLINE_FUNCTION void release (Object_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Environment_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Principal_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (TypeCode_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (ORB_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (ServerRequest_ptr req);

#if (TAO_HAS_MINIMUM_CORBA == 0)

  TAO_NAMESPACE_INLINE_FUNCTION void release (Request_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NVList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Context_ptr);

#endif /* TAO_HAS_MINIMUM_CORBA */

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
    tk_component          = 33,
    tk_home               = 34,

    // This symbol is not defined by CORBA 2.0.  It's used to speed up
    // dispatch based on TCKind values, and lets many important ones
    // just be table lookups.  It must always be the last enum value!!

    TC_KIND_COUNT
  };

  // All the typecode constants
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_null;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_void;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_short;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_long;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_ushort;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_ulong;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_float;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_double;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_boolean;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_char;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_octet;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_any;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_TypeCode;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_Principal;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_Object;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_string;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_longlong;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_ulonglong;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_longdouble;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_wchar;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_wstring;

  // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_ ## name
  TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_UnknownUserException;

  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Bounds;
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_Current;

#if (TAO_HAS_MINIMUM_CORBA == 0)
  TAO_NAMESPACE_STORAGE_CLASS TypeCode_ptr _tc_NamedValue;
#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
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

  typedef char *ORBid;
  typedef CORBA::String_var ORBid_var;
  typedef CORBA::String_out ORBid_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ORBid;



  typedef CORBA::Short PolicyErrorCode;
  typedef CORBA::Short_out PolicyErrorCode_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyErrorCode;

  TAO_NAMESPACE_STORAGE_CLASS const PolicyErrorCode BAD_POLICY;
  TAO_NAMESPACE_STORAGE_CLASS const PolicyErrorCode UNSUPPORTED_POLICY;
  TAO_NAMESPACE_STORAGE_CLASS const PolicyErrorCode BAD_POLICY_TYPE;
  TAO_NAMESPACE_STORAGE_CLASS const PolicyErrorCode BAD_POLICY_VALUE;
  TAO_NAMESPACE_STORAGE_CLASS const PolicyErrorCode UNSUPPORTED_POLICY_VALUE;

  typedef CORBA_PolicyError PolicyError;
  typedef CORBA_PolicyError *PolicyError_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyError;

  typedef CORBA_InvalidPolicies InvalidPolicies;
  typedef CORBA_InvalidPolicies* InvalidPolicies_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InvalidPolicies;

  typedef CORBA::ULong PolicyType;
  typedef CORBA::ULong_out PolicyType_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyType;

  typedef CORBA_Policy Policy;
  typedef CORBA_Policy *Policy_ptr;
  typedef CORBA_Policy_var Policy_var;
  typedef CORBA_Policy_out Policy_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Policy;

  typedef CORBA_PolicyList PolicyList;
  typedef CORBA_PolicyList_var PolicyList_var;
  typedef CORBA_PolicyList_out PolicyList_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyList;

  // = Service definitions
  typedef CORBA::UShort ServiceType;
  typedef ServiceType CORBA_ServiceType;
  typedef CORBA::UShort_out ServiceType_out;
  typedef ServiceType_out CORBA_ServiceType_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ServiceType;
  TAO_NAMESPACE_STORAGE_CLASS const ServiceType Security;

  typedef CORBA::ULong ServiceOption;
  typedef ServiceOption CORBA_ServiceOption;
  typedef CORBA::ULong_out ServiceOption_out;
  typedef ServiceOption_out CORBA_ServiceOption_out;
  TAO_NAMESPACE_STORAGE_CLASS  CORBA::TypeCode_ptr _tc_ServiceOption;

  typedef CORBA::ULong ServiceDetailType;
  typedef ServiceDetailType CORBA_ServiceDetailType;
  typedef CORBA::ULong_out ServiceDetailType_out;
  typedef ServiceDetailType_out CORBA_ServiceDetailType_out;
  TAO_NAMESPACE_STORAGE_CLASS  CORBA::TypeCode_ptr _tc_ServiceDetailType;

  typedef CORBA_ServiceDetail ServiceDetail;
  typedef CORBA_ServiceDetail_var ServiceDetail_var;
  typedef CORBA_ServiceDetail_out ServiceDetail_out;
  typedef CORBA_ServiceDetail *ServiceDetail_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ServiceDetail;

  typedef CORBA_ServiceInformation ServiceInformation;
  typedef CORBA_ServiceInformation_var ServiceInformation_var;
  typedef CORBA_ServiceInformation_out ServiceInformation_out;
  typedef CORBA_ServiceInformation *ServiceInformation_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ServiceInformation;

  typedef CORBA_PolicyTypeSeq PolicyTypeSeq;
  typedef CORBA_PolicyTypeSeq_var PolicyTypeSeq_var;
  typedef CORBA_PolicyTypeSeq_out PolicyTypeSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyTypeSeq;

  enum SetOverrideType
  {
    SET_OVERRIDE,
    ADD_OVERRIDE
  };
  typedef SetOverrideType &SetOverrideType_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_SetOverrideType;

  typedef CORBA_PolicyManager PolicyManager;
  typedef CORBA_PolicyManager_var PolicyManager_var;
  typedef CORBA_PolicyManager_out PolicyManager_out;
  typedef CORBA_PolicyManager *PolicyManager_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyManager;

  typedef CORBA_PolicyCurrent PolicyCurrent;
  typedef CORBA_PolicyCurrent_var PolicyCurrent_var;
  typedef CORBA_PolicyCurrent_out PolicyCurrent_out;
  typedef CORBA_PolicyCurrent *PolicyCurrent_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PolicyCurrent;

  typedef CORBA_DomainManager DomainManager;
  typedef CORBA_DomainManager *DomainManager_ptr;
  typedef CORBA_DomainManager_var DomainManager_var;
  typedef CORBA_DomainManager_out DomainManager_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_DomainManager;

  typedef CORBA_DomainManagerList DomainManagerList;
  typedef CORBA_DomainManagerList *DomainManagerList_ptr;
  typedef CORBA_DomainManagerList_var DomainManagerList_var;
  typedef CORBA_DomainManagerList_out DomainManagerList_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_DomainManagerList;

  TAO_NAMESPACE_STORAGE_CLASS const PolicyType SecConstruction;

#if (TAO_HAS_MINIMUM_CORBA == 0)
  typedef CORBA_ConstructionPolicy ConstructionPolicy;
  typedef CORBA_ConstructionPolicy *ConstructionPolicy_ptr;
  typedef CORBA_ConstructionPolicy_var ConstructionPolicy_var;
  typedef CORBA_ConstructionPolicy_out ConstructionPolicy_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConstructionPolicy;

  typedef CORBA_WrongTransaction WrongTransaction;
  typedef WrongTransaction *WrongTransaction_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_WrongTransaction;

#endif /* ! defined (TAO_HAS_MINIMUM_CORBA) */

  // ****************************************************************

  typedef CORBA_PollableSet PollableSet;
  typedef CORBA_PollableSet_var PollableSet_var;
  typedef CORBA_PollableSet_out PollableSet_out;
  typedef CORBA_PollableSet_ptr PollableSet_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PollableSet;

  typedef CORBA_Pollable Pollable;
  typedef CORBA_Pollable_var Pollable_var;
  typedef CORBA_Pollable_out Pollable_out;
  typedef CORBA_Pollable_ptr Pollable_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Pollable;

  typedef CORBA_DIIPollable DIIPollable;
  typedef CORBA_DIIPollable_var DIIPollable_var;
  typedef CORBA_DIIPollable_out DIIPollable_out;
  typedef CORBA_DIIPollable_ptr DIIPollable_ptr;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_DIIPollable;

    // ****************************************************************
  typedef char *RepositoryId;
  typedef CORBA::String_var RepositoryId_var;
  typedef CORBA::String_out RepositoryId_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_RepositoryId;

  typedef char *Identifier;
  typedef CORBA::String_var Identifier_var;
  typedef CORBA::String_out Identifier_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Identifier;

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
  // ------------------------------Interface REPOS Bala

  // =================== Interface Repository =====================
  typedef char * ScopedName;
  typedef CORBA_String_var ScopedName_var;
  typedef CORBA_String_out ScopedName_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ScopedName;

  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_IRObject;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TypeCodeFactory;

  typedef CORBA::Short Visibility;
  typedef CORBA::Short_out Visibility_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Visibility;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short PRIVATE_MEMBER;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short PUBLIC_MEMBER;

  typedef CORBA::Short ValueModifier;
  typedef CORBA::Short_out ValueModifier_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueModifier;

  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short VM_NONE;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short VM_CUSTOM;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short VM_ABSTRACT;
  TAO_NAMESPACE_STORAGE_CLASS const CORBA::Short VM_TRUNCATABLE;
  // ================== Interface Repository =====================
#endif /* TAO_HAS_INTERFACE_REPOSITORY */
  // There could be a single version of these methods, but g++ 2.7.2
  // gets horribly confused if we used CORBA::default_environment() at
  // this point.
  TAO_NAMESPACE_STORAGE_CLASS ORB_ptr ORB_init (int &argc,
                                                char *argv[],
                                                const char *orb_name = 0);
  TAO_NAMESPACE_STORAGE_CLASS ORB_ptr ORB_init (int &argc,
                                                char *argv[],
                                                const char *orb_name,
                                                CORBA_Environment &ACE_TRY_ENV);
  // ORB initialisation

  // = TAO extensions...

  TAO_NAMESPACE_INLINE_FUNCTION CORBA_Environment& default_environment (void);

  // Obtain the thread-specific default environment.
  // This is obsolete and only left here for backwards compatibility.

}
TAO_NAMESPACE_CLOSE  // end of class (namespace) CORBA

// ****************************************************************

// Several OMG assigned numbers, for a complete list check:
//
// ftp://ftp.omg.org/pub/docs/ptc/99-05-02.txt
//
// For details on how to ask more numbers check:
//
// http://www.omg.org/cgi-bin/doc?ptc/99-02-01
//

// These numbers were assigned by the OMG.  Do *NOT* change.
// The ASCII representation is "TAO\x00", we can request more ORB
// types later.
#define TAO_ORB_TYPE 0x54414f00U

// The standard profile tags, they are listed here only to avoid
// putting the raw literal in the code, it is *NOT* necessary to list
// your own protocols here.
#define TAO_TAG_INVALID_PROFILE -1
#define TAO_TAG_IIOP_PROFILE 0
#define TAO_TAG_MULTIPLE_COMPONENT_PROFILE 1

// We reserved the range 0x54414f00 - 0x54414f0f with the OMG to
// define our own profile ids in TAO.
#define TAO_TAG_UIOP_PROFILE   0x54414f00U /* Local IPC (Unix Domain) */
// @@ The values below are suggestions for some of the protocols
//    we have thought of, subject to change at any point
// #define TAO_TAG_AIOP_PROFILE   0x54414f01U /* ATM/AAL5 */
#define TAO_TAG_SHMEM_PROFILE  0x54414f02U /* Shared memory */
// #define TAO_TAG_MSGQ_PROFILE   0x54414f03U /* Message Queue */
// #define TAO_TAG_UDP_PROFILE    0x54414f04U /* UDP */
// #define TAO_TAG_MCAST_PROFILE  0x54414f05U /* IP/Multicast */
// #define TAO_TAG_CPCI_PROFILE   0x54414f06U /* Compact/PCI */
// #define TAO_TAG_VME_PROFILE    0x54414f07U /* VME Bus */
// #define TAO_TAG_NTNP_PROFILE   0x54414f08U /* NT Named Pipes */
// #define TAO_TAG_HTTPNG_PROFILE 0x54414f09U /* HTTP-NG */
// #define TAO_TAG_PIPE_PROFILE   0x54414f0AU /* Pipe */
// #define TAO_TAG_XXXX_PROFILE   0x54414f0BU /* ???? */
#define TAO_TAG_BIOP_PROFILE   0x54414f0FU /* Boeign Custom Protocol */

// We reserved the range 0x54414f00 - 0x54414f0f with the OMG to
// define our own profile tagged components in TAO.

// This tag is no longer used in TAO.  It will not be reused for a
// period of time to preserve backward compatibility with servers
// running older versions of TAO.
// #define TAO_TAG_PRIORITY 0x54414f00U

//  Tag 0x54414f01U is defined/used by SSLIOP.

// Tag for storing multiple endpoints within a single profile.
#define TAO_TAG_ENDPOINTS 0x54414f02U
//#define TAO_TAG_SOME_OTHER_TAG 0x54414f03U


// We reserved the range 0x54414f00 - 0x54414f0f with the OMG to
// define our own service context list entries.

// We insert a dummy service context in the service context list to
// preserve the alignment in DSI based gateways, so no
// marshaling/demarshaling is required.  This is *extremely* brittle,
// but works.
#define TAO_SVC_CONTEXT_ALIGN 0x54414f00U

// This number is reserver by the OMG as a prefix to all the standard
// minor codes.  Check the CORBA/IIOP spec for details
#define TAO_OMG_VMCID 0x4f4d0000

// The following macros are used as semi-symbolic names for the
// standard minor codes. Check the CORBA/IIOP spec for details, and/or
// read the code in Exception.cpp
// @@ TODO: Add more of the minor codes.  Use them in the right spot.
#define TAO_OMG_MINOR_BAD_PARAM_10 10

// This number was assigned by the OMG.  Do *NOT* change at random.
// The ASCII representation is TA0xxxx, close enough since they only
// take 20 bits, the first 16 are TA, the next 4 are 0000.  Remember
// that we can only play with the last 12 bits, TAO_MAX_MINOR_CODE is
// there to remind us of that.
#define TAO_DEFAULT_MINOR_CODE 0x54410000
#define TAO_MAX_MINOR_CODE 0x54410FFF

// Minor code encoding.  Encode the location in 5 bits, and the errno
// in 7 bits:
// 0x   0101 0100   0100 0001   0000   ____ _     ___ ____
//          T           A        0    location      errno

// Location encoding:  5 bits, after the errno encoding.
#define TAO_INVOCATION_CONNECT_MINOR_CODE          (0x01U << 7)
#define TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE (0x02U << 7)
#define TAO_INVOCATION_SEND_REQUEST_MINOR_CODE     (0x03U << 7)
#define TAO_POA_DISCARDING                         (0x04U << 7)
#define TAO_POA_HOLDING                            (0x05U << 7)
#define TAO_UNHANDLED_SERVER_CXX_EXCEPTION         (0x06U << 7)
#define TAO_INVOCATION_RECV_REQUEST_MINOR_CODE     (0x07U << 7)
#define TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL  (0x08U << 7)
#define TAO_MPROFILE_CREATION_ERROR                (0x09U << 7)
#define TAO_TIMEOUT_CONNECT_MINOR_CODE             (0x0AU << 7)
#define TAO_TIMEOUT_SEND_MINOR_CODE                (0x0BU << 7)
#define TAO_TIMEOUT_RECV_MINOR_CODE                (0x0CU << 7)
#define TAO_IMPLREPO_SERVER_MANUAL_ACTIVATION      (0x0DU << 7)
#define TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE   (0x0EU << 7)
#define TAO_ORB_CORE_INIT_LOCATION_CODE            (0x0FU << 7)
#define TAO_POLICY_NARROW_CODE                     (0x10U << 7)
#define TAO_GUARD_FAILURE                          (0x11U << 7)
#define TAO_POA_BEING_DESTROYED                    (0x12U << 7)
// *Don't* use TAO_<location>_MINOR_CODE greater than 0x1FU!

// errno encoding:  bottom 7 bits.
#define TAO_UNSPECIFIED_MINOR_CODE  0x0U
#define TAO_ETIMEDOUT_MINOR_CODE    0x1U
#define TAO_ENFILE_MINOR_CODE       0x2U
#define TAO_EMFILE_MINOR_CODE       0x3U
#define TAO_EPIPE_MINOR_CODE        0x4U
#define TAO_ECONNREFUSED_MINOR_CODE 0x5U
#define TAO_ENOENT_MINOR_CODE       0x6U
#define TAO_EBADF_MINOR_CODE        0x7U
#define TAO_ENOSYS_MINOR_CODE       0x8U
#define TAO_EPERM_MINOR_CODE        0x9U
#define TAO_EAFNOSUPPORT_MINOR_CODE 0xAU
#define TAO_EAGAIN_MINOR_CODE       0xBU
#define TAO_ENOMEM_MINOR_CODE       0xCU
#define TAO_EACCES_MINOR_CODE       0xDU
#define TAO_EFAULT_MINOR_CODE       0xEU
#define TAO_EBUSY_MINOR_CODE        0xFU
#define TAO_EEXIST_MINOR_CODE       0x10U
#define TAO_EINVAL_MINOR_CODE       0x11U
#define TAO_ECOMM_MINOR_CODE        0x12U
#define TAO_ECONNRESET_MINOR_CODE   0x13U
#define TAO_ENOTSUP_MINOR_CODE      0x14U
// *Don't* use TAO_<errno>_MINOR_CODE greater than 0x7FU!

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

// A helper class to handle the various kinds of octet sequences used
// inside the ORB.

typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;
extern TAO_Export CORBA::TypeCode_ptr TC_opaque;

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_opaque&);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_opaque&);

class TAO_ObjectKey;
class TAO_ObjectKey_var;
class TAO_ObjectKey_out;

#if defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_CORBAFWD_H */
