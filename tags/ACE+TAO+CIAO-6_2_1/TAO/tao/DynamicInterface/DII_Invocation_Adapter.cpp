//$Id$
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicInterface/DII_Invocation_Adapter.h"
#include "tao/DynamicInterface/DII_Invocation.h"
#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"
#include "tao/DynamicInterface/DII_Arguments_Converter_Impl.h"
#include "tao/DynamicInterface/Request.h"

#include "tao/Exception.h"
#include "tao/Exception_Data.h"
#include "tao/ORB_Constants.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Transport.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/SystemException.h"
#include "tao/operation_details.h"

#include "ace/os_include/os_errno.h"

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
                          TAO_CO_THRU_POA_STRATEGY, // default through POA collocation is possible
                          TAO_TWOWAY_INVOCATION,
                          mode)
      , exception_list_ (excp)
      , request_ (r)
      , ex_data_ (0)
  {
  }

  DII_Invocation_Adapter::~DII_Invocation_Adapter (void)
  {
    delete[] ex_data_;
  }

  void
  DII_Invocation_Adapter::invoke (TAO::Exception_Data * /*ex_data*/,
                                  unsigned long ex_count)
  {
    // Convert DII exception list to a form the invocation can use
    // to filter raised user exceptions.
    ex_count = this->exception_list_->count ();
    ACE_NEW_THROW_EX (this->ex_data_,
                      TAO::Exception_Data[ex_count],
                      CORBA::NO_MEMORY ());
    for (unsigned long l=0; l<ex_count ;++l)
    {
      CORBA::TypeCode_var xtc = this->exception_list_->item (l);
      this->ex_data_[l].id = xtc->id ();
      this->ex_data_[l].alloc = 0;
#if TAO_HAS_INTERCEPTORS == 1
      this->ex_data_[l].tc_ptr = xtc.in ();
#endif
    }

    Invocation_Adapter::invoke (this->ex_data_, ex_count);
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
        ACE_Time_Value *&max_wait_time,
        Invocation_Retry_State *retry_state)
  {
    ACE_UNUSED_ARG (retry_state);

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

    TAO_Transport* const transport = r.transport ();

    if (!transport)
      {
        // Way back, we failed to find a profile we could connect to.
        // We've come this far only so we reach the interception points
        // in case they can fix things. Time to bail....
        throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
      }

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, transport->output_cdr_lock (),
                      TAO_INVOKE_FAILURE);

    transport->messaging_object ()->out_stream ().reset_byte_order (
        request_->_tao_byte_order ());

    TAO::DII_Invocation synch (this->target_,
                               r,
                               op,
                               this->exception_list_,
                               this->request_);

    ace_mon.release();

    Invocation_Status status = synch.remote_invocation (max_wait_time);

    if (status == TAO_INVOKE_RESTART &&
        (synch.reply_status () == GIOP::LOCATION_FORWARD ||
         synch.reply_status () == GIOP::LOCATION_FORWARD_PERM))
      {
        CORBA::Boolean const permanent_forward =
          (synch.reply_status () == GIOP::LOCATION_FORWARD_PERM);

        effective_target = synch.steal_forwarded_reference ();

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
      int collocation_opportunity,
      TAO_ORB_Core *oc,
      CORBA::Request *req,
      Invocation_Mode mode)
    : Invocation_Adapter (target,
                          args,
                          arg_number,
                          operation,
                          op_len,
                          collocation_opportunity,
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
      ACE_Time_Value *&max_wait_time,
      Invocation_Retry_State *retry_state)
  {
    ACE_UNUSED_ARG (retry_state);

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

    TAO_Transport* const transport = r.transport ();

    if (!transport)
      {
        // Way back, we failed to find a profile we could connect to.
        // We've come this far only so we reach the interception points
        // in case they can fix things. Time to bail....
        throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
      }

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, transport->output_cdr_lock (),
                      TAO_INVOKE_FAILURE);

    transport->messaging_object ()->out_stream ().reset_byte_order (
      request_->_tao_byte_order ());

    TAO::DII_Deferred_Invocation synch (
        this->target_,
        r,
        op,
        this->rd_,
        this->request_);

    ace_mon.release ();

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
        ACE_Time_Value *&,
        Invocation_Retry_State *)
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
                          TAO_CO_THRU_POA_STRATEGY,
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
