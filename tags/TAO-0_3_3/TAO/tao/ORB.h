// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    ORB.h
//
// = DESCRIPTION
//     Header file for CORBA's <ORB> type and <CORBA> type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc. and Douglas C. Schmidt.
//
// ============================================================================

#ifndef TAO_ORB_H
#  define TAO_ORB_H

#include "tao/corba.h"

// ObjectIds recognized by CORBA_ORB::resolve_initial_references ()...
// of course, no guarantees are made that the call will return
// something useful.
#define TAO_OBJID_NAMESERVICE      "NameService"
#define TAO_OBJID_TRADINGSERVICE   "TradingService"
#define TAO_OBJID_ROOTPOA          "RootPOA"
#define TAO_DEFAULT_ROOTPOA_NAME   ""
#define TAO_OBJID_POACURRENT       "POACurrent"
#define TAO_OBJID_INTERFACEREP     "InterfaceRepository"

typedef enum
{
  TAO_SERVICEID_NAMESERVICE,
  TAO_SERVICEID_TRADINGSERVICE
} TAO_Service_ID;

// @@ NW: Disable messing with the alignment for now.
// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if defined (_MSC_VER)
//# pragma  pack (push, 1)          // VC++, stack 1-byte alignment policy
# ifdef   _DEBUG                  // convert from VC++ convention ...
#   define  DEBUG           // ... to normal convention
# endif
#elif defined (__BORLANDC__)
# pragma option -a                // BC++, use 1 byte alignment
#endif

#if !defined (TAO_CONST)
// Something to document the fact that we want to put 'const' in front
// of a type, but that it won't turn out the way we want, e.g., we
// really want to express that a CORBA_String is const, but since
// CORBA_String is a char*, the const modifies the pointer and not the
// pointed-to, and some compilers (like SGI's EDG-derived thang)
// complain.
#define TAO_CONST
#endif /* TAO_CONST */

// Forward declarations of some data types are needed.

class TAO_POA_Manager;
class TAO_POA_Policies;

class CORBA_Any;
class CORBA_Any_var;
class CORBA_Any_out;
//typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_TypeCode;
class CORBA_TypeCode_var;
class CORBA_TypeCode_out;
//typedef class CORBA_TypeCode *CORBA_TypeCode_ptr;

class CORBA_Exception;
//typedef class CORBA_Exception *CORBA_Exception_ptr;

class CORBA_Request;
class CORBA_Request_var;
class CORBA_Request_out;
//typedef class CORBA_Request *CORBA_Request_ptr;

class CORBA_NamedValue;
class CORBA_NamedValue_var;
class CORBA_NamedValue_out;
//typedef class CORBA_NamedValue *CORBA_NamedValue_ptr;

class CORBA_NVList;
class CORBA_NVList_var;
class CORBA_NVList_out;
//typedef class CORBA_NVList *CORBA_NVList_ptr;

class CORBA_ORB;
class CORBA_ORB_var;
class CORBA_ORB_out;
//typedef class CORBA_ORB *CORBA_ORB_ptr;

class CORBA_Object;
class CORBA_Object_var;
class CORBA_Object_out;
//typedef class CORBA_Object *CORBA_Object_ptr;

class CORBA_ServerRequest;
//typedef class CORBA_ServerRequest *CORBA_ServerRequest_ptr;

class CORBA_Environment;
//typedef class CORBA_Environment *CORBA_Environment_ptr;

class CORBA_Principal;
//typedef class CORBA_Principal *CORBA_Principal_ptr;

class CORBA_SystemException;
class CORBA_UserException;

class CORBA_Policy;
class CORBA_Policy_var;
class CORBA_Policy_out;

class CORBA_PolicyList;
class CORBA_PolicyList_var;
class CORBA_PolicyList_out;

class CORBA_Current;
class CORBA_Current_var;
class CORBA_Current_out;

// forward declarations of system exceptions
#define TAO_SYSTEM_EXCEPTION(name) class CORBA_ ## name

TAO_SYSTEM_EXCEPTION(UNKNOWN);
TAO_SYSTEM_EXCEPTION(BAD_PARAM);
TAO_SYSTEM_EXCEPTION(NO_MEMORY);
TAO_SYSTEM_EXCEPTION(IMP_LIMIT);
TAO_SYSTEM_EXCEPTION(COMM_FAILURE);
TAO_SYSTEM_EXCEPTION(INV_OBJREF);
TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST);
TAO_SYSTEM_EXCEPTION(NO_PERMISSION);
TAO_SYSTEM_EXCEPTION(INTERNAL);
TAO_SYSTEM_EXCEPTION(MARSHAL);
TAO_SYSTEM_EXCEPTION(INITIALIZE);
TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT);
TAO_SYSTEM_EXCEPTION(BAD_TYPECODE);
TAO_SYSTEM_EXCEPTION(BAD_OPERATION);
TAO_SYSTEM_EXCEPTION(NO_RESOURCES);
TAO_SYSTEM_EXCEPTION(NO_RESPONSE);
TAO_SYSTEM_EXCEPTION(PERSIST_STORE);
TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER);
TAO_SYSTEM_EXCEPTION(TRANSIENT);
TAO_SYSTEM_EXCEPTION(FREE_MEM);
TAO_SYSTEM_EXCEPTION(INV_IDENT);
TAO_SYSTEM_EXCEPTION(INV_FLAG);
TAO_SYSTEM_EXCEPTION(INTF_REPOS);
TAO_SYSTEM_EXCEPTION(BAD_CONTEXT);
TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER);
TAO_SYSTEM_EXCEPTION(DATA_CONVERSION);
#undef TAO_SYSTEM_EXCEPTION

class CORBA_UnknownUserException;

typedef class CORBA_ImplementationDef *
        CORBA_ImplementationDef_ptr;

typedef class CORBA_InterfaceDef *
        CORBA_InterfaceDef_ptr;

// =Forward declarations
struct TAO_Dispatch_Context;
class TAO_Operation_Table;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_ORB_Parameters;
class TAO_InputCDR;
class TAO_OutputCDR;

// The new (POA) base class for servants.
class TAO_ServantBase;

// enum values defined in nvlist.h, bitwise ORed.
typedef u_int CORBA_Flags;

// This and the other CORBA IDL types below are
// typedef'd to the corresponding CDR types
// already defined in ACE.
typedef CDR::Boolean CORBA_Boolean;

  typedef struct TAO_Leader_Follower_Info_Struct
    {
      ACE_SYNCH_MUTEX leader_follower_lock_;
      // do protect the access to the following three members

      ACE_Unbounded_Set<ACE_SYNCH_CONDITION *> follower_set_;
      // keep a set of followers around (protected)

      int leaders_;
      // 0 if no leader is around, 1 if there is a leader
      // > 1 if we do nested upcalls (protected)

      ACE_thread_t leader_thread_ID_;
      // thread ID of the leader thread (protected)
    } TAO_Leader_Follower_Info;


// forward declare sequences.
template <class T> class TAO_Unbounded_Sequence;
template <class T> class TAO_Unbounded_Object_Sequence;

class TAO_Export CORBA
{
  // = TITLE
  //    Provides the CORBA namespace.
  //
  // = DESCRIPTION
  //    This class allows the use of CORBA::Foo, as well as CORBA_Foo
  //    for all standard types Foo.
public:

  typedef CORBA_Boolean Boolean;
  typedef Boolean &Boolean_out;
  // Out type for boolean.

// This and the other CORBA IDL types below are
// typedef'd to the corresponding CDR types
// already defined in ACE.
  typedef CDR::Octet Octet;
  typedef Octet  &Octet_out;
  // Out type for octet.

  typedef CDR::Short Short;
  typedef Short &Short_out;
  // Out type for Short.

  typedef CDR::UShort UShort;
  typedef UShort &UShort_out;
  // Out type for UShort.

  // = CORBA "Long" (and its unsigned cousin) are 32 bits.
  typedef CDR::Long Long;
  typedef CDR::ULong ULong;

  // 94-9-32 Appendix A, also the OMG C++ mapping, stipulate that 64
  // bit integers are "LongLong".
  typedef Long &Long_out;
  // Out type for long.

  typedef ULong &ULong_out;
  // Out type for unsigned long.

  // = The following are IDL extensions, not yet standard.

  typedef CDR::LongLong LongLong;
  typedef CDR::ULongLong ULongLong;

  typedef LongLong &LongLong_out;
  // Out type for long long.

  typedef ULongLong &ULongLong_out;
  // Out type for unsigned long long.

  typedef CDR::Float Float;
  typedef Float &Float_out; // out type for float

  typedef CDR::Double Double;
  typedef Double &Double_out;
  // Out type for double.

  // 94-9-32 Appendix A defines a 128 bit floating point "long double"
  // data type, with greatly extended precision and four more bits of
  // exponent (compared to "double").  This is an IDL extension, not
  // yet standard.

  typedef CDR::LongDouble LongDouble;
  typedef LongDouble &LongDouble_out;
  // Out type for long doubles.

  typedef CDR::Char Char;
  typedef Char &Char_out;
  // Out type for char.

  typedef TAO_Unbounded_Sequence<Octet> OctetSeq;

  typedef Char *String;

  // = String memory management.
  static String string_alloc (ULong len);
  static String string_copy (const Char *);
  static String string_dup (const Char *);
  static void string_free (Char *);

  class TAO_Export String_var
  {
    // = TITLE
    //   String var class. Provides automatic deallocation of storage
    //   for the string once it goes out of scope.
  public:
    String_var (void);
    // default constructor.

    String_var (char *p);
    // constructor, owns p.

    String_var (const char *p);
    // constructor. Makes a copy of p.

    String_var (const String_var &s);
    // copy constructor.

    ~String_var (void);
    // destructor.

    String_var &operator= (char *p);
    // assignment operator.

    String_var &operator= (const char *p);
    // assignment to a const char*.  Makes a copy.

    String_var &operator= (const String_var &s);
    // assignment operator.

    operator char *();
    // access and modify.

    operator const char *() const;
    // only read privileges.

    char &operator[] (ULong index);
    // allows access and modification using an index.

    char operator[] (ULong index) const;
    // allows only accessing thru an index.

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

    const char *in (void) const;
    // for in parameter.

    char *&inout (void);
    // for inout parameter.

    char *&out (void);
    // for out parameter.

    char *_retn (void);
    // for string of return type.

  private:
    char *ptr_;
    // instance.
  };

  class TAO_Export String_out
  {
    // = TITLE
    //   String_out
    //
    // = DESCRIPTION
    //   To support the memory management for "out" parameter passing
    //   mode.  ORBOS/97-05-15, Appendix C.2 defines a String_out class
  public:
    // = operations.

    String_out (char *&p);
    // Construction from a reference to a string.

    String_out (String_var &p);
    // Construction from a var.

    String_out (const String_out &s);
    // Copy constructor.

    String_out &operator= (const String_out &s);
    // Assignment from a string_out.

    String_out &operator= (char *p);
    // Assignment from a string.

    operator char *&();
    // Cast.

    char *&ptr (void);
    // Return underlying instance.

  private:
    char *&ptr_;
    // Instance.

    // assignment from _var disallowed
    void operator= (const String_var &);
  };

  // WChar is an IDL extension, not yet standard.  We use 32 bits
  // because that's what many platforms use for their native wchar_t.

  typedef CDR::WChar WChar;
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
  typedef Environment *Environment_ptr;

  typedef CORBA_Exception Exception;
  typedef Exception *Exception_ptr;

  typedef CORBA_NamedValue NamedValue;
  typedef NamedValue *NamedValue_ptr;
  typedef CORBA_NamedValue_var NamedValue_var;
  typedef CORBA_NamedValue_out NamedValue_out;

  typedef CORBA_NVList NVList;
  typedef NVList *NVList_ptr;
  typedef CORBA_NVList_var NVList_var;
  typedef CORBA_NVList_out NVList_out;

  typedef CORBA_Object Object;
  typedef CORBA_Object *Object_ptr;
  typedef CORBA_Object_var Object_var;
  typedef CORBA_Object_out Object_out;

  typedef CORBA_Principal Principal;
  typedef CORBA_Principal *Principal_ptr;

  typedef CORBA_ORB ORB;
  typedef CORBA_ORB *ORB_ptr;
  typedef CORBA_ORB_var ORB_var;
  typedef CORBA_ORB_out ORB_out;

  typedef CORBA_Request Request;
  typedef Request *Request_ptr;
  typedef CORBA_Request_var Request_var;
  typedef CORBA_Request_out Request_out;

  typedef CORBA_ServerRequest ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;

  typedef CORBA_SystemException SystemException;

  typedef CORBA_TypeCode TypeCode;
  typedef TypeCode *TypeCode_ptr;
  typedef CORBA_TypeCode_var TypeCode_var;
  typedef CORBA_TypeCode_out TypeCode_out;

  typedef CORBA_UserException  UserException;

  typedef CORBA_ExceptionList ExceptionList;
  typedef CORBA_ExceptionList *ExceptionList_ptr;

  typedef CORBA_ImplementationDef *ImplementationDef_ptr;

  typedef class CORBA_InterfaceDef *InterfaceDef_ptr;


  typedef CORBA_Policy Policy;
  typedef CORBA_Policy *Policy_ptr;
  typedef CORBA_Policy_var Policy_var;
  typedef CORBA_Policy_out Policy_out;

  typedef CORBA_PolicyList PolicyList;
  typedef CORBA_PolicyList_var PolicyList_var;
  typedef CORBA_PolicyList_out PolicyList_out;

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
  TAO_SYSTEM_EXCEPTION(UNKNOWN);
  TAO_SYSTEM_EXCEPTION(BAD_PARAM);
  TAO_SYSTEM_EXCEPTION(NO_MEMORY);
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT);
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE);
  TAO_SYSTEM_EXCEPTION(INV_OBJREF);
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST);
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION);
  TAO_SYSTEM_EXCEPTION(INTERNAL);
  TAO_SYSTEM_EXCEPTION(MARSHAL);
  TAO_SYSTEM_EXCEPTION(INITIALIZE);
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT);
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE);
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION);
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES);
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE);
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE);
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER);
  TAO_SYSTEM_EXCEPTION(TRANSIENT);
  TAO_SYSTEM_EXCEPTION(FREE_MEM);
  TAO_SYSTEM_EXCEPTION(INV_IDENT);
  TAO_SYSTEM_EXCEPTION(INV_FLAG);
  TAO_SYSTEM_EXCEPTION(INTF_REPOS);
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT);
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER);
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION);
#undef TAO_SYSTEM_EXCEPTION

  typedef CORBA_UnknownUserException UnknownUserException;

  // = all the CORBA::is_nil methods.
  static Boolean is_nil (Object_ptr);
  static Boolean is_nil (Environment_ptr);
  static Boolean is_nil (NamedValue_ptr);
  static Boolean is_nil (NVList_ptr);
  static Boolean is_nil (Request_ptr);
  //  static Boolean is_nil (Context_ptr);
  static Boolean is_nil (TypeCode_ptr);
  static Boolean is_nil (ORB_ptr);
  static Boolean is_nil (Principal_ptr);
  static Boolean is_nil (ServerRequest_ptr req);

  // = all the CORBA release methods.
  static void release (Object_ptr);
  static void release (Environment_ptr);
  static void release (NamedValue_ptr);
  static void release (NVList_ptr);
  static void release (Request_ptr);
  static void release (Principal_ptr);
  //  static Boolean release (Context_ptr);
  static void release (TypeCode_ptr);
  static void release (ORB_ptr);
  static void release (ServerRequest_ptr req);

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
    tk_alias              = 21,           // 94-11-7
    tk_except             = 22,           // 94-11-7

    // = These five are OMG-IDL data type extensions.
    tk_longlong           = 23,           // 94-9-32 Appendix A (+ 2)
    tk_ulonglong          = 24,           // 94-9-32 Appendix A (+ 2)
    tk_longdouble         = 25,           // 94-9-32 Appendix A (+ 2)
    tk_wchar              = 26,           // 94-9-32 Appendix A (+ 2)
    tk_wstring            = 27,           // 94-9-32 Appendix A (+ 2)

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
  // static TypeCode_ptr _tc_alias;
  // static TypeCode_ptr _tc_except;
  static TypeCode_ptr _tc_longlong;
  static TypeCode_ptr _tc_ulonglong;
  static TypeCode_ptr _tc_longdouble;
  static TypeCode_ptr _tc_wchar;
  static TypeCode_ptr _tc_wstring;

  // = Typecode constants for system exceptions.
#define TAO_SYSTEM_EXCEPTION(name) \
  static TypeCode_ptr          _tc_ ## name

  TAO_SYSTEM_EXCEPTION(UNKNOWN);
  TAO_SYSTEM_EXCEPTION(BAD_PARAM);
  TAO_SYSTEM_EXCEPTION(NO_MEMORY);
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT);
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE);
  TAO_SYSTEM_EXCEPTION(INV_OBJREF);
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST);
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION);
  TAO_SYSTEM_EXCEPTION(INTERNAL);
  TAO_SYSTEM_EXCEPTION(MARSHAL);
  TAO_SYSTEM_EXCEPTION(INITIALIZE);
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT);
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE);
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION);
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES);
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE);
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE);
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER);
  TAO_SYSTEM_EXCEPTION(TRANSIENT);
  TAO_SYSTEM_EXCEPTION(FREE_MEM);
  TAO_SYSTEM_EXCEPTION(INV_IDENT);
  TAO_SYSTEM_EXCEPTION(INV_FLAG);
  TAO_SYSTEM_EXCEPTION(INTF_REPOS);
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT);
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER);
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION);
#undef  TAO_SYSTEM_EXCEPTION

  static TypeCode_ptr _tc_UnknownUserException;

  static ORB_ptr ORB_init (int &argc,
                           char *const *argv,
                           const char *orb_name = 0,
                           Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // ORB initialisation, per OMG document 94-9-46.

  // = The following two methods are TAO-specific extensions.
  static ORB_ptr instance (void);
  // Returns a pointer to the "default ORB."

  static void instance (ORB_ptr);
  // Sets a pointer to the "default ORB."

  enum
  {
    // = Define flags for NVList add methods

    ARG_IN = 0x01,
    ARG_OUT = 0x02,
    ARG_INOUT = 0x04,
    IN_COPY_VALUE = 0x08,
    OUT_LIST_MEMORY = 0x10
  };

  static CORBA::TypeCode_ptr _tc_Policy;
  static CORBA::TypeCode_ptr _tc_PolicyList;

  static CORBA::TypeCode_ptr _tc_Current;

  typedef char *Identifier;
  typedef CORBA::String_var Identifier_var;
  typedef CORBA::String_out Identifier_out;
  static CORBA::TypeCode_ptr _tc_Identifier;

  typedef char *RepositoryId;
  typedef CORBA::String_var RepositoryId_var;
  typedef CORBA::String_out RepositoryId_out;
  static CORBA::TypeCode_ptr _tc_RepositoryId;

  typedef CORBA::ULong PolicyType;
  typedef CORBA::ULong_out PolicyType_out;
  static CORBA::TypeCode_ptr _tc_PolicyType;

private:
  friend class CORBA_ORB;
  static ORB_ptr instance_;
  // Points to the "default ORB."
};  // end of class (namespace) CORBA

#include "tao/Exception.h"
#include "tao/Managed_Types.h"
#include "tao/Sequence.h"
#include "tao/Sequence_T.h"
#include "tao/Object_KeyC.h"
#include "tao/Union.h"

class STUB_Object;
// Forward declarations.

class TAO_Export CORBA_ORB
{
  // = TITLE
  //   ORB pseudo-objref.
  //
  // = DESCRIPTION
  //
  //   The "ORB" pseudo-object is used in bootstrapping, such as to
  //   create object references from strings.  This class is intended
  //   to be inherited by others, which will provide some more of the
  //   CORBA support.  Implementations of this "CORBA::ORB" class must
  //   know how to create stringify/destringify their objrefs, as well
  //   as how to marshal and unmarshal them.
  //
public:

  // ORB_Core has special privileges
  friend class TAO_ORB_Core;

  class TAO_Export InvalidName : public CORBA_UserException
  {
  public:
    InvalidName (void);

    virtual void _raise (void);
    InvalidName* _narrow (CORBA_Exception *ex);
    virtual int _is_a (const char* interface_id) const;
  };

  static CORBA::ORB_ptr _duplicate (CORBA::ORB_ptr orb);
  // Return a duplicate of <{orb}>.  When work with this duplicate is
  // complete, it should be freed up using <CORBA::release()>.

  static CORBA::ORB_ptr _nil (void);
  // Returns a pointer to a nil ORB, i.e., an non-existent ORB.  This
  // can be used for initialization or in comparisons.

  virtual CORBA::Object_ptr string_to_object (const char *str,
                                              CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ()) = 0;
  // Turn a string-ified object reference back into an object pointer.
  // Each type of ORB, e.g. an IIOP ORB, must implement this.
  // Typically these strings are created using <object_to_string()>.
  virtual CORBA::String object_to_string (CORBA::Object_ptr obj,
                                          CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ()) = 0;
  // Turn an object reference into a string.  Each type of ORB,
  // e.g. an IIOP ORB, must implement this.  This can be used by
  // servers to publish their whereabouts to clients.  The output of
  // this is typically eventually given to <string_to_object()> as an
  // argument.

  virtual TAO_ServantBase *_get_collocated_servant (STUB_Object *p) = 0;
  // Return the object pointer of an collocated object it there is
  // one, otherwise, return 0.  Each type of ORB, e. g., IIOP ORB,
  // must implement this and determine what is a collocated object
  // based on information provided in the STUB_Object.

  void create_list (CORBA::Long count,
                    CORBA::NVList_ptr &retval);

  // = New Things from the POA Specification
  //
  // Of all of these operations, only <run> is currently implemented.
  // Others require clarification of design or more implementation
  // than is currently available.

  CORBA::Boolean work_pending (void);
  // Returns an indication of whether the ORB needs the <{main thread}> to
  // perform some work.

  int perform_work (const ACE_Time_Value & = ACE_Time_Value::zero);
  // If called by the <{main thread}>, this operation performs an
  // implementation-defined unit of work. Otherwise, it does nothing.
  // Note that the default behavior is not to block; this behavior can
  // be modified by passing an appropriate <ACE_Time_Value>.
  //
  // It is platform-specific how the application and ORB arrange to
  // use compatible threading primitives.

  int run (void);
  int run (ACE_Time_Value &tv);
  int run (ACE_Time_Value *tv);
  // Instructs the ORB to initialize itself and run its event loop in
  // the current thread, not returning until the ORB has shut down.
  // If an error occurs during initialization or a run-time this
  // method will return -1.  If <tv> is non-NULL, then if no requests
  // arrive at this thread before the timeout elapses we return to the
  // caller with a value of 0 (this allows timeouts).  Otherwise, if
  // we've returned since we've been asked to shut down the value of 1
  // is returned.

  void shutdown (CORBA::Boolean wait_for_completion = 0);
  // This operation instructs the ORB to shut down. Shutting down the
  // ORB causes all Object Adapters to be shut down. If
  // <wait_for_completion> parameter is TRUE, this operation blocks
  // until all ORB processing (including request processing and object
  // deactivation or other operations associated with object adapters)
  // has completed.  <[NOTE]> <wait_for_completion>=TRUE is not
  // currently supported.

  CORBA_Object_ptr resolve_initial_references (CORBA::String name,
                                               CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());

  CORBA_Object_ptr resolve_initial_references (CORBA::String name,
                                               ACE_Time_Value *timeout,
                                               CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // This method acts as a mini-bootstrapping Naming Service, which is
  // provided by the ORB for certain well-known object references.
  // TAO supports the "NameService", "TradingService", "RootPOA", and
  // "POACurrent" via this method.  The <timeout> value bounds the
  // amount of time the ORB blocks waiting to resolve the service.
  // This is most useful for bootstrapping remote services, such as
  // the "NameService" or "TradingService", that are commonly resolved
  // via multicast.  By default, the value is 0, which means "use the
  // <TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT> timeout period".  Note
  // that by using a default value for the <timeout> parameter, TAO
  // will remains compliant with the CORBA
  // <resolve_initial_references> specification.

  // = TAO-specific extensions to the CORBA specification.

  CORBA_Object_ptr resolve_root_poa (const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                                     TAO_POA_Manager *poa_manager = 0,
                                     const TAO_POA_Policies *policies = 0);
  // Resolve the POA.

  STUB_Object *create_stub_object (const TAO_ObjectKey &key,
                                   const char *type_id,
                                   CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Makes sure that the ORB is open and then creates an IIOP object
  // based on the endpoint.

  CORBA_Object_ptr key_to_object (const TAO_ObjectKey &key,
                                  const char *type_id,
                                  CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Convert key into an object reference.  Return Object_ptr as out
  // parameter.  Errors will come through the environment.
  //
  // Object IDs are assigned and used by servers to identify objects.
  //
  // Type IDs are repository IDs, assigned as part of OMG-IDL
  // interface definition to identify specific interfaces and their
  // relationships to other OMG-IDL interfaces.  It's OK to provide a
  // null type ID.  Providing a null object key will result in an
  // INV_OBJREF exception.
  //
  // Clients which invoke operations using one of these references
  // when the server is not active (or after the last reference to the
  // POA is released) will normally see an OBJECT_NOT_EXIST exception
  // reported by the ORB.  If the POA is a "Named POA" the client's
  // ORB will not normally return OBJECT_NOT_EXIST unless the POA
  // reports that fault.

  int open (void);
  // Set up the ORB Core's acceptor to listen on the
  // previously-specified port for requests.  Returns -1 on failure,
  // else 0.

  static void init_orb_globals (CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Initialize the ORB globals correctly, i.e., only when they
  // haven't been initialized yet.

  static CORBA::Boolean orb_free_resources (void);
  // Indicates if we have reached a point where all ORB owned
  // resources will be deallocated.

  // Reference counting...
  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  TAO_Leader_Follower_Info &leader_follower_info (void);
  // Get access to the leader_follower_info

  void should_shutdown (int value);
  // Set the shutdown flag to <value>.

  int should_shutdown (void);
  // Get the shutdown flag value

  // Forward declaration and typedefs for the exception thrown by
  // the ORB Dynamic Any factory functions.
  class CORBA_ORB_InconsistentTypeCode;
  typedef CORBA_ORB_InconsistentTypeCode InconsistentTypeCode;
  typedef CORBA_ORB_InconsistentTypeCode *InconsistentTypeCode_ptr;

  // Typecode for the above exception.
  static CORBA::TypeCode_ptr _tc_InconsistentTypeCode;

  // Dynamic Any factory functions.

  CORBA_DynAny_ptr       create_dyn_any       (const CORBA_Any& any,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynAny_ptr       create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynStruct_ptr    create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynSequence_ptr  create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynArray_ptr     create_dyn_array     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynUnion_ptr     create_dyn_union     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynEnum_ptr      create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

protected:
  // We must be created via the <ORB_init> call.
  CORBA_ORB (void);
  virtual ~CORBA_ORB (void);

  CORBA_Object_ptr resolve_poa_current (void);
  // Resolve the POA current.

  int run (ACE_Time_Value *tv,
           int break_on_timeouts);
  // Implements the run routine

private:

  CORBA_Object_ptr resolve_commandline_ref (const char *& init_ref);
  // Resolve the Initial reference according to the commandline
  // option -ORBInitRef <ObjectID>:<IOR>

  CORBA_Object_ptr resolve_name_service (ACE_Time_Value *timeout);
  // Resolve the name service object reference.

  CORBA_Object_ptr resolve_trading_service (ACE_Time_Value *timeout);
  // Resolve the trading object reference.

  char *multicast_query (TAO_Service_ID service_id,
                         u_short port,
                         ACE_Time_Value *timeout);
  // returns and IOR string, the client is responsible for freeing
  // memory!

  CORBA_Object_ptr multicast_to_service (TAO_Service_ID service_id,
                                         u_short port,
                                         ACE_Time_Value *timeout);
  // Resolve the refernce of a service of type <name>.

  ACE_SYNCH_MUTEX lock_;
  // lock required for mutual exclusion between multiple threads.

  u_int refcount_;
  // Maintains a reference count of number of instantiations of the
  // ORB.

  u_int open_called_;
  // Flag which denotes that the open method was called.

  ACE_SYNCH_MUTEX open_lock_;
  // Mutual exclusion for calling open.

  ACE_Lock *shutdown_lock_;
  // Pointer to our shutdown lock.

  int should_shutdown_;
  // Flag which denotes that the ORB should shut down and <run> should
  // return.

  CORBA_Object_ptr name_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Naming Context.

  CORBA_Object_ptr schedule_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured RtecScheduler::Scheduler.

  CORBA_Object_ptr event_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Event Channel.

  CORBA_Object_ptr trading_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Trading.

  static int orb_init_count_;
  // Count of the number of times that <ORB_init> has been called.
  // This must be protected by <ACE_Static_Object_Lock>.

  ACE_SYNCH_CONDITION* cond_become_leader_;
  // wait to become the leader if the leader-follower model is active

  TAO_Leader_Follower_Info  leader_follower_info_;
  // Information about the leader follower model

  // = NON-PROVIDED METHODS
  CORBA_ORB (const CORBA_ORB &);
  CORBA_ORB &operator= (const CORBA_ORB &);
};

class TAO_Export CORBA_ORB_var
{
public:
  CORBA_ORB_var (void); // default constructor
  CORBA_ORB_var (CORBA::ORB_ptr);
  CORBA_ORB_var (const CORBA_ORB_var &); // copy constructor
  ~CORBA_ORB_var (void); // destructor

  CORBA_ORB_var &operator= (CORBA::ORB_ptr);
  CORBA_ORB_var &operator= (const CORBA_ORB_var &);
  CORBA::ORB_ptr operator-> (void) const;

  operator const CORBA::ORB_ptr &() const;
  operator CORBA::ORB_ptr &();
  // in, inout, out, _retn
  CORBA::ORB_ptr in (void) const;
  CORBA::ORB_ptr &inout (void);
  CORBA::ORB_ptr &out (void);
  CORBA::ORB_ptr _retn (void);
  CORBA::ORB_ptr ptr (void) const;

private:
  CORBA::ORB_ptr ptr_;
};

class TAO_Export CORBA_ORB_out
{
public:
  CORBA_ORB_out (CORBA::ORB_ptr &);
  CORBA_ORB_out (CORBA_ORB_var &);
  CORBA_ORB_out (CORBA_ORB_out &);
  CORBA_ORB_out &operator= (CORBA_ORB_out &);
  CORBA_ORB_out &operator= (const CORBA_ORB_var &);
  CORBA_ORB_out &operator= (CORBA::ORB_ptr);
  operator CORBA::ORB_ptr &();
  CORBA::ORB_ptr &ptr (void);
  CORBA::ORB_ptr operator-> (void);

private:
  CORBA::ORB_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "tao/ORB.i"
// We are forced to include this file here to satisfy some dependencies
#include "tao/Managed_Types.i"
#endif /* __ACE_INLINE__ */

#if  defined (_MSC_VER)
// @@ NW: Disable messing with alignment for now.
// VC++, goes back to other padding rules
//# pragma pack (pop)
#endif /* _MSV_VER */

#endif /* TAO_ORB_H */
