// -*- C++ -*-

//=============================================================================
/**
 *  @file    Asynch_Invocation_Adapter.h
 *
 *  $Id$
 *
 *
 *  @author Originally Balachandran Natarajan <bala@dre.vanderbilt.edu>
 *  @author Modified   Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_MESSAGING_ASYNCH_INVOCATION_ADAPTER_H
#define TAO_MESSAGING_ASYNCH_INVOCATION_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Messaging/Messaging.h"
#include "tao/Asynch_Reply_Dispatcher_Base.h"
#include "tao/Invocation_Adapter.h"
#include "ace/CORBA_macros.h"
#include "ace/Global_Macros.h"
#include "ace/Auto_Functor.h"

class TAO_Operation_Details;
class TAO_Stub;
class ACE_Time_Value;
class TAO_Asynch_Reply_Dispatcher;
class TAO_Asynch_Reply_Dispatcher_Base;

namespace  CORBA
{
  class Object;
  class Environment;
  class SystemException;
}

namespace TAO
{
  class Argument;

  class Collocation_Proxy_Broker;

  /**
   * @class Asynch_Invocation_Adapter
   *
   * @brief Generic interface for the invocation object visible to the
   * IDL compiler.
   *
   */
  class TAO_Messaging_Export Asynch_Invocation_Adapter : public Invocation_Adapter
  {
  public:
    Asynch_Invocation_Adapter (
        CORBA::Object *target,
        Argument **args,
        int arg_number,
        const char *operation,
        int op_len,
        Collocation_Proxy_Broker *b,
        TAO::Invocation_Mode mode = TAO_ASYNCHRONOUS_CALLBACK_INVOCATION);

    void invoke (Messaging::ReplyHandler_ptr reply_handler_ptr,
                 const TAO_Reply_Handler_Skeleton &reply_handler_skel
                 ACE_ENV_ARG_DECL);

    virtual void invoke (TAO::Exception_Data *ex,
                         unsigned long ex_count
                         ACE_ENV_ARG_DECL);
  protected:

    /**
    virtual void invoke_collocated (TAO_Stub *,
                                    TAO_Operation_Details &op
                                    ACE_ENV_ARG_DECL);
    **/

//@@ TAO_HAS_COLLOCATION -- start
#if !defined (TAO_HAS_COLLOCATION)

    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);
#endif
//@@ TAO_HAS_COLLOCATION -- end

  private:
    /// Autofunctor to manage the reply dispatcher
    ACE_Utils::Auto_Functor<TAO_Asynch_Reply_Dispatcher_Base,
                            ARDB_Refcount_Functor> safe_rd_;

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (Asynch_Invocation_Adapter (void))

    ACE_UNIMPLEMENTED_FUNC (Asynch_Invocation_Adapter & operator= (const Asynch_Invocation_Adapter &))

  };
} // End namespace TAO


#if defined (__ACE_INLINE__)
#include "Asynch_Invocation_Adapter.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_MESSAGING_ASYNCH_INVOCATION_ADAPTER_H*/
