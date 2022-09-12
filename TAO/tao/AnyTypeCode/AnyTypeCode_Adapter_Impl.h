// -*- C++ -*-

//=============================================================================
/**
 *  @file    AnyTypeCode_Adapter_Impl.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ANYTYPECODE_ADAPTER_IMPL_H
#define TAO_ANYTYPECODE_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode_Adapter.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_AnyTypeCode_Adapter
 */
class TAO_AnyTypeCode_Export TAO_AnyTypeCode_Adapter_Impl
  : public TAO_AnyTypeCode_Adapter
{
public:
    // Used to force the initialization of the code.
    static int Initializer ();

#define ANYTYPECODE__EXCEPTION_LIST \
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
  CORBA::TypeCode_ptr _tao_type_ ## name () const override;

ANYTYPECODE__EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  void insert_into_any (CORBA::Any * any, CORBA::Char const * mychar) override;

  void insert_into_any (CORBA::Any * any, CORBA::WChar const * mywchar) override;

  void insert_into_any (CORBA::Any * any, const std::string & value) override;

#if !defined(ACE_LACKS_STD_WSTRING)
  void insert_into_any (CORBA::Any * any, const std::wstring & value) override;
#endif /* ACE_LACKS_STD_WSTRING */

  void insert_into_any (CORBA::Any * any, CORBA::Long value) override;

  void insert_into_any (CORBA::Any * any, CORBA::Short value) override;

  void insert_into_any (CORBA::Any * any, CORBA::UShort value) override;

  void insert_into_any (CORBA::Any * any, CORBA::Float value) override;

  void insert_into_any (CORBA::Any * any, CORBA::Double value) override;

  void insert_into_any (CORBA::Any * any, CORBA::LongLong value) override;

  void insert_into_any (CORBA::Any * any, CORBA::ULongLong value) override;

  void insert_into_any (CORBA::Any * any, CORBA::LongDouble value) override;

  void insert_into_any (CORBA::Any * any, CORBA::Policy_ptr policy) override;

  void insert_into_any (CORBA::Any * any, CORBA::Policy_ptr * policy) override;

  void insert_into_any (CORBA::Any * any, CORBA::ULong value) override;

  void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_wchar value) override;

  void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_char value) override;

  void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_octet value) override;

  void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_boolean value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::BooleanSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::OctetSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::CharSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::WCharSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::ShortSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::UShortSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::LongSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::ULongSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::LongLongSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::ULongLongSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::FloatSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::DoubleSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::LongDoubleSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::StringSeq & value) override;

  void insert_into_any (CORBA::Any * any, const CORBA::WStringSeq & value) override;
};


ACE_STATIC_SVC_DECLARE (TAO_AnyTypeCode_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_AnyTypeCode, TAO_AnyTypeCode_Adapter_Impl)

static int TAO_Requires_TAO_AnyTypeCode_Adapter_Impl =
  TAO_AnyTypeCode_Adapter_Impl::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ANYTYPECODE_ADAPTER_IMPL_H */
