//$Id$

#include "FP_Scheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "test.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> server_guid_counter;

RTCORBA::Priority
Segment_Sched_Param_Policy::value (void)
{
  return this->value_;
}

void
Segment_Sched_Param_Policy::value (RTCORBA::Priority value)
{
  this->value_ = value;
}

CORBA::Policy_ptr
Segment_Sched_Param_Policy::copy (void)
{
  Segment_Sched_Param_Policy *copy = 0;
  ACE_NEW_THROW_EX (copy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY ());

  copy->value (this->value_);

  return copy;
}

void
Segment_Sched_Param_Policy::destroy (void)
{
}

Fixed_Priority_Scheduler::Fixed_Priority_Scheduler (CORBA::ORB_ptr orb)
{
  try
    {
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTScheduler_Current");

      this->current_ =
        RTScheduling::Current::_narrow (object.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}

Fixed_Priority_Scheduler::~Fixed_Priority_Scheduler (void)
{
}

FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
Fixed_Priority_Scheduler::create_segment_scheduling_parameter (RTCORBA::Priority segment_priority)
{
  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr segment_policy;
  ACE_NEW_THROW_EX (segment_policy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                                                                               TAO::VMCID,
                                                                               ENOMEM),
                                      CORBA::COMPLETED_NO));

  segment_policy->value (segment_priority);

  return segment_policy;

}


void
Fixed_Priority_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &,
                                                        const char *,
                                                        CORBA::Policy_ptr /*sched_policy*/,
                                                        CORBA::Policy_ptr)
{
}

void
Fixed_Priority_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                                           const char *name,
                                                           CORBA::Policy_ptr sched_param,
                                                           CORBA::Policy_ptr implicit_sched_param)
{
  this->begin_new_scheduling_segment (guid,
                                      name,
                                      sched_param,
                                      implicit_sched_param);
}

void
Fixed_Priority_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                                     const char *name,
                                                     CORBA::Policy_ptr sched_param,
                                                     CORBA::Policy_ptr implicit_sched_param)
{
  this->begin_new_scheduling_segment (guid,
                                      name,
                                      sched_param,
                                      implicit_sched_param);

}

void
Fixed_Priority_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &,
                                                  const char *)
{
}

void
Fixed_Priority_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                         const char *,
                                                         CORBA::Policy_ptr)
{

}


void
Fixed_Priority_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info)
{

  int priority;
  ACE_hthread_t current;
  ACE_Thread::self (current);
  if (ACE_Thread::getprio (current, priority) == -1)
    return;

  ACE_DEBUG ((LM_DEBUG,
              "Request thread priority is %d %d\n",
              priority,
              ACE_DEFAULT_THREAD_PRIORITY));


  IOP::ServiceContext* srv_con = new IOP::ServiceContext;
  srv_con->context_id = Client_Interceptor::SchedulingInfo;
  srv_con->context_data.length (sizeof (size_t));
  ACE_OS::memcpy (srv_con->context_data.get_buffer (),
                  current_->id ()->get_buffer (),
                  sizeof (size_t));
  request_info->add_request_service_context (*srv_con,
                                             0);

}

void
Fixed_Priority_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
                                           RTScheduling::Current::IdType_out guid_out,
                                           CORBA::String_out /*name*/,
                                           CORBA::Policy_out /*sched_param*/,
                                           CORBA::Policy_out /*implicit_sched_param*/)
{
  IOP::ServiceContext* serv_cxt =
    request_info->get_request_service_context (Server_Interceptor::SchedulingInfo);

  if (serv_cxt != 0)
    {
      size_t gu_id;
      ACE_OS::memcpy (&gu_id,
                      serv_cxt->context_data.get_buffer (),
                      serv_cxt->context_data.length ());

      char msg [BUFSIZ];
      ACE_OS::sprintf (msg,
                       "The Guid is "
                       ACE_SIZE_T_FORMAT_SPECIFIER
                       "\n", gu_id);

      DT_TEST::instance ()->dt_creator ()->log_msg (msg);

      RTScheduling::Current::IdType* guid;
      ACE_NEW (guid,
               RTScheduling::Current::IdType);


      // Generate GUID.
      guid->length (sizeof(size_t));

      ACE_OS::memcpy (guid->get_buffer (),
                      &gu_id,
                      sizeof(size_t));

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Receive request The Guid is %d\n",
                    gu_id));

      guid_out.ptr () = guid;
    }

}

void
Fixed_Priority_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::cancel (const RTScheduling::Current::IdType &)
{
}

CORBA::PolicyList*
Fixed_Priority_Scheduler::scheduling_policies (void)
{
  return 0;
}

void
Fixed_Priority_Scheduler::scheduling_policies (const CORBA::PolicyList &)
{
}

CORBA::PolicyList*
Fixed_Priority_Scheduler::poa_policies (void)
{
  return 0;
}

char *
Fixed_Priority_Scheduler::scheduling_discipline_name (void)
{
  return 0;
}

RTScheduling::ResourceManager_ptr
Fixed_Priority_Scheduler::create_resource_manager (const char *,
                                                   CORBA::Policy_ptr)
{
  return 0;
}

void
Fixed_Priority_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                                    const char *,
                                                    CORBA::Policy_ptr)
{
}
