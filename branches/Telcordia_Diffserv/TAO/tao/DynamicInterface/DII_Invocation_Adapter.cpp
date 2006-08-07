//$Id$
#include "tao/DynamicInterface/DII_Invocation_Adapter.h"
#include "tao/DynamicInterface/DII_Invocation.h"
#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"

#include "tao/Exception.h"
#include "tao/ORB_Constants.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Transport.h"
#include "tao/Transport.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/DynamicInterface/Request.h"

#include "ace/os_include/os_errno.h"

ACE_RCSID (tao,
           Invocation_Adapter,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  DII_Invocation_Adapter::DII_Invocation_Adapter (CORBA::Object *target,
                                                  Argument **args,
                                                  int arg_number,
                                                  const char *operation,
                                                  size_t op_len,
                                                  CORBA::ExceptionList *excp,
                                                  CORBA::Request *r,
                                                  Invocation_Mode mode)

    : Invocation_Adapter (target,
                          args,
                          arg_number,
                          operation,
                          op_len,
                          0, // Collocation Proxy broker pointer
                          TAO_TWOWAY_INVOCATION,
                          mode)
      , exception_list_ (excp)
      , request_ (r)
  {
  }

  Invocation_Status
  DII_Invocation_Adapter::invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL)
  {
    // Simple sanity check
    if (this->mode_ != TAO_DII_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
                TAO::VMCID,
                EINVAL),
            CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    r.transport ()->messaging_object ()->out_stream ().reset_byte_order (request_->_tao_byte_order ());

    TAO::DII_Invocation synch (this->target_,
                               r,
                               op,
                               this->exception_list_,
                               this->request_);


    Invocation_Status status =
      synch.remote_invocation (max_wait_time
                               ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    if (status == TAO_INVOKE_RESTART &&
        synch.is_forwarded ())
      {
        effective_target =
          synch.steal_forwarded_reference ();

#if TAO_HAS_INTERCEPTORS == 1
        const CORBA::Boolean permanent_forward =
            (synch.reply_status() == TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD_PERM);
#else
        const CORBA::Boolean permanent_forward = false;
#endif
        this->object_forwarded (effective_target,
                                r.stub (),
                                permanent_forward
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }
    return status;
  }

  //================================================================
  DII_Deferred_Invocation_Adapter::DII_Deferred_Invocation_Adapter (
      CORBA::Object *target,
      Argument **args,
      int arg_number,
      const char *operation,
      size_t op_len,
      Collocation_Proxy_Broker *b,
      TAO_ORB_Core *oc,
      CORBA::Request *req,
      Invocation_Mode mode)
    : Invocation_Adapter (target,
                          args,
                          arg_number,
                          operation,
                          op_len,
                          b,
                          TAO_TWOWAY_INVOCATION,
                          mode)
      , request_ (req)
      , rd_ (0)
      , orb_core_ (oc)
  {
  }

  void
  DII_Deferred_Invocation_Adapter::invoke (
      TAO::Exception_Data *ex,
      unsigned long ex_count
      ACE_ENV_ARG_DECL)
  {
    // New reply dispatcher on the heap, because we will go out of
    // scope and hand over the  reply dispatcher to the ORB.
    // So this->rd_ is 0, because we do not need to
    // hold a pointer to it.
    ACE_NEW_THROW_EX (this->rd_,
        TAO_DII_Deferred_Reply_Dispatcher (this->request_,
                                           this->orb_core_),
                      CORBA::NO_MEMORY ());

    Invocation_Adapter::invoke (ex,
                                ex_count
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  Invocation_Status
  DII_Deferred_Invocation_Adapter::invoke_twoway (
      TAO_Operation_Details &op,
      CORBA::Object_var &effective_target,
      Profile_Transport_Resolver &r,
      ACE_Time_Value *&max_wait_time
      ACE_ENV_ARG_DECL)
  {
    // Simple sanity check
    if (this->mode_ != TAO_DII_DEFERRED_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
                TAO::VMCID,
                EINVAL),
            CORBA::COMPLETED_NO),
                   TAO_INVOKE_FAILURE);
      }

    r.transport ()->messaging_object ()->out_stream ().reset_byte_order (request_->_tao_byte_order ());
    TAO::DII_Deferred_Invocation synch (
        this->target_,
        r,
        op,
        this->rd_,
        this->request_);

    r.transport ()->messaging_object ()->out_stream ().reset_byte_order (request_->_tao_byte_order ());

    Invocation_Status status =
      synch.remote_invocation (max_wait_time
                               ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (status == TAO_INVOKE_RESTART)
      {
        effective_target =
          synch.steal_forwarded_reference ();
      }

    return status;
  }
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
