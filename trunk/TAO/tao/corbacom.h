// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    corbacom.h
//
// = DESCRIPTION
//    CORBA C/C++/COM mapping for Win32
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc. and Andy Gokhale
//     <gokhale@cs.wustl.edu>.
//
// ============================================================================

#if !defined (TAO_CORBACOM_H)
#define TAO_CORBACOM_H

// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if     defined (_MSC_VER)
#       pragma  pack (push, 1)          // VC++, stack 1-byte alignment policy

#       ifdef   _DEBUG                  // convert from VC++ convention ...
#               define  DEBUG           // ... to normal convention
#       endif

#elif   defined (__BORLANDC__)
#       pragma option -a                // BC++, use 1 byte alignment

#endif

#if !defined (TAO_CONST)
//Something to document the fact that we want to put 'const' in front
// of a type, but that it won't turn out the way we want, e.g., we
// really want to express that a CORBA_String is const, but since CORBA_String
// is a char*, the const modifies the pointer and not the pointed-to, and
// some compilers (like SGI's EDG-derived thang) complain.
#define TAO_CONST
#endif /* TAO_CONST */

// Forward declarations of some data types are needed.

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
//typedef class CORBA_Request *CORBA_Request_ptr;

class CORBA_NamedValue;
//typedef class CORBA_NamedValue *CORBA_NamedValue_ptr;

class CORBA_NVList;
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

typedef class CORBA_ImplementationDef *
        CORBA_ImplementationDef_ptr;

typedef class CORBA_InterfaceDef *
        CORBA_InterfaceDef_ptr;

// =Forward declarations
struct TAO_Dispatch_Context;
class TAO_Object_Table_Impl;
class TAO_Operation_Table;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_ORB_Parameters;
struct CDR;

// The new (POA) base class for servants.
class TAO_ServantBase;

// enum values defined in nvlist.h, bitwise ORed.
typedef u_int CORBA_Flags;

#  if defined (ACE_HAS_BOOL)
  typedef bool CORBA_Boolean;
#  else /* "bool" not builtin to this compiler */
  typedef int CORBA_Boolean;
#  endif /* "bool" not builtin */

// forward declare sequences.
template <class T> class TAO_Unbounded_Sequence;

template <class T>
struct CORBA_SEQUENCE
  // = TITLE
  //    Utility template class.
  //
  // = DESCRIPTION
{
#if ACE_SIZEOF_LONG == 4
  u_long maximum;
  u_long length;
#else
  // = Just assume "int" is four bytes long ...
  u_int maximum;
  u_int length;
#endif /* ACE_SIZEOF_LONG */
  T *buffer;
  CORBA_Boolean release; // Only here to make it compliant with IDL-generated layout

  CORBA_SEQUENCE (void)
    : maximum (0),
      length (0),
      buffer (0),
#  if defined (ACE_HAS_BOOL)
      release (false) { }
#  else /* "bool" not builtin to this compiler */
      // The following hard-codes the definition of CORBA::B_FALSE here, but
      // this class will be disappearing soon . . .
      release (0 /* CORBA::B_FALSE */) { }
#  endif /* "bool" not builtin */

  // XXX destructor should free buffer, elements!!
  ~CORBA_SEQUENCE (void) { }
};

class TAO_Export CORBA
{
  // = TITLE
  //    Provides the CORBA namespace.
  //
  // = DESCRIPTION
  //    This class allows the use of CORBA::Foo, as well as CORBA_Foo
  //    for all standard types Foo.
public:
  // typedef void Status; // g++ doesn't like this
  // return status of operations in a number of standard CORBA classes.

#  if defined (ACE_HAS_BOOL)
#    define B_FALSE false
#    define B_TRUE true
#  else /* "bool" not builtin to this compiler */
  enum { B_FALSE = 0, B_TRUE = 1 };
#  endif /* "bool" not builtin */

  typedef CORBA_Boolean Boolean;
  typedef Boolean &Boolean_out; // out type for boolean

  typedef u_char Octet;
  typedef Octet  &Octet_out;  // out type for octet

  typedef short Short;
  typedef Short &Short_out;   // out type for short

  typedef u_short UShort;
  typedef UShort &UShort_out; // out type for unsigned short

  // CORBA "Long" (and its unsigned cousin) are 32 bits, just like on
  // almost all C/C++ compilers.

  typedef ACE_INT32 Long;
  typedef ACE_UINT32 ULong;

  // 94-9-32 Appendix A, also the OMG C++ mapping, stipulate that 64
  // bit integers are "LongLong".
  typedef Long &Long_out; // out type for long
  typedef ULong &ULong_out; // out type for unsigned long

  //
  // NOTE:  those are IDL extensions, not yet standard.

  typedef ACE_UINT64 ULongLong;
# if defined (_MSC_VER) && _MSC_VER >= 900
    typedef __int64 LongLong;
# elif ACE_SIZEOF_LONG == 8
    typedef long LongLong;
# elif ACE_SIZEOF_LONG_LONG == 8 && !defined (ACE_LACKS_LONGLONG_T)
#   if defined (sun)
      // sun #defines u_longlong_t, maybe other platforms do also.
      // Use it, at least with g++, so that its -pedantic doesn't
      // complain about no ANSI C++ long long.
      typedef longlong_t LongLong;
#   else
      // LynxOS 2.5.0 and Linux don't have u_longlong_t.
      typedef long long LongLong;
#   endif /* sun */
# else  /* no native 64 bit integer type */

    // If "long long" isn't native, programs can't use these data
    // types in normal arithmetic expressions.  If any particular
    // application can cope with the loss of range, it can define
    // conversion operators itself.
#   define NONNATIVE_LONGLONG
#   if defined (ACE_BIG_ENDIAN)
      struct LongLong { Long h, l; };
#   else
      struct LongLong { Long l, h; };
#   endif /* ! ACE_BIG_ENDIAN */
# endif /* no native 64 bit integer type */

  typedef LongLong &LongLong_out;  // out type for long long
  typedef ULongLong &ULongLong_out; // out type for unsigned long long

  typedef float Float;
  typedef Float &Float_out; // out type for float

  typedef double Double;
  typedef Double &Double_out; // out type for double

  // 94-9-32 Appendix A defines a 128 bit floating point "long double"
  // data type, with greatly extended precision and four more bits of
  // exponent (compared to "double").
  //
  // NOTE:  that is an IDL extension, not yet standard.

#  if   ACE_SIZEOF_LONG_DOUBLE == 16
  typedef long double LongDouble;
#  else
#    define     NONNATIVE_LONGDOUBLE
  struct LongDouble
  {
    char ld[16];
  };
#  endif /* ACE_SIZEOF_LONG_DOUBLE != 16 */

  typedef LongDouble &LongDouble_out; // out type for long doubles

  typedef char Char;
  typedef Char &Char_out; // out type for char

  typedef CORBA_SEQUENCE<Octet> OctetSeq;

  typedef Char *String;

  static String string_alloc (ULong len);
  static String string_copy (const Char *);
  static String string_dup (const Char *);
  static void string_free (Char *);

  class TAO_Export String_var
    // = TITLE
    //   String var class. Provides automatic deallocation of storage
    //   for the string once it goes out of scope.
  {
  public:
    String_var (void);
    // default constructor

    String_var (char *p);
    // constructor, owns p

    String_var (const char *p);
    // constructor. Makes a copy of p

    String_var (const String_var &s);
    // copy constructor

    ~String_var (void);
    // destructor

    String_var &operator= (char *p);
    // assignment operator

    String_var &operator= (const char *p);
    // assignment to a const char*. Makes a copy.

    String_var &operator= (const String_var &s);
    // assignment operator

    operator char *();
    // access and modify

    operator const char *() const;
    // only read privileges

    char &operator[] (ULong index);
    // allows access and modification using an index

    char operator[] (ULong index) const;
    // allows only accessing thru an index

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

    const char *in (void) const;
    // for in parameter

    char *&inout (void);
    // for inout parameter

    char *&out (void);
    // for out parameter

    char *_retn (void);
    // for string of return type

  private:
    char *ptr_;
    // instance.
  };

  // ORBOS/97-05-15, Appendix C.2 defines a String_out class

  class String_out
  {
    //=TITLE
    // String_out
    // =DESCRIPTION
    // To support the memory management for "out" parameter passing mode.

  public:
    // =operations

    String_out (char *&p);
    // construction from a reference to a string

    String_out (String_var &p);
    // construction from a var

    String_out (String_out &s);
    // copy constructor

    String_out &operator= (String_out &s);
    // assignment from a string_out

    String_out &operator= (char *p);
    // assignment from a string

    String_out &operator= (const char *p);
    // assignment from a const string

    operator char *&();
    // cast

    char *&ptr (void);
    // return underlying instance

  private:
    char *&ptr_;
    // instance

    // assignment from _var disallowed
    void operator= (const String_var &);
  };

  // 94-9-32 Appendix A defines 16-bit UNICODE characters as
  // "WChar", and null-terminated strings of them as "WString".
  //
  // NOTE:  those are IDL extensions, not yet standard.

#if defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
  typedef short WChar;
#else  /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */
  typedef wchar_t WChar;
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */
  typedef WChar *WString;

  typedef WChar &WChar_out; // out type for WChar

  static WString wstring_alloc (ULong len);
  static WString wstring_copy (const WChar *const);
  static WString wstring_dup (const WChar *const);
  static void wstring_free (WChar *const);

  // various CORBA defined classes

  typedef CORBA_Any         Any;
  typedef CORBA_Any_var     Any_var;
  typedef CORBA_Any_out     Any_out;
  typedef Any               *Any_ptr;

  typedef CORBA_Environment Environment;
  typedef Environment *Environment_ptr;

  typedef CORBA_Exception Exception;
  typedef Exception *Exception_ptr;

  typedef CORBA_NamedValue NamedValue;
  typedef NamedValue *NamedValue_ptr;

  typedef CORBA_NVList NVList;
  typedef NVList *NVList_ptr;

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

  typedef CORBA_ServerRequest ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;

  typedef CORBA_SystemException
  SystemException;

  typedef CORBA_TypeCode TypeCode;
  typedef TypeCode *TypeCode_ptr;
  typedef CORBA_TypeCode_var TypeCode_var;
  typedef CORBA_TypeCode_out TypeCode_out;

  typedef CORBA_UserException  UserException;

  typedef CORBA_SEQUENCE<TypeCode_ptr> ExceptionList;
  typedef ExceptionList *ExceptionList_ptr;

  typedef CORBA_ImplementationDef *ImplementationDef_ptr;

  typedef class CORBA_InterfaceDef *InterfaceDef_ptr;

  // enum values defined in nvlist.hh, bitwise ORed.
  typedef u_int Flags;

  // Completion Status for System exceptions
  enum CompletionStatus
  {
    COMPLETED_YES,     // successful or exceptional completion
    COMPLETED_NO,      // didn't change any state; retry is OK
    COMPLETED_MAYBE    // can't say what happened; retry unsafe
  };

  // exception type
  enum ExceptionType
  {
    NO_EXCEPTION,
    SYSTEM_EXCEPTION,
    USER_EXCEPTION
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

  // = all the CORBA::is_nil methods
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

  // = all the CORBA release methods
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

  // = Kinds of typecodes.

  // Do not change these enum values, or duplicate them if you need to
  // add values.  They are used to index tables, and if you change the
  // values you'll need to find and update all of those tables.  The
  // values are also part of the Common Data Representation, and hence
  // are part of IIOP and other ORB protocols.

  enum TCKind
  {
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

    // these five are OMG-IDL data type extensions
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
  static TypeCode_ptr          _tc_null;
  static TypeCode_ptr          _tc_void;
  static TypeCode_ptr          _tc_short;
  static TypeCode_ptr          _tc_long;
  static TypeCode_ptr          _tc_ushort;
  static TypeCode_ptr          _tc_ulong;
  static TypeCode_ptr          _tc_float;
  static TypeCode_ptr          _tc_double;
  static TypeCode_ptr          _tc_boolean;
  static TypeCode_ptr          _tc_char;
  static TypeCode_ptr          _tc_octet;
  static TypeCode_ptr          _tc_any;
  static TypeCode_ptr          _tc_TypeCode;
  static TypeCode_ptr          _tc_Principal;
  static TypeCode_ptr          _tc_Object;
  static TypeCode_ptr          _tc_struct;
  static TypeCode_ptr          _tc_union;
  static TypeCode_ptr          _tc_enum;
  static TypeCode_ptr          _tc_string;
  static TypeCode_ptr          _tc_sequence;
  static TypeCode_ptr          _tc_array;
  static TypeCode_ptr          _tc_alias;
  static TypeCode_ptr          _tc_except;
  static TypeCode_ptr          _tc_longlong;
  static TypeCode_ptr          _tc_ulonglong;
  static TypeCode_ptr          _tc_longdouble;
  static TypeCode_ptr          _tc_wchar;
  static TypeCode_ptr          _tc_wstring;

  // typecode constants for system exceptions
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

  static TypeCode_ptr _tc_Bounds;
  static TypeCode_ptr _tc_BadKind;

  static ORB_ptr ORB_init (int &argc,
                           char *const *argv,
                           const char *orb_name,
                           Environment &env);
  // ORB initialisation, per OMG document 94-9-46.

  // Define flags for NVList add methods
  enum
  {
    ARG_IN = 0x01,
    ARG_OUT = 0x02,
    ARG_INOUT = 0x04,
    IN_COPY_VALUE = 0x08,
    OUT_LIST_MEMORY = 0x10
  };
};  // end of class (namespace) CORBA

typedef void (*TAO_Skeleton)(CORBA::ServerRequest &,
                             void *, // object_ptr
                             void *, // context_ptr
                             CORBA::Environment &);
// signature of a skeleton for every operation of an interface

// COM interface IIDs
extern "C" TAO_Export const TAO_IID IID_CORBA_Any;
extern "C" TAO_Export const TAO_IID IID_BOA;
extern "C" TAO_Export const TAO_IID IID_CORBA_Exception;
extern "C" TAO_Export const TAO_IID IID_CORBA_UserException;
extern "C" TAO_Export const TAO_IID IID_CORBA_SystemException;
extern "C" TAO_Export const TAO_IID IID_CORBA_Request;
extern "C" TAO_Export const TAO_IID IID_CORBA_Object;
extern "C" TAO_Export const TAO_IID IID_CORBA_ORB;
extern "C" TAO_Export const TAO_IID IID_CORBA_NVList;
extern "C" TAO_Export const TAO_IID IID_CORBA_Principal;
extern "C" TAO_Export const TAO_IID IID_CORBA_TypeCode;
extern "C" TAO_Export const TAO_IID IID_CORBA_ServerRequest;
extern "C" TAO_Export const TAO_IID IID_IIOP_ServerRequest;
extern "C" TAO_Export const TAO_IID IID_STUB_Object;
extern "C" TAO_Export const TAO_IID IID_IIOP_Object;
extern "C" TAO_Export const TAO_IID IID_IIOP_ORB;

// NOTE: stub APIs are nonportable, and must be explicitly #included
// by code emitted from an IDL compiler.

#if     defined (_MSC_VER)
#       pragma pack (pop)               // VC++, goes back to other padding rules
#endif /* VC++ */

// BC++ we leave with 1-byte padding rules...

#endif /* TAO_CORBA_COM_H */
