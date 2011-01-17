// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/SystemExceptionA.h"
#include "tao/AnyTypeCode/Any_SystemException.h"
#include "tao/AnyTypeCode/Any_Dual_Impl_T.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Enum_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

#include "tao/CORBA_String.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (THREAD_CANCELLED)
#undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

// List of standard/system exceptions ... used to create static
// storage for their typecodes.

#define STANDARD_EXCEPTION_LIST \
    TAO_SYSTEM_EXCEPTION (UNKNOWN) \
    TAO_SYSTEM_EXCEPTION (BAD_PARAM) \
    TAO_SYSTEM_EXCEPTION (NO_MEMORY) \
    TAO_SYSTEM_EXCEPTION (IMP_LIMIT) \
    TAO_SYSTEM_EXCEPTION (COMM_FAILURE) \
    TAO_SYSTEM_EXCEPTION (INV_OBJREF) \
    TAO_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST) \
    TAO_SYSTEM_EXCEPTION (NO_PERMISSION) \
    TAO_SYSTEM_EXCEPTION (INTERNAL) \
    TAO_SYSTEM_EXCEPTION (MARSHAL) \
    TAO_SYSTEM_EXCEPTION (INITIALIZE) \
    TAO_SYSTEM_EXCEPTION (NO_IMPLEMENT) \
    TAO_SYSTEM_EXCEPTION (BAD_TYPECODE) \
    TAO_SYSTEM_EXCEPTION (BAD_OPERATION) \
    TAO_SYSTEM_EXCEPTION (NO_RESOURCES) \
    TAO_SYSTEM_EXCEPTION (NO_RESPONSE) \
    TAO_SYSTEM_EXCEPTION (PERSIST_STORE) \
    TAO_SYSTEM_EXCEPTION (BAD_INV_ORDER) \
    TAO_SYSTEM_EXCEPTION (TRANSIENT) \
    TAO_SYSTEM_EXCEPTION (FREE_MEM) \
    TAO_SYSTEM_EXCEPTION (INV_IDENT) \
    TAO_SYSTEM_EXCEPTION (INV_FLAG) \
    TAO_SYSTEM_EXCEPTION (INTF_REPOS) \
    TAO_SYSTEM_EXCEPTION (BAD_CONTEXT) \
    TAO_SYSTEM_EXCEPTION (OBJ_ADAPTER) \
    TAO_SYSTEM_EXCEPTION (DATA_CONVERSION) \
    TAO_SYSTEM_EXCEPTION (INV_POLICY) \
    TAO_SYSTEM_EXCEPTION (REBIND) \
    TAO_SYSTEM_EXCEPTION (TIMEOUT) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_UNAVAILABLE) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_MODE) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED) \
    TAO_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK) \
    TAO_SYSTEM_EXCEPTION (INVALID_TRANSACTION) \
    TAO_SYSTEM_EXCEPTION (CODESET_INCOMPATIBLE) \
    TAO_SYSTEM_EXCEPTION (BAD_QOS) \
    TAO_SYSTEM_EXCEPTION (INVALID_ACTIVITY) \
    TAO_SYSTEM_EXCEPTION (ACTIVITY_COMPLETED) \
    TAO_SYSTEM_EXCEPTION (ACTIVITY_REQUIRED) \
    TAO_SYSTEM_EXCEPTION (THREAD_CANCELLED)

namespace TAO
{
  namespace TypeCode
  {
    char const * const enumerators_CORBA_CompletionStatus[] =
      {
        "COMPLETED_YES",
        "COMPLETED_NO",
        "COMPLETED_MAYBE"
      };

    Enum<char const *,
         char const * const *,
         TAO::Null_RefCount_Policy>
      tc_CompletionStatus ("IDL:omg.org/CORBA/CompletionStatus:1.0",
                           "CompletionStatus",
                           enumerators_CORBA_CompletionStatus,
                           3 /* # of enumerators */);
  }
}

namespace CORBA
{
  // An internal TypeCode.
  TypeCode_ptr const _tc_CompletionStatus =
    &TAO::TypeCode::tc_CompletionStatus;
}

namespace TAO
{
  namespace TypeCode
  {
    Struct_Field<char const *,
                 CORBA::TypeCode_ptr const *> const
    fields_CORBA_SystemException[] =
      {
        { "minor",     &CORBA::_tc_ulong },
        { "completed", &CORBA::_tc_CompletionStatus }
      };

    typedef TAO::TypeCode::Struct<char const *,
                                  CORBA::TypeCode_ptr const *,
                                  TAO::TypeCode::Struct_Field<char const *,
                                                              CORBA::TypeCode_ptr const *> const *,
                                  TAO::Null_RefCount_Policy> tc_SystemException;
  }
}

#define TAO_SYSTEM_EXCEPTION(name) \
namespace TAO \
{ \
  namespace TypeCode \
  { \
    tc_SystemException tc_CORBA_ ## name ( \
      CORBA::tk_except, \
      "IDL:omg.org/CORBA/" #name ":1.0", \
      #name, \
      TAO::TypeCode::fields_CORBA_SystemException, \
      2 /* # of fields */); \
  } \
} \
CORBA::TypeCode_ptr const CORBA::_tc_ ## name = \
    &TAO::TypeCode::tc_CORBA_ ## name;

STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION


#define TAO_SYSTEM_EXCEPTION(name) \
void \
CORBA::operator<<= (CORBA::Any &any, const CORBA::name &ex) \
{ \
  TAO::Any_SystemException::insert_copy ( \
      any, \
      CORBA::name ::_tao_any_destructor, \
      CORBA::_tc_ ## name, \
      ex \
    ); \
}

STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
void \
CORBA::operator<<= (CORBA::Any &any, CORBA::name *ex) \
{ \
  TAO::Any_SystemException::insert ( \
      any, \
      CORBA::name ::_tao_any_destructor, \
      CORBA::_tc_ ## name, \
      ex \
    ); \
}

STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA::Boolean \
CORBA::operator>>= (const CORBA::Any &any, \
                    const CORBA::name *&ex) \
{ \
  return \
    TAO::Any_SystemException::extract ( \
        any, \
        CORBA::name ::_tao_any_destructor, \
        CORBA::_tc_ ## name, \
        (const CORBA::SystemException *&) ex, \
        &CORBA::name ::_tao_create); \
}

STANDARD_EXCEPTION_LIST
#undef TAO_SYSTEM_EXCEPTION

TAO_END_VERSIONED_NAMESPACE_DECL
