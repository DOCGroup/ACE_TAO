// -*- C++ -*-
// $Id$

#include "tao/Messaging/ExceptionHolder_i.h"

ACE_RCSID (Messaging,
           ExceptionHolder_i,
           "$Id$")

#include "tao/Messaging/Messaging.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ExceptionHolder::ExceptionHolder (void) :
    data_ (0),
    count_ (0)
  {
  }

  ExceptionHolder::ExceptionHolder (
      ::CORBA::Boolean is_system_exception,
      ::CORBA::Boolean byte_order,
      const ::CORBA::OctetSeq &marshaled_exception,
      ::TAO::Exception_Data* data,
      ::CORBA::ULong exceptions_count) :
        data_ (data),
        count_ (exceptions_count)
  {
    // @todo According to the latest corba spec we should be able to
    // pass this to the ExceptionHolder constructor but the TAO_IDL
    // compiler doesn't seem to generate this.
    this->is_system_exception (is_system_exception);
    this->byte_order (byte_order);
    this->marshaled_exception (marshaled_exception);
  }

  ExceptionHolder::~ExceptionHolder ()
  {
  }

  void ExceptionHolder::raise_exception (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    {
      TAO_Messaging_Helper::exception_holder_raise (
        this->data_,
        this->count_,
        this->marshaled_exception ().get_buffer (),
        this->marshaled_exception ().length (),
        this->byte_order (),
        this->is_system_exception ()
        ACE_ENV_ARG_PARAMETER);
      }

  void ExceptionHolder::raise_exception_with_list (
      const ::Dynamic::ExceptionList & ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    {
      // todo convert exceptionlist to something we can really use.
      this->raise_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

  CORBA::ValueBase *
  ExceptionHolderFactory::create_for_unmarshal (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  {
    TAO::ExceptionHolder* ret_val = 0;
    ACE_NEW_THROW_EX (ret_val,
                      ExceptionHolder,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (0);

    return ret_val;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

