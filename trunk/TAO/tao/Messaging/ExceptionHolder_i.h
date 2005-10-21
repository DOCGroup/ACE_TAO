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

#if !defined (TAO_HAS_DEPRECATED_EXCEPTION_HOLDER)

#include "tao/Messaging/ExceptionHolderA.h"
#include "tao/Messaging/ExceptionHolderC.h"

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
    ExceptionHolder (
      ::CORBA::Boolean is_system_exception,
      ::CORBA::Boolean byte_order,
      const ::CORBA::OctetSeq &marshaled_exception,
      ::TAO::Exception_Data* data_,
      ::CORBA::ULong exceptions_count
    );

    virtual ~ExceptionHolder ();

    virtual void raise_exception (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    virtual void raise_exception_with_list (
        const ::Dynamic::ExceptionList & exc_list ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );

  private:
    TAO::Exception_Data* data_;
    CORBA::ULong count_;
  };
}

#endif

#include /**/ "ace/post.h"
#endif /* TAO_MESSAGING_EXCEPTIONHOLDER_I_H */

