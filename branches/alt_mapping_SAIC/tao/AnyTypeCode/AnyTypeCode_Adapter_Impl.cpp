// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/SystemExceptionA.h"
#include "tao/AnyTypeCode/WrongTransactionA.h"
#include "tao/AnyTypeCode/PolicyA.h"
#include "tao/AnyTypeCode/Any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    TAO_SYSTEM_EXCEPTION (THREAD_CANCELLED) \
    TAO_SYSTEM_EXCEPTION (WrongTransaction) \
    TAO_SYSTEM_EXCEPTION (PolicyError) \
    TAO_SYSTEM_EXCEPTION (InvalidPolicies)

#define TAO_SYSTEM_EXCEPTION(name) \
CORBA::TypeCode_ptr \
TAO_AnyTypeCode_Adapter_Impl::_tao_type_ ## name (void) const \
{ \
  return CORBA::_tc_ ## name; \
}

STANDARD_EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Char const * mychar)
{
  (*any) <<= mychar;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::WChar const * mywchar)
{
  (*any) <<= mywchar;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, const std::string & value)
{
  (*any) <<= value.c_str ();
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Long value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Short value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::UShort value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Float value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Double value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::LongLong value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::ULongLong value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::LongDouble value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Policy_ptr policy)
{
  (*any) <<= policy;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::Policy_ptr * policy)
{
  (*any) <<= policy;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, CORBA::ULong value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_wchar value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_char value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_octet value)
{
  (*any) <<= value;
}

void
TAO_AnyTypeCode_Adapter_Impl::insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_boolean value)
{
  (*any) <<= value;
}

int
TAO_AnyTypeCode_Adapter_Impl::Initializer (void)
{
  return ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_AnyTypeCode_Adapter_Impl);
}

ACE_STATIC_SVC_DEFINE (
  TAO_AnyTypeCode_Adapter_Impl,
  ACE_TEXT ("AnyTypeCode_Adapter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (TAO_AnyTypeCode_Adapter_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_DEFINE (TAO_AnyTypeCode, TAO_AnyTypeCode_Adapter_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

