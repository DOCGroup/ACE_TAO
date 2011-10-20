//$Id$

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include "tao/RTScheduling/Request_Interceptor.h"
#include "tao/RTScheduling/Current.h"
#include "tao/RTScheduling/Distributable_Thread.h"

#include "tao/TSS_Resources.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const IOP::ServiceId
Client_Interceptor::SchedulingInfo = 30;

void
Client_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Client_Interceptor::send_request\n"));

  // Temporary current.
  TAO_RTScheduler_Current_i *new_current = 0;
  TAO_RTScheduler_Current_i *current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);

  if (current != 0)
    {
      // If this is a one way request
      if (!ri->response_expected ())
        {
          // Generate GUID.
          RTScheduling::Current::IdType guid;
          guid.length (sizeof(long));

          size_t temp = ++TAO_RTScheduler_Current::guid_counter;
          ACE_OS::memcpy (guid.get_buffer (),
                          &temp,
                          sizeof(size_t));

          size_t id;
          ACE_OS::memcpy (&id,
                          guid.get_buffer (),
                          guid.length ());

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "The Guid is %d %d\n",
                        id,
                        TAO_RTScheduler_Current::guid_counter.value_i ()));

          // Create new DT.
          RTScheduling::DistributableThread_var dt =
            TAO_DistributableThread_Factory::create_DT ();

          // Add new DT to map.
          int result = current->dt_hash ()->bind (guid, dt);
          if (result != 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "No Scheduling Segment Context\n"));
              throw ::CORBA::INTERNAL ();

            }

          // @@ Store implicit_sched_param in a var

          // Create new temporary current. Note that
          // the new <sched_param> is the current
          // <implicit_sched_param> and there is no
          // segment name.
          CORBA::Policy_var implicit_sched_param =
            current->implicit_scheduling_parameter ();
          ACE_NEW (new_current,
                   TAO_RTScheduler_Current_i (current->orb (),
                                              current->dt_hash (),
                                              guid,
                                              0,
                                              implicit_sched_param.in (),
                                              0,
                                              dt.in (),
                                              current));

          // Install new current in the ORB.
          //current->implementation (new_current);
          tss->rtscheduler_current_impl_ = new_current;

        }

      // Scheduler populates the service context with
      // scheduling parameters.
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->send_request (ri);

      // If this is a one way request
      if (!ri->response_expected ())
        {
          // Cleanup temporary DT.
          new_current->cleanup_DT ();

          //Restore old current
          new_current->cleanup_current ();
        }
    }
}

void
Client_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Client_Interceptor::send_poll\n"));

    TAO_RTScheduler_Current_i *current = 0;

    TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

    current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
    if (current != 0)
      {
        RTScheduling::Scheduler_var scheduler = current->scheduler ();
        scheduler->send_poll (ri);
      }
}

void
Client_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Client_Interceptor::receive_reply\n"));

  TAO_RTScheduler_Current_i *current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->receive_reply (ri);
    }
}

void
Client_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Client_Interceptor::receive_exception\n"));

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  TAO_RTScheduler_Current_i *current =
    static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);

  if (current != 0)
    {
      if (ri == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ri = 0\n"));
          return;
        }

      CORBA::Any_var ex = ri->received_exception ();
      CORBA::TypeCode_var type = ex->type ();

      if (CORBA::is_nil (type.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "type = 0\n"));
          return;
        }
      const char * id = type->id ();

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Received Exception %C\n",
                    id));

      // If the remote host threw a THREAD_CANCELLED
      // exception, make sure to take the appropriate
      // local action.
      if (ACE_OS::strstr (id, "CORBA::THREAD_CANCELLED") == 0)
        {
          // Perform the necessary cleanup as the
          // thread was cancelled.
          current->cancel_thread ();
        }
      else
        {
          // Inform scheduler that exception was
          // received.
          RTScheduling::Scheduler_var scheduler = current->scheduler ();
          scheduler->receive_exception (ri);
        }
    }
}

void
Client_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Client_Interceptor::receive_other\n"));

  TAO_RTScheduler_Current_i *current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->receive_other (ri);
    }
}

char*
Client_Interceptor::name (void)
{
  return  CORBA::string_dup ("RTSchdeuler_Client_Interceptor");
}

void
Client_Interceptor::destroy (void)
{
}

const IOP::ServiceId
Server_Interceptor::SchedulingInfo = 30;

Server_Interceptor::Server_Interceptor (TAO_RTScheduler_Current_ptr current)
{
  this->current_ = TAO_RTScheduler_Current::_duplicate (current);
}

void
Server_Interceptor::receive_request_service_contexts (
  PortableInterceptor::ServerRequestInfo_ptr)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Server_Interceptor::receive_request_service_contexts\n"));

}

void
Server_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                     )
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Server_Interceptor::receive_request\n"));

  IOP::ServiceContext_var serv_cxt;

  try
    {
      serv_cxt =
        ri->get_request_service_context (Server_Interceptor::SchedulingInfo);
    }
  catch (const ::CORBA::Exception&)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Invalid Service Request\n"));
      return;
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Request from Distributable Thread\n"));

  RTScheduling::Current::IdType_var guid_var;
  char* name = 0;
  CORBA::Policy_var sched_param = 0;
  CORBA::Policy_var implicit_sched_param = 0;

  TAO_RTScheduler_Current_i* new_current = 0;
  ACE_NEW_THROW_EX (new_current,
                    TAO_RTScheduler_Current_i (this->current_->orb (),
                                               this->current_->dt_hash ()),
                    CORBA::NO_MEMORY (
                           CORBA::SystemException::_tao_minor_code (
                             TAO::VMCID,
                             ENOMEM),
                           CORBA::COMPLETED_NO));

  // Scheduler retrieves scheduling parameters
  // from request and populates the out
  // parameters.
  RTScheduling::Scheduler_var scheduler = new_current->scheduler();
  scheduler->receive_request (ri,
                              guid_var.out (),
                              name,
                              sched_param.out (),
                              implicit_sched_param.out ());

  if (guid_var->length () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "The scheduler MUST retreive and return the "
                  "GUID from the service context\n"));
      return;
    }
  RTScheduling::Current::IdType guid;
  guid.length (sizeof (size_t));
  ACE_OS::memcpy (guid.get_buffer (),
                  guid_var->get_buffer (),
                  sizeof (size_t));

  size_t id;
  ACE_OS::memcpy (&id,
                  guid.get_buffer (),
                  guid.length ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "The Guid is %d\n",
                id));

  // Create new DT.
  RTScheduling::DistributableThread_var dt = TAO_DistributableThread_Factory::create_DT ();

  // Add new DT to map.
  int result = new_current->dt_hash ()->bind (guid, dt);

  if (result != 0)
    {
      throw ::CORBA::INTERNAL ();
    }

  // Create new temporary current. Note that the new <sched_param> is
  // the current <implicit_sched_param> and there is no segment name.
  new_current->id (guid);
  new_current->name (name);
  new_current->scheduling_parameter (sched_param.in ());
  new_current->implicit_scheduling_parameter (implicit_sched_param.in ());
  new_current->DT (dt.in ());

  // Install new current in the ORB and store the previous current
  // implementation
  // current->implementation (new_current)
  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  tss->rtscheduler_previous_current_impl_ = this->current_->implementation (new_current);
}

void
Server_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Server_Interceptor::send_reply\n"));

  TAO_RTScheduler_Current_i *current = 0;
  TAO_RTScheduler_Current_i *prev_current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      RTScheduling::DistributableThread_var dt = current->DT ();
      if (dt->state () == RTScheduling::DistributableThread::CANCELLED)
        {
          current->cancel_thread ();

          return;
        }
      else ACE_DEBUG ((LM_DEBUG,
                       "Thread Not Cancelled\n"));


      // Inform scheduler that upcall is complete.
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->send_reply (ri);

      current->cleanup_DT ();
      current->cleanup_current ();

      // Get the previous current if any.
      prev_current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_previous_current_impl_);

      // Restore the previous current.
      tss->rtscheduler_current_impl_ = prev_current;

      // Reset the previous current pointer.
      tss->rtscheduler_previous_current_impl_ = 0;

    }
  else ACE_DEBUG ((LM_DEBUG,
                   "Send Reply Current is 0\n"));
}

void
Server_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Server_Interceptor::send_exception\n"));

  TAO_RTScheduler_Current_i *current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      // Inform scheduler that upcall is complete.
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->send_exception (ri);

      current->cleanup_DT ();
      current->cleanup_current ();
    }
}

void
Server_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Server_Interceptor::send_other\n"));

  TAO_RTScheduler_Current_i *current = 0;

  TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

  current = static_cast<TAO_RTScheduler_Current_i *> (tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      // Inform scheduler that upcall is complete.
      RTScheduling::Scheduler_var scheduler = current->scheduler ();
      scheduler->send_other (ri);

      current->cleanup_DT ();
      current->cleanup_current ();
    }
}

char*
Server_Interceptor::name (void)
{
    return  CORBA::string_dup ("RTSchdeuler_Server_Interceptor");
}

void
Server_Interceptor::destroy (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
