// -*- C++ -*-

//=============================================================================
/**
 *  @file    AnyTypeCode_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ANYTYPECODE_ADAPTER
#define TAO_ANYTYPECODE_ADAPTER

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/CDR_Stream.h"

#include "tao/Basic_Types.h"

#include <string>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  class Any;

  class Policy;
  typedef Policy *Policy_ptr;

  class BooleanSeq;
  class OctetSeq;
  class CharSeq;
  class WCharSeq;
  class ShortSeq;
  class UShortSeq;
  class LongSeq;
  class ULongSeq;
  class LongLongSeq;
  class ULongLongSeq;
  class FloatSeq;
  class DoubleSeq;
  class LongDoubleSeq;
  class StringSeq;
  class WStringSeq;
}

/**
 * @class TAO_AnyTypeCode_Adapter
 */
class TAO_Export TAO_AnyTypeCode_Adapter
  : public ACE_Service_Object
{
public:
  virtual ~TAO_AnyTypeCode_Adapter (void);

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
  virtual CORBA::TypeCode_ptr _tao_type_ ## name (void) const = 0;

ANYTYPECODE__EXCEPTION_LIST
#undef  TAO_SYSTEM_EXCEPTION

  virtual void insert_into_any (CORBA::Any * any, CORBA::Char const * mychar) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::WChar const * mywchar) = 0;

  virtual void insert_into_any (CORBA::Any * any, const std::string & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Policy_ptr policy) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Policy_ptr * policy) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::ULong value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Long value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Short value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::UShort value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Float value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::Double value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::LongLong value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::ULongLong value) = 0;

  virtual void insert_into_any (CORBA::Any * any, CORBA::LongDouble value) = 0;

  virtual void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_wchar value) = 0;

  virtual void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_char value) = 0;

  virtual void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_octet value) = 0;

  virtual void insert_into_any (CORBA::Any * any, ACE_OutputCDR::from_boolean value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::BooleanSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::OctetSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::CharSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::WCharSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::ShortSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::UShortSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::LongSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::LongLongSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::ULongLongSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::FloatSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::DoubleSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::LongDoubleSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::StringSeq & value) = 0;

  virtual void insert_into_any (CORBA::Any * any, const CORBA::WStringSeq & value) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ANYTYPECODE_ADAPTER */
