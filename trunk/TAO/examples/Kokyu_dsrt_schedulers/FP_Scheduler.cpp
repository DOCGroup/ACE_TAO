//$Id$

#include "FP_Scheduler.h"
#include "Kokyu_qosC.h"
#include "utils.h"
#include "tao/ORB_Constants.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/RTScheduling/Request_Interceptor.h"

FP_Segment_Sched_Param_Policy::FP_Segment_Sched_Param_Policy ()
{
}

FP_Segment_Sched_Param_Policy::FP_Segment_Sched_Param_Policy (
    const FP_Segment_Sched_Param_Policy &rhs)
  : CORBA::Object (),
  CORBA::Policy (),
  FP_Scheduling::SegmentSchedulingParameterPolicy (),
  CORBA::LocalObject (),
  value_ (rhs.value_)
{
}

FP_Scheduling::SegmentSchedulingParameter
FP_Segment_Sched_Param_Policy::value (void)
{
  return value_;
}

void
FP_Segment_Sched_Param_Policy::value (
        const FP_Scheduling::SegmentSchedulingParameter & value)
{
  this->value_ = value;
}

CORBA::Policy_ptr
FP_Segment_Sched_Param_Policy::copy (void)
{
  FP_Segment_Sched_Param_Policy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, FP_Segment_Sched_Param_Policy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
FP_Segment_Sched_Param_Policy::destroy (void)
{
}

Fixed_Priority_Scheduler::Fixed_Priority_Scheduler (
  CORBA::ORB_ptr orb,
  Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type,
  int ace_sched_policy,
  int ace_sched_scope)
  : orb_ (orb),
    disp_impl_type_ (disp_impl_type),
    ace_sched_policy_ (ace_sched_policy),
    ace_sched_scope_ (ace_sched_scope)
{

  Kokyu::DSRT_ConfigInfo config;

  config.impl_type_ = this->disp_impl_type_;
  config.sched_policy_ = ace_sched_policy_;
  config.sched_scope_ = ace_sched_scope_;

  Kokyu::DSRT_Dispatcher_Factory<FP_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
    tmp( Kokyu::DSRT_Dispatcher_Factory<FP_Scheduler_Traits>::
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

Fixed_Priority_Scheduler::~Fixed_Priority_Scheduler (void)
{
  //  delete kokyu_dispatcher_;
}

void
Fixed_Priority_Scheduler::shutdown (void)
{
  kokyu_dispatcher_->shutdown ();
  ACE_DEBUG ((LM_DEBUG, "kokyu DSRT dispatcher shutdown\n"));
}

FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
Fixed_Priority_Scheduler::create_segment_scheduling_parameter (
        const FP_Scheduling::SegmentSchedulingParameter & value
      )
{
  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
    segment_sched_param_policy;
  ACE_NEW_THROW_EX (segment_sched_param_policy,
                    FP_Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                                       TAO::VMCID,
                                       ENOMEM),
                                      CORBA::COMPLETED_NO));

  segment_sched_param_policy->value (value);

  return segment_sched_param_policy;
}


void
Fixed_Priority_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType& guid,
                                             const char *,
                                             CORBA::Policy_ptr sched_policy,
                                             CORBA::Policy_ptr)
{
#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):FP_Scheduler::begin_new_scheduling_segment enter\n"));
#endif

#ifdef KOKYU_DSRT_LOGGING
  int int_guid;
  ACE_OS::memcpy (&int_guid,
                  guid.get_buffer (),
                  guid.length ());
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): guid is %d\n", int_guid));
#endif

  FP_Scheduler_Traits::QoSDescriptor_t qos;
  FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_policy =
    FP_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

  FP_Scheduling::SegmentSchedulingParameter sched_param = sched_param_policy->value ();
  RTCORBA::Priority desired_priority = sched_param.base_priority;
  qos.priority_ = desired_priority;
  kokyu_dispatcher_->schedule (guid, qos);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "(%t|%T):FP_Scheduler::begin_new_scheduling_segment exit\n"));
#endif
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

  FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_policy =
    FP_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

  FP_Scheduling::SegmentSchedulingParameter sched_param =
    sched_param_policy->value ();

  RTCORBA::Priority desired_priority = sched_param.base_priority;

  FP_Scheduler_Traits::QoSDescriptor_t qos;
  qos.priority_ = desired_priority;

  kokyu_dispatcher_->update_schedule (guid, qos);
}

void
Fixed_Priority_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &guid,
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
Fixed_Priority_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                         const char *,
                                                         CORBA::Policy_ptr)
{

}


void
Fixed_Priority_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
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

  CORBA::Policy_ptr sched_policy =
    this->current_->scheduling_parameter();
  /*
  int guid;
  ACE_OS::memcpy (&guid,
                  this->current_->id ()->get_buffer (),
                  this->current_->id ()->length ());
  */
  RTCORBA::Priority desired_priority;
  if (CORBA::is_nil (sched_policy))
    {
      desired_priority = 0;
    }
  else
    {
      FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_policy =
        FP_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

      FP_Scheduling::SegmentSchedulingParameter sched_param =
        sched_param_policy->value ();

      desired_priority = sched_param.base_priority;

#ifdef KOKYU_DSRT_LOGGING
      int int_guid;
      ACE_OS::memcpy (&int_guid,
                      this->current_->id ()->get_buffer (),
                      this->current_->id ()->length ());
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t): send_request desired_priority from current = %d, guid = %d\n"),
                  desired_priority, int_guid));
#endif

      //Fill the guid in the SC Qos struct
      sc_qos.guid.length (this->current_->id ()->length ());
      guid_copy (sc_qos.guid, *(this->current_->id ()));
      sc_qos.desired_priority = desired_priority;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;

      sc.context_data =
        reinterpret_cast<CORBA::OctetSeq &> (*codec_->encode (sc_qos_as_any));

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

  kokyu_dispatcher_->update_schedule (*(this->current_->id ()),
                                        Kokyu::BLOCK);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t|%T): send_request interceptor done\n")));
#endif
}

void
Fixed_Priority_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out /*name*/,
                                CORBA::Policy_out sched_param_out,
                                CORBA::Policy_out /*implicit_sched_param_out*/)
{
  Kokyu::Svc_Ctxt_DSRT_QoS* sc_qos_ptr;

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):entered FP_Scheduler::receive_request\n"));
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

  RTCORBA::Priority desired_priority;

  if (sc.ptr () == 0)
    {
      desired_priority = 0;
    }
  else
    {
      CORBA::OctetSeq oc_seq = CORBA::OctetSeq (sc->context_data.length (),
                                                 sc->context_data.length (),
                                                 sc->context_data.get_buffer (),
                                                 0);
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any = *codec_->decode (oc_seq);
      //Don't store in a _var, since >>= returns a pointer to an
      //internal buffer and we are not supposed to free it.
      sc_qos_as_any >>= sc_qos_ptr;

      desired_priority = sc_qos_ptr->desired_priority;
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
                  "(%t|%T): Desired_Priority = %d, guid = %d in recvd service context\n",
                  desired_priority,
                  int_guid));
#endif

      FP_Scheduling::SegmentSchedulingParameter sched_param;
      sched_param.base_priority = desired_priority;
      sched_param_out = this->create_segment_scheduling_parameter (sched_param);
    }

  FP_Scheduler_Traits::QoSDescriptor_t qos;
  qos.priority_ = desired_priority;
  this->kokyu_dispatcher_->schedule (guid, qos);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): receive_request interceptor done\n"));
#endif

}

void
Fixed_Priority_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Fixed_Priority_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  RTCORBA::Priority desired_priority = 0;
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

  ACE_DEBUG ((LM_DEBUG, "in send_reply: before accessing current_->sched_param\n"));
  CORBA::Policy_ptr sched_policy =
    this->current_->scheduling_parameter();

  if (CORBA::is_nil (sched_policy))
  {
    ACE_DEBUG ((LM_DEBUG, "sched_policy nil. desired_priority not set in sched params\n"));
    desired_priority = 0;
  }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "sched_policy not nil. desired_priority set in sched params\n"));

      FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_policy =
        FP_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

      FP_Scheduling::SegmentSchedulingParameter sched_param =
        sched_param_policy->value ();

      desired_priority = sched_param.base_priority;

      //Fill the guid in the SC Qos struct
      sc_qos.guid.length (this->current_->id ()->length ());
      guid_copy (sc_qos.guid, *(this->current_->id ()));
       sc_qos.desired_priority = desired_priority;
      CORBA::Any sc_qos_as_any;
      sc_qos_as_any <<= sc_qos;

      sc.context_data = reinterpret_cast<CORBA::OctetSeq &> (*codec_->encode (sc_qos_as_any));

      // Add this context to the service context list.
      ri->add_reply_service_context (sc, 1);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG, "(%t|%T):reply sc added\n"));
#endif
    }

  kokyu_dispatcher_->update_schedule (*(this->current_->id ()),
                                      Kokyu::BLOCK);

#ifdef KOKYU_DSRT_LOGGING
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): send_reply interceptor done\n"));
#endif
}

void
Fixed_Priority_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  send_reply (ri);
}

void
Fixed_Priority_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  send_reply (ri);
}

void
Fixed_Priority_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  RTScheduling::Current::IdType guid;
  RTCORBA::Priority desired_priority=0;

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

  if (sc.ptr () == 0)
    {
      desired_priority = 0;
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

      desired_priority = sc_qos_ptr->desired_priority;
      guid.length (sc_qos_ptr->guid.length ());
      guid_copy (guid, sc_qos_ptr->guid);

#ifdef KOKYU_DSRT_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "(%t): Desired_Priority = %d in recvd service context\n",
                  desired_priority));
#endif
    }

  FP_Scheduler_Traits::QoSDescriptor_t qos;
  qos.priority_ = desired_priority;
  this->kokyu_dispatcher_->schedule (guid, qos);
}

void
Fixed_Priority_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  receive_reply (ri);
}

void
Fixed_Priority_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  receive_reply (ri);
}

void
Fixed_Priority_Scheduler::cancel (const RTScheduling::Current::IdType &)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::PolicyList*
Fixed_Priority_Scheduler::scheduling_policies (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Fixed_Priority_Scheduler::scheduling_policies (const CORBA::PolicyList &)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::PolicyList*
Fixed_Priority_Scheduler::poa_policies (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

char *
Fixed_Priority_Scheduler::scheduling_discipline_name (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

RTScheduling::ResourceManager_ptr
Fixed_Priority_Scheduler::create_resource_manager (const char *,
                                        CORBA::Policy_ptr)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Fixed_Priority_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         const char *,
                                         CORBA::Policy_ptr)
{
  throw CORBA::NO_IMPLEMENT ();
}

