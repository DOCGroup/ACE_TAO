#include "MUF_Scheduler.h"
#include "Kokyu_qosC.h"
#include "utils.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/ORB_Constants.h"

MUF_Sched_Param_Policy::MUF_Sched_Param_Policy ()
{
}

MUF_Sched_Param_Policy::MUF_Sched_Param_Policy (
    const MUF_Sched_Param_Policy &rhs
  )
  : CORBA::Object (),
  CORBA::Policy (),
  MUF_Scheduling::SchedulingParameterPolicy (),
  CORBA::LocalObject (),
  value_ (rhs.value_)
{
}

MUF_Scheduling::SchedulingParameter
MUF_Sched_Param_Policy::value (void)
{
  return this->value_;
}

void
MUF_Sched_Param_Policy::value (const MUF_Scheduling::SchedulingParameter& value)
{
  this->value_ = value;
}

CORBA::Policy_ptr
MUF_Sched_Param_Policy::copy (void)
{
  MUF_Sched_Param_Policy* tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    MUF_Sched_Param_Policy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

CORBA::PolicyType
MUF_Sched_Param_Policy::policy_type (void)
{
  return 0;
}

void
MUF_Sched_Param_Policy::destroy (void)
{
}

MUF_Scheduler::MUF_Scheduler (CORBA::ORB_ptr orb,
                              Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type,
                              int ace_sched_policy,
                              int ace_sched_scope)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    disp_impl_type_ (disp_impl_type),
    ace_sched_policy_ (ace_sched_policy),
    ace_sched_scope_ (ace_sched_scope)
{

  Kokyu::DSRT_ConfigInfo config;

  config.impl_type_ = this->disp_impl_type_;
  config.sched_policy_ = ace_sched_policy_;
  config.sched_scope_ = ace_sched_scope_;

  Kokyu::DSRT_Dispatcher_Factory<MUF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
    tmp( Kokyu::DSRT_Dispatcher_Factory<MUF_Scheduler_Traits>::
         create_DSRT_dispatcher (config) );
  kokyu_dispatcher_ = tmp;

  CORBA::Object_var object =
    orb->resolve_initial_references ("RTScheduler_Current");

  this->current_ =
    RTScheduling::Current::_narrow (object.in ());

  IOP::CodecFactory_var codec_factory;
  CORBA::Object_var obj =
    orb->resolve_initial_references ("CodecFactory");

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
}

void
MUF_Scheduler::shutdown (void)
{
  kokyu_dispatcher_->shutdown ();
  ACE_DEBUG ((LM_DEBUG, "kokyu DSRT dispatcher shutdown\n"));
}

MUF_Scheduling::SchedulingParameterPolicy_ptr
MUF_Scheduler::create_scheduling_parameter (const MUF_Scheduling::SchedulingParameter & value)
{
  MUF_Scheduling::SchedulingParameterPolicy_ptr sched_param_policy;
  ACE_NEW_THROW_EX (sched_param_policy,
                    MUF_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                                       TAO::VMCID,
                                       ENOMEM),
                                      CORBA::COMPLETED_NO));

  sched_param_policy->value (value);

  return sched_param_policy;
}


void
MUF_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                             const char *,
                                             CORBA::Policy_ptr sched_policy,
                                             CORBA::Policy_ptr)
{
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):MUF_Scheduler::begin_new_scheduling_segment enter\n"));
#endif

#ifdef KOKYU_DSRT_LOGGING
  int int_guid;
  ACE_OS::memcpy (&int_guid,
                  guid.get_buffer (),
                  guid.length ());
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): guid is %d\n", int_guid));
#endif

  MUF_Scheduler_Traits::QoSDescriptor_t qos;
  MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
    MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

  MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();

  qos.deadline_ = sched_param->deadline;
  qos.exec_time_ = sched_param->estimated_initial_execution_time;
  qos.criticality_ = sched_param->criticality;

  kokyu_dispatcher_->schedule (guid, qos);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):MUF_Scheduler::begin_new_scheduling_segment exit\n"));
#endif
}


void
MUF_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &guid,
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
MUF_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType& guid,
                                          const char* name,
                                          CORBA::Policy_ptr sched_policy,
                                          CORBA::Policy_ptr implicit_sched_param)
{
  ACE_UNUSED_ARG ((name));
  ACE_UNUSED_ARG ((implicit_sched_param));

#ifdef KOKYU_DSRT_LOGGING
  int int_guid ;
  ACE_OS::memcpy (&int_guid,
                  guid.get_buffer (),
                  guid.length ());
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): update_sched_seg::guid is %d\n", int_guid));
#endif

  MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
    MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy);

  MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();
  MUF_Scheduler_Traits::QoSDescriptor_t qos;

  qos.deadline_ = sched_param->deadline;
  qos.exec_time_ = sched_param->estimated_initial_execution_time;
  qos.criticality_ = sched_param->criticality;

  kokyu_dispatcher_->update_schedule (guid, qos);
}

void
MUF_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                       const char *)
{
#ifdef KOKYU_DSRT_LOGGING
  int int_guid;
  ACE_OS::memcpy (&int_guid,
                  guid.get_buffer (),
                  guid.length ());
  ACE_DEBUG ((LM_DEBUG, "(%t|%T) call to end_sched_segment for guid %d\n", int_guid));
#endif

  kokyu_dispatcher_->cancel_schedule (guid);
}

void
MUF_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                         const char *,
                                                         CORBA::Policy_ptr)
{
}


void
MUF_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  Kokyu::Svc_Ctxt_DSRT_QoS sc_qos;

  CORBA::String_var operation = ri->operation ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T): send_request "
              "from \"%s\"\n",
              operation.in ()));
#endif

  // Make the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = Client_Interceptor::SchedulingInfo;

  CORBA::Policy_var sched_policy =
    this->current_->scheduling_parameter();

  RTScheduling::Current::IdType_var guid = this->current_->id ();

  /*
  ACE_OS::memcpy (&guid,
                  guid->get_buffer (),
                  guid->length ());
  */

  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  if (CORBA::is_nil (sched_policy.in ()))
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
        MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy.in ());

      MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();
      deadline = sched_param->deadline;
      exec_time = sched_param->estimated_initial_execution_time;
      criticality = sched_param->criticality;

#ifdef KOKYU_DSRT_LOGGING
      int int_guid;
      ACE_OS::memcpy (&int_guid,
                      guid->get_buffer (),
                      guid->length ());
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): send_request guid = %d\n",
                  int_guid));
#endif

      //Fill the guid in the SC Qos struct
      sc_qos.guid.length (guid->length ());
      guid_copy (sc_qos.guid, guid.in ());
      sc_qos.deadline = deadline;
      sc_qos.estimated_initial_execution_time = exec_time;
      sc_qos.criticality = criticality;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;

      CORBA::OctetSeq_var cdtmp = codec_->encode (sc_qos_as_any);
      sc.context_data = cdtmp.in ();

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t|%T): send_request : about to add sched SC\n")));
#endif

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0);
    }


#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t|%T): send_request : ")
              ACE_TEXT ("about to call scheduler to inform block\n")
              ));
#endif

  kokyu_dispatcher_->update_schedule (guid.in (),
                                      Kokyu::BLOCK);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t|%T): send_request interceptor done\n")));
#endif
}

void
MUF_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out /*name*/,
                                CORBA::Policy_out sched_param_out,
                                CORBA::Policy_out /*implicit_sched_param_out*/)
{
  const Kokyu::Svc_Ctxt_DSRT_QoS* sc_qos_ptr = 0;

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):entered MUF_Scheduler::receive_request\n"));
#endif

  RTScheduling::Current::IdType guid;

  CORBA::String_var operation = ri->operation ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T): receive_request from "
              "\"%s\"\n",
              operation.in ()));
#endif

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  if (ACE_OS::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceContext_var sc =
    ri->get_request_service_context (Server_Interceptor::SchedulingInfo);

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
      CORBA::Any sc_qos_as_any;
      CORBA::Any_var scqostmp = codec_->decode (oc_seq);
      sc_qos_as_any = scqostmp.in ();
      //Don't store in a _var, since >>= returns a pointer to an
      //internal buffer and we are not supposed to free it.
      sc_qos_as_any >>= sc_qos_ptr;

      deadline  = sc_qos_ptr->deadline;
      criticality = sc_qos_ptr->criticality;
      exec_time = sc_qos_ptr->estimated_initial_execution_time;

      guid.length (sc_qos_ptr->guid.length ());
      guid_copy (guid, sc_qos_ptr->guid);

      ACE_NEW (guid_out.ptr (),
               RTScheduling::Current::IdType);
      guid_out.ptr ()->length (guid.length ());
      *(guid_out.ptr ()) = guid;

#ifdef KOKYU_DSRT_LOGGING
      int int_guid;
      ACE_OS::memcpy (&int_guid,
                      guid.get_buffer (),
                      guid.length ());
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Criticality = %d, guid = %d "
                  "in recvd service context\n",
                  criticality,
                  int_guid));
#endif
      MUF_Scheduling::SchedulingParameter sched_param;
      sched_param.criticality = criticality;
      sched_param.deadline = deadline;
      sched_param_out = this->create_scheduling_parameter (sched_param);
    }

  MUF_Scheduler_Traits::QoSDescriptor_t qos;
  qos.criticality_ = criticality;
  qos.deadline_ = deadline;
  qos.exec_time_ = exec_time;

  this->kokyu_dispatcher_->schedule (guid, qos);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): receive_request interceptor done\n"));
#endif

}

void
MUF_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
MUF_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{

  Kokyu::Svc_Ctxt_DSRT_QoS sc_qos;

  CORBA::String_var operation = ri->operation ();

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T): send_reply from \"%s\"\n",
              ri->operation ()));
#endif

  // Make the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = Server_Interceptor::SchedulingInfo;


  CORBA::Long criticality;
  TimeBase::TimeT deadline,exec_time;

  CORBA::Policy_var sched_policy =
    this->current_->scheduling_parameter();

  RTScheduling::Current::IdType_var guid = this->current_->id ();

  if (CORBA::is_nil (sched_policy.in ()))
  {
#ifdef KOKYU_DSRT_LOGGING
    ACE_DEBUG ((LM_DEBUG,
                "(%t|%T): sched_policy nil.\n"));
#endif
    //24 hrs from now - infinity
    ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (24*60*60,0);
    deadline = deadline_tv.sec ()*1000000 + deadline_tv.usec ()*10; //100s of nanoseconds for TimeBase::TimeT
    exec_time = 0;
    criticality = 0;
  }
  else
    {
#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T):sched_policy not nil. ",
                  "sched params set\n"));
#endif
      MUF_Scheduling::SchedulingParameterPolicy_var sched_param_policy =
        MUF_Scheduling::SchedulingParameterPolicy::_narrow (sched_policy.in ());
      MUF_Scheduling::SchedulingParameter_var sched_param = sched_param_policy->value ();


      sc_qos.guid.length (guid->length ());
      guid_copy (sc_qos.guid, guid.in ());

      deadline = sched_param->deadline;
      exec_time = sched_param->estimated_initial_execution_time;
      criticality = sched_param->criticality;
      sc_qos.deadline = deadline;
      sc_qos.estimated_initial_execution_time = exec_time;
      sc_qos.criticality = criticality;

      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;

      CORBA::OctetSeq_var cdtmp = codec_->encode (sc_qos_as_any);
      sc.context_data = cdtmp.in ();

      // Add this context to the service context list.
      ri->add_reply_service_context (sc, 1);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):reply sc added\n"));
#endif
    }

  kokyu_dispatcher_->update_schedule (guid.in (),
                                      Kokyu::BLOCK);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): send_reply interceptor done\n"));
#endif
}

void
MUF_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  send_reply (ri);
}

void
MUF_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  send_reply (ri);
}

void
MUF_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  RTScheduling::Current::IdType guid;

  CORBA::String_var operation = ri->operation ();

  CORBA::Object_var target = ri->target ();

  ACE_CString opname = operation.in ();
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):receive_reply from "
              "\"%s\"\n",
              opname.c_str ()));
#endif

  // Check that the reply service context was received as
  // expected.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (Client_Interceptor::SchedulingInfo);

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
      const Kokyu::Svc_Ctxt_DSRT_QoS* sc_qos_ptr = 0;
      CORBA::Any sc_qos_as_any;
      CORBA::Any_var scqostmp = codec_->decode (oc_seq);
      sc_qos_as_any = scqostmp.in ();
      sc_qos_as_any >>= sc_qos_ptr;

      deadline  = sc_qos_ptr->deadline;
      criticality = sc_qos_ptr->criticality;
      exec_time = sc_qos_ptr->estimated_initial_execution_time;

      guid.length (sc_qos_ptr->guid.length ());
      guid_copy (guid, sc_qos_ptr->guid);

      ACE_DEBUG ((LM_DEBUG,
                  "(%t|%T): Criticality = %d in recvd service context\n",
                  criticality));
    }

  MUF_Scheduler_Traits::QoSDescriptor_t qos;
  qos.deadline_ =   qos.criticality_ = criticality;
  qos.deadline_ = deadline;
  qos.exec_time_ = exec_time;
  this->kokyu_dispatcher_->schedule (guid, qos);
}

void
MUF_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  receive_reply (ri);
}

void
MUF_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  receive_reply (ri);
}

void
MUF_Scheduler::cancel (const RTScheduling::Current::IdType &)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::PolicyList*
MUF_Scheduler::scheduling_policies (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
MUF_Scheduler::scheduling_policies (const CORBA::PolicyList &)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::PolicyList*
MUF_Scheduler::poa_policies (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

char *
MUF_Scheduler::scheduling_discipline_name (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

RTScheduling::ResourceManager_ptr
MUF_Scheduler::create_resource_manager (const char *,
                                        CORBA::Policy_ptr)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
MUF_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         const char *,
                                         CORBA::Policy_ptr)
{
  throw CORBA::NO_IMPLEMENT ();
}

