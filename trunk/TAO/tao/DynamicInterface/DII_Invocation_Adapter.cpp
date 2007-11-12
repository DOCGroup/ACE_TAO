//$Id$
#include "tao/DynamicInterface/DII_Invocation_Adapter.h"
#include "tao/DynamicInterface/DII_Invocation.h"
#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"
#include "tao/DynamicInterface/DII_Arguments_Converter_Impl.h"
#include "tao/DynamicInterface/Request.h"

#include "tao/Exception.h"
#include "tao/ORB_Constants.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Transport.h"
#include "tao/SystemException.h"
#include "tao/operation_details.h"

#include "ace/os_include/os_errno.h"

ACE_RCSID (DynamicInterface,
           DII_Invocation_Adapter,
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

  DII_Invocation_Adapter::~DII_Invocation_Adapter (void)
  {
  }

  Invocation_Status
  DII_Invocation_Adapter::invoke_collocated_i (
    TAO_Stub *stub,
    TAO_Operation_Details &details,
    CORBA::Object_var &effective_target,
    Collocation_Strategy strat)
  {
    TAO_DII_Arguments_Converter_Impl* dii_arguments_converter
      = ACE_Dynamic_Service<TAO_DII_Arguments_Converter_Impl>::instance (
        "DII_Arguments_Converter");
    details.cac (dii_arguments_converter);

    return Invocation_Adapter::invoke_collocated_i (stub,
                                                    details,
                                                    effective_target,
                                                    strat);
  }

  Invocation_Status
  DII_Invocation_Adapter::invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time)
  {
    // Simple sanity check
    if (this->mode_ != TAO_DII_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        throw ::CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    TAO::DII_Invocation synch (this->target_,
                               r,
                               op,
                               this->exception_list_,
                               this->request_);

    Invocation_Status status = synch.remote_invocation (max_wait_time);

    if (status == TAO_INVOKE_RESTART && synch.is_forwarded ())
      {
        effective_target = synch.steal_forwarded_reference ();

#if TAO_HAS_INTERCEPTORS == 1
        CORBA::Boolean const permanent_forward =
            (synch.reply_status() == GIOP::LOCATION_FORWARD_PERM);
#else
        CORBA::Boolean const permanent_forward = false;
#endif
        this->object_forwarded (effective_target,
                                r.stub (),
                                permanent_forward);
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
      unsigned long ex_count)
  {
    // New reply dispatcher on the heap, because we will go out of
    // scope and hand over the reply dispatcher to the ORB.
    // So this->rd_ is 0, because we do not need to
    // hold a pointer to it.
    ACE_NEW_THROW_EX (this->rd_,
        TAO_DII_Deferred_Reply_Dispatcher (this->request_,
                                           this->orb_core_),
                      CORBA::NO_MEMORY ());

    Invocation_Adapter::invoke (ex, ex_count);
  }

  Invocation_Status
  DII_Deferred_Invocation_Adapter::invoke_collocated_i (
    TAO_Stub *stub,
    TAO_Operation_Details &details,
    CORBA::Object_var &effective_target,
    Collocation_Strategy strat)
  {
    TAO_DII_Arguments_Converter_Impl* dii_arguments_converter
      = ACE_Dynamic_Service<TAO_DII_Arguments_Converter_Impl>::instance (
        "DII_Arguments_Converter");
    details.cac (dii_arguments_converter);

    return Invocation_Adapter::invoke_collocated_i (stub,
                                                    details,
                                                    effective_target,
                                                    strat);
  }

  Invocation_Status
  DII_Deferred_Invocation_Adapter::invoke_twoway (
      TAO_Operation_Details &op,
      CORBA::Object_var &effective_target,
      Profile_Transport_Resolver &r,
      ACE_Time_Value *&max_wait_time)
  {
    // Simple sanity check
    if (this->mode_ != TAO_DII_DEFERRED_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        throw ::CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    TAO::DII_Deferred_Invocation synch (
        this->target_,
        r,
        op,
        this->rd_,
        this->request_);

    Invocation_Status status = synch.remote_invocation (max_wait_time);

    if (status == TAO_INVOKE_RESTART)
      {
        effective_target = synch.steal_forwarded_reference ();
      }

    return status;
  }

  DII_Asynch_Invocation_Adapter::DII_Asynch_Invocation_Adapter (
      CORBA::Object *target,
      Argument **args,
      int arg_count,
      const char *operation,
      int op_len,
      CORBA::Request *req,
      TAO::Invocation_Mode mode)
    : DII_Invocation_Adapter (target,
                              args,
                              arg_count,
                              operation,
                              op_len,
                              0,
                              req,
                              mode)
  {
  }

  Invocation_Status
  DII_Asynch_Invocation_Adapter::invoke_twoway (
        TAO_Operation_Details &,
        CORBA::Object_var &,
        Profile_Transport_Resolver &,
        ACE_Time_Value *&)
  {
    return TAO_INVOKE_FAILURE;
  }

  DII_Oneway_Invocation_Adapter::DII_Oneway_Invocation_Adapter (
      CORBA::Object *target,
      Argument **args,
      int arg_count,
      const char *operation,
      int op_len,
      TAO::Invocation_Mode mode)
    : Invocation_Adapter (target,
                          args,
                          arg_count,
                          operation,
                          op_len,
                          0,
                          TAO_ONEWAY_INVOCATION,
                          mode)
  {
  }

  Invocation_Status
  DII_Oneway_Invocation_Adapter::invoke_collocated_i (
    TAO_Stub *stub,
    TAO_Operation_Details &details,
    CORBA::Object_var &effective_target,
    Collocation_Strategy strat)
  {
    TAO_DII_Arguments_Converter_Impl* dii_arguments_converter
      = ACE_Dynamic_Service<TAO_DII_Arguments_Converter_Impl>::instance (
        "DII_Arguments_Converter");
    details.cac (dii_arguments_converter);

    return Invocation_Adapter::invoke_collocated_i (stub,
                                                    details,
                                                    effective_target,
                                                    strat);
  }


} // End namespace TAO
TAO_END_VERSIONED_NAMESPACE_DECL
