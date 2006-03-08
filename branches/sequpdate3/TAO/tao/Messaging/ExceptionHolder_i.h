// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExceptionHolder_i.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_MESSAGING_EXCEPTIONHOLDER_I_H
#define TAO_MESSAGING_EXCEPTIONHOLDER_I_H
#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "tao/Messaging/ExceptionHolderA.h"
#include "tao/Messaging/ExceptionHolderC.h"
#include "tao/Valuetype/ValueFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace Dynamic
{
  class ExceptionList;
}

namespace Messaging
{
  class ReplyHandler;
}

namespace TAO
{
  struct Exception_Data;

  class TAO_Messaging_Export ExceptionHolder
    : public virtual ::OBV_Messaging::ExceptionHolder,
      public virtual ::CORBA::DefaultValueRefCountBase
  {
  public:
    ExceptionHolder (void);

    ExceptionHolder (
      ::CORBA::Boolean is_system_exception,
      ::CORBA::Boolean byte_order,
      const ::CORBA::OctetSeq &marshaled_exception,
      ::TAO::Exception_Data* data_,
      ::CORBA::ULong exceptions_count
    );

    virtual void raise_exception (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    virtual void raise_exception_with_list (
        const ::Dynamic::ExceptionList & exc_list ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

  protected:

    virtual ~ExceptionHolder (void);

  private:

    TAO::Exception_Data* const data_;
    CORBA::ULong const count_;

  };

  /**
   * @class ExceptionHolderFactory
   *
   * @brief OBV factory implementation.
   *
   * Factory for ExceptionHolder
   */
  class TAO_Messaging_Export ExceptionHolderFactory :
    public virtual CORBA::ValueFactoryBase
  {
  public:
    virtual CORBA::ValueBase * create_for_unmarshal (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_MESSAGING_EXCEPTIONHOLDER_I_H */

