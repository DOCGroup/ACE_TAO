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

// @@ TODO take the portion that we need of ace/CDR_Stream.h into
// ace/CDR_Types.h
#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/try_macros.h"

#if !defined (TAO_HAS_DLL)
// On Windows NT, the default is to build TAO as a DLL library.
#  define TAO_HAS_DLL 1
#endif /* TAO_HAS_DLL */

// Here are definition for TAO library.
#if defined (TAO_HAS_DLL) && (TAO_HAS_DLL == 1)
#  if defined (TAO_BUILD_DLL)
#    define TAO_Export ACE_Proper_Export_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#  else
#    define TAO_Export ACE_Proper_Import_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#  endif /* TAO_BUILD_DLL */
#else /* TAO_HAS_DLL */
#  define TAO_Export
#  define TAO_SINGLETON_DECLARATION(T)
#endif /* TAO_HAS_DLL */

#if defined (_MSC_VER)
# ifdef   _DEBUG                  // convert from VC++ convention ...
#  define  DEBUG           // ... to normal convention
# endif
#endif /* _MSC_VER */

#define TAO_DEFAULT_MINOR_CODE 0xffff0000L

// Forward declarations of some data types are needed.

class CORBA_Any;
class CORBA_Any_var;
class CORBA_Any_out;
typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_DynAny;
typedef class CORBA_DynAny *CORBA_DynAny_ptr;

class CORBA_DynStruct;
typedef CORBA_DynStruct *CORBA_DynStruct_ptr;

class CORBA_DynSequence;
typedef CORBA_DynSequence *CORBA_DynSequence_ptr;

class CORBA_DynArray;
typedef CORBA_DynArray *CORBA_DynArray_ptr;

class CORBA_DynUnion;
typedef CORBA_DynUnion *CORBA_DynUnion_ptr;

class CORBA_DynEnum;
typedef CORBA_DynEnum *CORBA_DynEnum_ptr;

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
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE);

#define TAO_SYSTEM_EXCEPTION(name) \
  class CORBA_ ## name
TAO_SYSTEM_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#if !defined (TAO_HAS_MINIMUM_CORBA)

class CORBA_Request;
class CORBA_Request_var;
class CORBA_Request_out;
typedef class CORBA_Request *CORBA_Request_ptr;

class CORBA_ORB_RequestSeq;
class CORBA_ORB_RequestSeq_var;
class CORBA_ORB_RequestSeq_out;
typedef class CORBA_ORB_RequestSeq *CORBA_ORB_RequestSeq_ptr;

class CORBA_NamedValue;
class CORBA_NamedValue_var;
class CORBA_NamedValue_out;
typedef class CORBA_NamedValue *CORBA_NamedValue_ptr;

class CORBA_NVList;
class CORBA_NVList_var;
class CORBA_NVList_out;
typedef class CORBA_NVList *CORBA_NVList_ptr;

#endif /* TAO_HAS_MINIMUM_CORBA */

class CORBA_ORB;
class CORBA_ORB_var;
class CORBA_ORB_out;
typedef class CORBA_ORB *CORBA_ORB_ptr;

class CORBA_Object;
class CORBA_Object_var;
class CORBA_Object_out;
typedef class CORBA_Object *CORBA_Object_ptr;

class CORBA_Context;
class CORBA_Context_var;
class CORBA_Context_out;
typedef class CORBA_Context *CORBA_Context_ptr;

class CORBA_ContextList;
class CORBA_ContextList_var;
class CORBA_ContextList_out;
typedef class CORBA_ContextList *CORBA_ContextList_ptr;

class CORBA_Current;
class CORBA_Current_var;
class CORBA_Current_out;
typedef class CORBA_Current *CORBA_Current_ptr;

class CORBA_ServerRequest;
typedef class CORBA_ServerRequest *CORBA_ServerRequest_ptr;

class CORBA_Environment;
class CORBA_Environment_var;
class CORBA_Environment_out;
typedef class CORBA_Environment *CORBA_Environment_ptr;

class CORBA_Principal;
typedef class CORBA_Principal *CORBA_Principal_ptr;

typedef class CORBA_ImplementationDef *CORBA_ImplementationDef_ptr;

typedef class CORBA_InterfaceDef *CORBA_InterfaceDef_ptr;

class CORBA_String_var;
class CORBA_String_out;

class CORBA_ExceptionList;

class CORBA_PolicyError;
typedef CORBA_PolicyError *CORBA_PolicyError_ptr;

class CORBA_InvalidPolicies;
typedef CORBA_InvalidPolicies* CORBA_InvalidPolicies_ptr;

class CORBA_PolicyTypeSeq;
class CORBA_PolicyTypeSeq_var;
class CORBA_PolicyTypeSeq_out;

class CORBA_PolicyManager;
typedef CORBA_PolicyManager *CORBA_PolicyManager_ptr;

class CORBA_PolicyCurrent;
typedef CORBA_PolicyCurrent *CORBA_PolicyCurrent_ptr;

class CORBA_Policy;
class CORBA_Policy_var;
class CORBA_Policy_out;
typedef CORBA_Policy *CORBA_Policy_ptr;

class CORBA_PolicyList;
class CORBA_PolicyList_var;
class CORBA_PolicyList_out;
typedef CORBA_PolicyList *CORBA_PolicyList_ptr;

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

// enum values defined in tao/NVList.h, bitwise ORed.
typedef u_int CORBA_Flags;

typedef void (*TAO_Skeleton)(
    CORBA_ServerRequest &,
    void *,
    void *,
    CORBA_Environment &TAO_IN_ENV
  );

// forward declare sequences.
template <class T> class TAO_Unbounded_Sequence;
template <class T> class TAO_Unbounded_Object_Sequence;

class CORBA_String_var;
class CORBA_String_out;

class TAO_Export CORBA
{
  // = TITLE
  //    Provides the CORBA namespace.
  //
  // = DESCRIPTION
  //    This class allows the use of CORBA::Foo, as well as CORBA_Foo
  //    for all standard types Foo.
public:

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

  typedef Char *String;

  // = String memory management.
  static String string_alloc (ULong len);
  static String string_copy (const Char *);
  static String string_dup (const Char *);
  static void string_free (Char *);

  typedef TAO_Unbounded_Sequence<Octet> OctetSeq;

  typedef CORBA_String_var String_var;
  typedef CORBA_String_out String_out;

  // WChar is an IDL extension, not yet standard.  We use 32 bits
  // because that's what many platforms use for their native wchar_t.

  typedef ACE_CDR::WChar WChar;
  typedef WChar *WString;

  typedef WChar &WChar_out;
  // Out type for WChar.

  // = String memory management routines.
  static WString wstring_alloc (ULong len);
  static WString wstring_copy (const WChar *const);
  static WString wstring_dup (const WChar *const);
  static void wstring_free (WChar *const);

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
  typedef Exception *Exception_ptr;
  typedef CORBA_SystemException SystemException;
  typedef CORBA_UserException  UserException;

#if !defined (TAO_HAS_MINIMUM_CORBA)

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

#endif /* TAO_HAS_MINIMUM_CORBA */

  typedef CORBA_Object Object;
  typedef CORBA_Object *Object_ptr;
  typedef CORBA_Object_var Object_var;
  typedef CORBA_Object_out Object_out;

  typedef CORBA_Context Context;
  typedef CORBA_Context *Context_ptr;
  typedef CORBA_Context_var Context_var;
  typedef CORBA_Context_out Context_out;

  typedef CORBA_ContextList ContextList;
  typedef CORBA_ContextList *ContextList_ptr;
  typedef CORBA_ContextList_var ContextList_var;
  typedef CORBA_ContextList_out ContextList_out;

  typedef CORBA_Principal Principal;
  typedef CORBA_Principal *Principal_ptr;

  typedef CORBA_ORB ORB;
  typedef CORBA_ORB *ORB_ptr;
  typedef CORBA_ORB_var ORB_var;
  typedef CORBA_ORB_out ORB_out;

  typedef CORBA_ServerRequest ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;


  typedef CORBA_TypeCode TypeCode;
  typedef TypeCode *TypeCode_ptr;
  typedef CORBA_TypeCode_var TypeCode_var;
  typedef CORBA_TypeCode_out TypeCode_out;

  typedef CORBA_ExceptionList ExceptionList;
  typedef CORBA_ExceptionList_var ExceptionList_var;
  typedef CORBA_ExceptionList *ExceptionList_ptr;

  typedef CORBA_ImplementationDef *ImplementationDef_ptr;

  typedef class CORBA_InterfaceDef *InterfaceDef_ptr;

  typedef CORBA_Current Current;
  typedef CORBA_Current *Current_ptr;
  typedef CORBA_Current_var Current_var;
  typedef CORBA_Current_out Current_out;

  // enum values defined in nvlist.hh, bitwise ORed.
  typedef u_int Flags;

  enum CompletionStatus
  {
    // = Completion Status for System exceptions

    COMPLETED_YES,     // successful or exceptional completion
    COMPLETED_NO,      // didn't change any state; retry is OK
    COMPLETED_MAYBE    // can't say what happened; retry unsafe
  };

  enum ExceptionType
  {
    // = Exception type.

    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION
  };

  // all the system exception classes
#define TAO_SYSTEM_EXCEPTION(name) \
  typedef CORBA_ ## name      name
TAO_SYSTEM_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

  typedef CORBA_UnknownUserException UnknownUserException;

  // = all the CORBA::is_nil methods.
  static Boolean is_nil (Object_ptr);
  static Boolean is_nil (Environment_ptr);
  //  static Boolean is_nil (Context_ptr);
  static Boolean is_nil (TypeCode_ptr);
  static Boolean is_nil (ORB_ptr);
  static Boolean is_nil (Principal_ptr);
  static Boolean is_nil (ServerRequest_ptr req);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  static Boolean is_nil (Request_ptr);
  static Boolean is_nil (NamedValue_ptr);
  static Boolean is_nil (NVList_ptr);

#endif /* TAO_HAS_MINIMUM_CORBA */

  // = all the CORBA release methods.
  static void release (Object_ptr);
  static void release (Environment_ptr);
  static void release (Principal_ptr);
  static void release (Context_ptr);
  static void release (TypeCode_ptr);
  static void release (ORB_ptr);
  static void release (ServerRequest_ptr req);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  static void release (Request_ptr);
  static void release (NamedValue_ptr);
  static void release (NVList_ptr);

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

    // This symbol is not defined by CORBA 2.0.  It's used to speed up
    // dispatch based on TCKind values, and lets many important ones
    // just be table lookups.  It must always be the last enum value!!

    TC_KIND_COUNT
  };

  // All the typecode constants
  static TypeCode_ptr _tc_null;
  static TypeCode_ptr _tc_void;
  static TypeCode_ptr _tc_short;
  static TypeCode_ptr _tc_long;
  static TypeCode_ptr _tc_ushort;
  static TypeCode_ptr _tc_ulong;
  static TypeCode_ptr _tc_float;
  static TypeCode_ptr _tc_double;
  static TypeCode_ptr _tc_boolean;
  static TypeCode_ptr _tc_char;
  static TypeCode_ptr _tc_octet;
  static TypeCode_ptr _tc_any;
  static TypeCode_ptr _tc_TypeCode;
  static TypeCode_ptr _tc_Principal;
  static TypeCode_ptr _tc_Object;
  static TypeCode_ptr _tc_string;
  static TypeCode_ptr _tc_longlong;
  static TypeCode_ptr _tc_ulonglong;
  static TypeCode_ptr _tc_longdouble;
  static TypeCode_ptr _tc_wchar;
  static TypeCode_ptr _tc_wstring;

  // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
  static TypeCode_ptr          _tc_ ## name
TAO_SYSTEM_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  static TypeCode_ptr _tc_UnknownUserException;

  static CORBA::TypeCode_ptr _tc_Current;

  enum
  {
    // = Define flags for NVList add methods

    ARG_IN = 0x01,
    ARG_OUT = 0x02,
    ARG_INOUT = 0x04,
    IN_COPY_VALUE = 0x08,
    OUT_LIST_MEMORY = 0x10
  };

  typedef char *ORBid;
  typedef CORBA::String_var ORBid_var;
  typedef CORBA::String_out ORBid_out;
  static CORBA::TypeCode_ptr _tc_ORBid;

  typedef char *Identifier;
  typedef CORBA::String_var Identifier_var;
  typedef CORBA::String_out Identifier_out;
  static CORBA::TypeCode_ptr _tc_Identifier;

  typedef char *RepositoryId;
  typedef CORBA::String_var RepositoryId_var;
  typedef CORBA::String_out RepositoryId_out;
  static CORBA::TypeCode_ptr _tc_RepositoryId;

  typedef CORBA::Short PolicyErrorCode;
  typedef CORBA::Short_out PolicyErrorCode_out;
  static CORBA::TypeCode_ptr _tc_PolicyErrorCode;

  static const PolicyErrorCode BAD_POLICY;
  static const PolicyErrorCode UNSUPPORTED_POLICY;
  static const PolicyErrorCode BAD_POLICY_TYPE;
  static const PolicyErrorCode BAD_POLICY_VALUE;
  static const PolicyErrorCode UNSUPPORTED_POLICY_VALUE;

  typedef CORBA_PolicyError PolicyError;
  typedef CORBA_PolicyError *PolicyError_ptr;
  static CORBA::TypeCode_ptr _tc_PolicyError;

  typedef CORBA_InvalidPolicies InvalidPolicies;
  typedef CORBA_InvalidPolicies* InvalidPolicies_ptr;
  static CORBA::TypeCode_ptr _tc_InvalidPolicies;

  typedef CORBA::ULong PolicyType;
  typedef CORBA::ULong_out PolicyType_out;
  static CORBA::TypeCode_ptr _tc_PolicyType;

  typedef CORBA_Policy Policy;
  typedef CORBA_Policy *Policy_ptr;
  typedef CORBA_Policy_var Policy_var;
  typedef CORBA_Policy_out Policy_out;
  static CORBA::TypeCode_ptr _tc_Policy;

  typedef CORBA_PolicyList PolicyList;
  typedef CORBA_PolicyList_var PolicyList_var;
  typedef CORBA_PolicyList_out PolicyList_out;
  static CORBA::TypeCode_ptr _tc_PolicyList;

  typedef CORBA_PolicyTypeSeq PolicyTypeSeq;
  typedef CORBA_PolicyTypeSeq_var PolicyTypeSeq_var;
  typedef CORBA_PolicyTypeSeq_out PolicyTypeSeq_out;
  static CORBA::TypeCode_ptr _tc_PolicyTypeSeq;

  enum SetOverrideType
  {
    SET_OVERRIDE,
    ADD_OVERRIDE
  };
  typedef SetOverrideType &SetOverrideType_out;
  static CORBA::TypeCode_ptr _tc_SetOverrideType;

  typedef CORBA_PolicyManager PolicyManager;
  typedef CORBA_PolicyManager *PolicyManager_ptr;
  static CORBA::TypeCode_ptr _tc_PolicyManager;

  typedef CORBA_PolicyCurrent PolicyCurrent;
  typedef CORBA_PolicyCurrent *PolicyCurrent_ptr;
  static CORBA::TypeCode_ptr _tc_PolicyCurrent;

  // ****************************************************************

  typedef CORBA_PollableSet PollableSet;
  typedef CORBA_PollableSet_var PollableSet_var;
  typedef CORBA_PollableSet_out PollableSet_out;
  typedef CORBA_PollableSet_ptr PollableSet_ptr;
  static CORBA::TypeCode_ptr _tc_PollableSet;

  typedef CORBA_Pollable Pollable;
  typedef CORBA_Pollable_var Pollable_var;
  typedef CORBA_Pollable_out Pollable_out;
  typedef CORBA_Pollable_ptr Pollable_ptr;
  static CORBA::TypeCode_ptr _tc_Pollable;

  typedef CORBA_DIIPollable DIIPollable;
  typedef CORBA_DIIPollable_var DIIPollable_var;
  typedef CORBA_DIIPollable_out DIIPollable_out;
  typedef CORBA_DIIPollable_ptr DIIPollable_ptr;
  static CORBA::TypeCode_ptr _tc_DIIPollable;

  // ****************************************************************

  // There could be a single version of these methods, but g++ 2.7.2
  // gets horribly confused if we used CORBA::default_environment() at
  // this point.
  static ORB_ptr ORB_init (int &argc,
                           char *const *argv,
                           const char *orb_name = 0);
  static ORB_ptr ORB_init (int &argc,
                           char *const *argv,
                           const char *orb_name,
                           CORBA_Environment &TAO_IN_ENV);
  // ORB initialisation

  // = TAO extensions...

  static CORBA_Environment& default_environment (void);
  // Obtain the thread-specific default environment.

  // = The following two methods are TAO-specific extensions.
  static ORB_ptr instance (void);
  // Returns a pointer to the "default ORB."

  static void instance (ORB_ptr);
  // Sets a pointer to the "default ORB."

private:
  friend class CORBA_ORB;
  static ORB_ptr instance_;
  // Points to the "default ORB."
};  // end of class (namespace) CORBA

// ****************************************************************

// A helper clas to handle the various kinds of octet sequences used
// inside the ORB.

typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;
extern TAO_Export CORBA::TypeCode_ptr TC_opaque;

extern TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_opaque&);

extern TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_opaque&);

class TAO_ObjectKey;
class TAO_ObjectKey_var;
class TAO_ObjectKey_out;
typedef TAO_ObjectKey *TAO_ObjectKey_ptr;

#endif /* TAO_CORBAFWD_H */
