//$Id$

#include "MUF_Scheduler.h"
#include "ace/Atomic_Op.h"
#include "../Kokyu_qosC.h"

//this needs to be formally defined in IOP.pidl
namespace
{
  static const IOP::ServiceId service_id = 0xdddd;
}

ACE_Time_Value time_base_to_tv (TimeBase::TimeT time)
{
  ACE_Time_Value tv(0,time/10);

  return tv;
}

void guid_copy( Kokyu::GuidType& lhs, const RTScheduling::Current::IdType& rhs)
{
  lhs.length(rhs.length ());
  ACE_OS::memcpy(lhs.get_buffer (),
                 rhs.get_buffer (),
                 rhs.length ());
}

void guid_copy( RTScheduling::Current::IdType& lhs, const Kokyu::GuidType& rhs)
{
  lhs.length(rhs.length ());
  ACE_OS::memcpy(lhs.get_buffer (),
                 rhs.get_buffer (),
                 rhs.length ());
}

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

MUF_Scheduling::SchedulingParameter 
MUF_Sched_Param_Policy::value (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

void
MUF_Sched_Param_Policy::value (const MUF_Scheduling::SchedulingParameter& value)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->value_ = value;
}

MUF_Scheduler::MUF_Scheduler (CORBA::ORB_ptr orb)
  : orb_ (orb)
{
  Kokyu::DSRT_ConfigInfo config;

  config.sched_strategy_ = Kokyu::DSRT_MUF;
  kokyu_dispatcher_ = Kokyu::Dispatcher_Factory::create_DSRT_dispatcher (config);

  CORBA::Object_var object =
    orb->resolve_initial_references ("RTScheduler_Current"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->current_ =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  IOP::CodecFactory_var codec_factory;
  CORBA::Object_var obj = orb->resolve_initial_references ("CodecFactory");

  if (CORBA::is_nil(obj.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Nil Codec factory\n"));
    }
  else
    {
      codec_factory = IOP::CodecFactory::_narrow (obj.in ());
    }

  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = 1;
  encoding.minor_version = 2;

  codec_ = codec_factory->create_codec (encoding);
}

MUF_Scheduler::~MUF_Scheduler (void)
{
  delete kokyu_dispatcher_;
}

void
MUF_Scheduler::shutdown (void)
{
  kokyu_dispatcher_->shutdown ();
}

MUF_Scheduling::SchedulingParameterPolicy_ptr
MUF_Scheduler::create_scheduling_parameter (const MUF_Scheduling::SchedulingParameter & value)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  MUF_Scheduling::SchedulingParameterPolicy_ptr sched_param_policy;
  ACE_NEW_THROW_EX (sched_param_policy,
                    MUF_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                                       TAO_DEFAULT_MINOR_CODE,
                                       ENOMEM),
                                      CORBA::COMPLETED_NO));

  sched_param_policy->value (value);

  return sched_param_policy;
}


void
MUF_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
                                             const char *,
                                             CORBA::Policy_ptr sched_policy,
                                             CORBA::Policy_ptr
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  int count;
  ACE_OS::memcpy (&count,
                  this->current_->id ()->get_buffer (),
                  this->current_->id ()->length ());

  Kokyu::DSRT_QoSDescriptor qos;
  MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
    MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

  MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();

  qos.deadline_ = time_base_to_tv (sched_param->deadline);
  qos.exec_time_ = time_base_to_tv (sched_param->estimated_initial_execution_time);
  qos.criticality_ = sched_param->criticality;
  
  kokyu_dispatcher_->schedule (count, qos);
}

void
MUF_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                                           const char *name,
                                                           CORBA::Policy_ptr sched_param,
                                                           CORBA::Policy_ptr implicit_sched_param
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  this->begin_new_scheduling_segment (guid,
                                      name,
                                      sched_param,
                                      implicit_sched_param
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
MUF_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
                                          const char */*name*/,
                                          CORBA::Policy_ptr sched_policy,
                                          CORBA::Policy_ptr /*implicit_sched_param*/
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  int count ;
  ACE_OS::memcpy (&count,
                  this->current_->id ()->get_buffer (),
                  this->current_->id ()->length ());

  MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
    MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

  MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();
  Kokyu::DSRT_QoSDescriptor qos;

  qos.deadline_ = time_base_to_tv (sched_param->deadline);
  qos.exec_time_ = time_base_to_tv (sched_param->estimated_initial_execution_time);
  qos.criticality_ = sched_param->criticality;

  kokyu_dispatcher_->update_schedule (count, qos);
}

void
MUF_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                       const char *
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int count;
  ACE_OS::memcpy (&count,
                  guid.get_buffer (),
                  guid.length ());


  ACE_DEBUG ((LM_DEBUG, "(%t) call to end_sched_segment for guid %d\n", count));
  kokyu_dispatcher_->cancel_schedule (count);
}

void
MUF_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                         const char *,
                                                         CORBA::Policy_ptr
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}


void
MUF_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%t): send_request "
              "from \"%s\"\n",
              operation.in ()));

  // Make the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = ::service_id;

  CORBA::Policy_ptr sched_policy =
    this->current_->scheduling_parameter(ACE_ENV_ARG_PARAMETER);

  int guid;
  ACE_OS::memcpy (&guid,
                  this->current_->id ()->get_buffer (),
                  this->current_->id ()->length ());

  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  if (CORBA::is_nil (sched_policy))
    {
      //24 hrs from now - infinity
      ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (24*60*60,0);
      deadline = deadline_tv.sec () * 1000000 + deadline_tv.usec () * 10; //100s of nanoseconds for TimeBase::TimeT
      exec_time = 0;
      criticality = 0;
    }
  else
    {
      MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
        MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

      MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();
      deadline = sched_param->deadline;
      exec_time = sched_param->estimated_initial_execution_time;
      criticality = sched_param->criticality;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t): send_request criticality from current = %d\n"),
                  criticality));

      Kokyu::Svc_Ctxt_DSRT_QoS sc_qos;

      //Fill the guid in the SC Qos struct
      sc_qos.guid.length (this->current_->id ()->length ());
      //sc_qos.guid = *(this->current_->id ());
      guid_copy (sc_qos.guid, *(this->current_->id ()));
      sc_qos.deadline = deadline;
      sc_qos.estimated_initial_execution_time = exec_time;
      sc_qos.criticality = criticality;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;

      ACE_TRY_NEW_ENV
        {
          sc.context_data =
            ACE_reinterpret_cast(IOP::ServiceContext::_tao_seq_Octet &,
                                 *codec_->encode (sc_qos_as_any));
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Exception caught:");
        }
      ACE_ENDTRY;


      ACE_DEBUG ((LM_DEBUG, "after encode\n"));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t): send_request : about to add sched SC\n")));

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t): send_request : about to call scheduler to inform block\n")
              ));

  kokyu_dispatcher_->update_schedule (guid, Kokyu::BLOCK);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t): guid = %d, send_request interceptor done\n"),
              guid));
}

void
MUF_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out /*name*/,
                                CORBA::Policy_out sched_param_out,
                                CORBA::Policy_out /*implicit_sched_param_out*/
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG, "entered MUF_Scheduler::receive_request\n"));
  RTScheduling::Current::IdType* guid;
  ACE_NEW (guid,
           RTScheduling::Current::IdType);


  // Generate GUID.
  guid->length (sizeof(long));

  long temp = ++server_guid_counter;
  ACE_OS::memcpy (guid->get_buffer (),
                  &temp,
                  sizeof(long));

  int id;
  ACE_OS::memcpy (&id,
                  guid->get_buffer (),
                  guid->length ());

  guid_out.ptr () = guid;

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%t): receive_request from "
              "\"%s\"\n",
              operation.in ()));

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  if (ACE_OS_String::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceContext_var sc =
    ri->get_request_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  if (sc.ptr () == 0)
    {
      //24 hrs from now - infinity
      ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (24*60*60,0);
      deadline = deadline_tv.sec ()*1000000 + deadline_tv.usec ()*10; //100s of nanoseconds for TimeBase::TimeT
      exec_time = 0;
      criticality = 0;
    }
  else
    {
      CORBA::OctetSeq oc_seq = CORBA::OctetSeq (sc->context_data.length (),
                                                 sc->context_data.length (),
                                                 sc->context_data.get_buffer (),
                                                 0);
      //Don't store in a _var, since >>= returns a pointer to an internal buffer
      //and we are not supposed to free it.
      Kokyu::Svc_Ctxt_DSRT_QoS* sc_qos_ptr;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any = *codec_->decode (oc_seq);
      sc_qos_as_any >>= sc_qos_ptr;

      deadline  = sc_qos_ptr->deadline;
      criticality = sc_qos_ptr->criticality;
      exec_time = sc_qos_ptr->estimated_initial_execution_time;

      //*(guid_out.ptr ()) = sc_qos_ptr->guid;
      guid_copy (*(guid_out.ptr ()), sc_qos_ptr->guid);


      ACE_OS::memcpy (&id,
                      sc_qos_ptr->guid.get_buffer (),
                      sc_qos_ptr->guid.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): Criticality = %d, guid = %d in recvd service context\n",
                  criticality,
                  id));

      MUF_Scheduling::SchedulingParameter sched_param;
      sched_param.criticality = criticality;
      sched_param.deadline = deadline;
      sched_param_out.ptr () = this->create_scheduling_parameter (sched_param);
    }

  Kokyu::DSRT_QoSDescriptor qos;
  qos.criticality_ = criticality;
  qos.deadline_ = time_base_to_tv (deadline);
  qos.exec_time_ = time_base_to_tv (exec_time);

  this->kokyu_dispatcher_->schedule (id, qos);

  ACE_DEBUG ((LM_DEBUG, "(%t): guid = %d, receive_request interceptor done\n", id));
}

void
MUF_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%t): send_reply from \"%s\"\n",
              ri->operation ()));

  // Make the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = ::service_id;

  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  CORBA::Policy_ptr sched_policy =
    this->current_->scheduling_parameter(ACE_ENV_ARG_PARAMETER);

  int guid;
  ACE_OS::memcpy (&guid,
                  this->current_->id ()->get_buffer (),
                  this->current_->id ()->length ());

  if (CORBA::is_nil (sched_policy))
  {
    ACE_DEBUG ((LM_DEBUG, "sched_policy nil\n"));

    //24 hrs from now - infinity
    ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (24*60*60,0);
    deadline = deadline_tv.sec ()*1000000 + deadline_tv.usec ()*10; //100s of nanoseconds for TimeBase::TimeT
    exec_time = 0;
    criticality = 0;
  }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "sched_policy not nil\n"));

      MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
        MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

      MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();

      Kokyu::Svc_Ctxt_DSRT_QoS sc_qos;
      //Fill the guid in the SC Qos struct
      //sc_qos.guid = *(this->current_->id ());
      guid_copy ( sc_qos.guid, *(this->current_->id ()));

      deadline = sched_param->deadline;
      exec_time = sched_param->estimated_initial_execution_time;
      criticality = sched_param->criticality;
      sc_qos.deadline = deadline;
      sc_qos.estimated_initial_execution_time = exec_time;
      sc_qos.criticality = criticality;

      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;
      
      sc.context_data = ACE_reinterpret_cast(
					     IOP::ServiceContext::_tao_seq_Octet &,
					     *codec_->encode (sc_qos_as_any));
      
      // Add this context to the service context list.
      ri->add_reply_service_context (sc, 1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      
      ACE_DEBUG ((LM_DEBUG, "reply sc added\n"));
    }
  
  kokyu_dispatcher_->update_schedule (guid, Kokyu::BLOCK);
  
  ACE_DEBUG ((LM_DEBUG, "(%t): guid = %d, send_reply interceptor done\n", guid));
}

void
MUF_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
MUF_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
MUF_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int guid = 0;

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "receive_reply from "
              "\"%s\"\n",
              operation.in ()));

  // Check that the reply service context was received as
  // expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (::service_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  if (sc.ptr () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "service context was not filled\n"));
      //24 hrs from now - infinity
      ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (24*60*60,0);
      deadline = deadline_tv.sec ()*1000000 + deadline_tv.usec ()*10; //100s of nanoseconds for TimeBase::TimeT
      exec_time = 0;
      criticality = 0;
    }
  else
    {
      CORBA::OctetSeq oc_seq = CORBA::OctetSeq (sc->context_data.length (),
                                                sc->context_data.length (),
                                                sc->context_data.get_buffer (),
                                                0);

      //Don't store in a _var, since >>= returns a pointer to an internal buffer
      //and we are not supposed to free it.
      Kokyu::Svc_Ctxt_DSRT_QoS* sc_qos_ptr;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any = *codec_->decode (oc_seq);
      sc_qos_as_any >>= sc_qos_ptr;

      deadline  = sc_qos_ptr->deadline;
      criticality = sc_qos_ptr->criticality;
      exec_time = sc_qos_ptr->estimated_initial_execution_time;

      ACE_OS::memcpy (&guid,
                      sc_qos_ptr->guid.get_buffer (),
                      sc_qos_ptr->guid.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "(%t): Criticality = %d, guid = %d in recvd service context\n",
                  criticality,
                  guid));
    }

  Kokyu::DSRT_QoSDescriptor qos;
  qos.criticality_ = criticality;
  qos.deadline_ = time_base_to_tv (deadline);
  qos.exec_time_ = time_base_to_tv (exec_time);
  this->kokyu_dispatcher_->schedule (guid, qos);
}

void
MUF_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
MUF_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
MUF_Scheduler::cancel (const RTScheduling::Current::IdType &
                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList*
MUF_Scheduler::scheduling_policies (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
        return 0;
}

void
MUF_Scheduler::scheduling_policies (const CORBA::PolicyList &
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList*
MUF_Scheduler::poa_policies (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
        return 0;
}

char *
MUF_Scheduler::scheduling_discipline_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
        return 0;
}

RTScheduling::ResourceManager_ptr
MUF_Scheduler::create_resource_manager (const char *,
                                        CORBA::Policy_ptr
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
        return 0;
}

void
MUF_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         const char *,
                                         CORBA::Policy_ptr
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
