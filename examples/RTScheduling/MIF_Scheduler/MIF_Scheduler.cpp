//$Id$

#include "MIF_Scheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "test.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> server_guid_counter;

DT::DT (TAO_SYNCH_MUTEX &lock,
        int guid)
  : dt_cond_ (lock),
    guid_ (guid),
    eligible_ (0)
{
}

void
DT::suspend (void)
{
  eligible_ = 0;
  while (!eligible_)
    this->dt_cond_.wait ();
}

void
DT::resume (void)
{
  eligible_ = 1;
  this->dt_cond_.signal ();
}

CORBA::Short
Segment_Sched_Param_Policy::importance (void)
{
  return this->importance_;
}

void
Segment_Sched_Param_Policy::importance (CORBA::Short importance)
{
  this->importance_ = importance;
}

CORBA::Policy_ptr
Segment_Sched_Param_Policy::copy (void)
{
  Segment_Sched_Param_Policy *copy = 0;
  ACE_NEW_THROW_EX (copy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY ());

  copy->importance (this->importance_);

  return copy;
}

CORBA::PolicyType
Segment_Sched_Param_Policy::policy_type (void)
{
  return 0;
}

void
Segment_Sched_Param_Policy::destroy (void)
{
}

MIF_Scheduler::MIF_Scheduler (CORBA::ORB_ptr orb)
  : wait_cond_ (lock_),
    wait_ (0)
{
  try
    {
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTScheduler_Current");

      this->current_ =
        RTScheduling::Current::_narrow (object.in ());

      object =
        orb->resolve_initial_references ("PriorityMappingManager");

      this->mapping_manager_ =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}

MIF_Scheduler::~MIF_Scheduler (void)
{
  while (free_que_.message_count () > 0)
    {
      DT *dt = 0;
      ACE_Message_Block *msg = 0;
      free_que_.dequeue_head (msg);
      dt = dynamic_cast<DT*> (msg);
      delete dt;
    }
}

void
MIF_Scheduler::incr_thr_count (void)
{
  lock_.acquire ();
  wait_++;
  lock_.release ();
}

void
MIF_Scheduler::wait (void)
{
  lock_.acquire ();
  while (wait_ > 0)
    wait_cond_.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "After Wait %d\n",
              wait_));

  lock_.release ();
}

void
MIF_Scheduler::resume_main (void)
{
  wait_--;
  wait_cond_.signal ();
}

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
MIF_Scheduler::create_segment_scheduling_parameter (CORBA::Short importance)
{
  MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr segment_policy;
  ACE_NEW_THROW_EX (segment_policy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
               TAO::VMCID,
                                       ENOMEM),
                                      CORBA::COMPLETED_NO));

  segment_policy->importance (importance);

  return segment_policy;
}


void
MIF_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
                                             const char *,
                                             CORBA::Policy_ptr sched_policy,
                                             CORBA::Policy_ptr)
{
  size_t count = 0;
  RTScheduling::Current::IdType_var guid = this->current_->id ();

  ACE_OS::memcpy (&count,
                  guid->get_buffer (),
                  guid->length ());


  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

  CORBA::Short desired_priority = sched_param->importance ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%t MIF_Scheduler::begin_scheduling_segment - Importance %d\n",
                desired_priority));


  if (desired_priority != 100)
    {
      //NOT Main Thread
      DT* new_dt = 0;
      ACE_NEW (new_dt,
               DT (this->lock_,
                   count));

      new_dt->msg_priority (desired_priority);
      lock_.acquire ();
      ready_que_.enqueue_prio (new_dt);
      resume_main ();
      new_dt->suspend ();
      lock_.release ();
    }
}

void
MIF_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &guid,
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
MIF_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
                                          const char* /*name*/,
                                          CORBA::Policy_ptr sched_policy,
                                          CORBA::Policy_ptr /*implicit_sched_param*/)
{
  size_t count = 0;
  RTScheduling::Current::IdType_var guid = this->current_->id ();

  ACE_OS::memcpy (&count,
                  guid->get_buffer (),
                  guid->length ());

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

  CORBA::Short desired_priority = sched_param->importance ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%t MIF_Scheduler::update_scheduling_segment - Importance %d\n",
                desired_priority));

  DT* new_dt = 0;
  ACE_NEW (new_dt,
           DT (this->lock_,
               count));

  new_dt->msg_priority (desired_priority);

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg = 0;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      if ((desired_priority == 100) || run_dt->msg_priority () >= (unsigned int)desired_priority)
        {
          ready_que_.enqueue_prio (new_dt);
          lock_.acquire ();
          run_dt->resume ();
          new_dt->suspend ();
          lock_.release ();
          free_que_.enqueue_prio (run_dt);
        }
      else
        {
          ready_que_.enqueue_prio (run_dt);
          delete new_dt;
        }
    }
  else delete new_dt;
}

void
MIF_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &guid,
                                       const char *)
{
  size_t count = 0;
  ACE_OS::memcpy (&count,
                  guid.get_buffer (),
                  guid.length ());

  ACE_DEBUG ((LM_DEBUG,
              "MIF_Scheduler::end_scheduling_segment %d\n",
                  count));

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt = 0;
      ACE_Message_Block* msg = 0;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
      free_que_.enqueue_prio (run_dt);
    }
}

void
MIF_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                         const char *,
                                                         CORBA::Policy_ptr)
{
}

void
MIF_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info)
{
  CORBA::Policy_var sched_param = current_->scheduling_parameter ();

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_var =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_param.in ());

  IOP::ServiceContext srv_con;
  srv_con.context_id = Client_Interceptor::SchedulingInfo;

  RTScheduling::Current::IdType_var guid = current_->id ();

  int guid_length = guid->length ();

  CORBA::OctetSeq seq_buf (guid_length);
  seq_buf.length (seq_buf.maximum ());
  ACE_OS::memcpy (seq_buf.get_buffer (),
                  guid->get_buffer (),
                  guid_length);

  int cxt_data_length = sizeof (int) + guid_length;
  srv_con.context_data.length (cxt_data_length);

  int i = 0;
  for (;i < guid_length;i++)
    {
      srv_con.context_data [i] = seq_buf [i];
    }

  int importance = sched_param_var->importance ();
  CORBA::OctetSeq int_buf (sizeof (importance));
  int_buf.length (int_buf.maximum ());
  ACE_OS::memcpy (int_buf.get_buffer (),
                  &importance,
                  sizeof (importance));

  int j = 0;
  for (;i < cxt_data_length;i++)
    {
      srv_con.context_data [i] = int_buf [j++];
    }

  request_info->add_request_service_context (srv_con,
                                             0);

  lock_.acquire ();
  if (ready_que_.message_count () > 0)
    {
      int priority;
      ACE_hthread_t current;
      ACE_Thread::self (current);
      if (ACE_Thread::getprio (current, priority) == -1)
        return;

      ACE_DEBUG ((LM_DEBUG,
                  "Initial thread priority is %d %d\n",
                  priority,
                  ACE_DEFAULT_THREAD_PRIORITY));

      RTCORBA::Priority rtpriority;
      RTCORBA::PriorityMapping* pm = this->mapping_manager_->mapping ();
      if (pm->to_CORBA(priority + 1, rtpriority))
        {
          current_->the_priority (rtpriority);

          ACE_Thread::self (current);
          if (ACE_Thread::getprio (current, priority) == -1)
            return;

          ACE_DEBUG ((LM_DEBUG,
                      "Bumped thread priority is %d\n",
                      priority));
        }

      DT* run_dt = 0;
      ACE_Message_Block* msg = 0;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      run_dt->resume ();
      free_que_.enqueue_prio (run_dt);
    }
  lock_.release ();

}

void
MIF_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out,
                                CORBA::Policy_out sched_param_out,
                                CORBA::Policy_out /*implicit_sched_param*/)
{

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MIF_Scheduler::receive_request\n"));

  IOP::ServiceContext_var serv_cxt =
    request_info->get_request_service_context (Server_Interceptor::SchedulingInfo);

  if (serv_cxt != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Got scheduling info\n"));

      RTScheduling::Current::IdType* guid;
      ACE_NEW (guid,
               RTScheduling::Current::IdType);

      guid->length (sizeof(size_t));
      ACE_OS::memcpy (guid->get_buffer (),
                      serv_cxt->context_data.get_buffer (),
                      sizeof (size_t));

      size_t gu_id;
      ACE_OS::memcpy (&gu_id,
                      guid->get_buffer (),
                      guid->length ());

      ACE_DEBUG ((LM_DEBUG,
                  "MIF_Scheduler::receive_request %d\n",
                  gu_id));


      CORBA::OctetSeq int_buf (sizeof (long));
      int_buf.length (int_buf.maximum ());
      int i = sizeof (long);
      for (unsigned int j = 0;j < sizeof (int);j++)
        {
          int_buf [j] = serv_cxt->context_data [i++];
        }

      int importance = 0;
      ACE_OS::memcpy (&importance,
                      int_buf.get_buffer (),
                      sizeof (importance));

      guid_out = guid;
      sched_param_out = DT_TEST::instance ()->scheduler ()->create_segment_scheduling_parameter (importance);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "%t The Guid is %d Importance is %d\n",
                    gu_id,
                    importance));

      DT* new_dt;
      ACE_NEW (new_dt,
               DT (this->lock_,
                   gu_id));

      new_dt->msg_priority (importance);
      lock_.acquire ();
      ready_que_.enqueue_prio (new_dt);
      new_dt->suspend ();
      lock_.release ();
    }
}

void
MIF_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr)
{

  RTScheduling::Current::IdType_var guid = current_->id ();

  size_t count;
  ACE_OS::memcpy (&count,
                  guid->get_buffer (),
                  guid->length ());

  ACE_DEBUG ((LM_DEBUG,
              "MIF_Scheduler::send_reply %d\n",
              count));

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt = 0;
      ACE_Message_Block* msg = 0;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
      free_que_.enqueue_prio (run_dt);
    }
}

void
MIF_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr)
{
  if (ready_que_.message_count () > 0)
    {
      DT* run_dt = 0;
      ACE_Message_Block* msg = 0;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
      free_que_.enqueue_prio (run_dt);
    }
}

void
MIF_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr)
{
  if (TAO_debug_level > 0)
    {
      RTScheduling::Current::IdType_var guid = current_->id ();

      size_t count;
      ACE_OS::memcpy (&count,
                      guid->get_buffer (),
                      guid->length ());


      ACE_DEBUG ((LM_DEBUG,
                  "MIF_Scheduler::send_other %d\n",
                  count));
    }

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = dynamic_cast<DT*> (msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
      free_que_.enqueue_prio (run_dt);
    }
}

void
MIF_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
MIF_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr)
{
  CORBA::Policy_var sched_param = current_->scheduling_parameter ();

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_var =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_param.in ());

  int importance = sched_param_var->importance ();

  RTScheduling::Current::IdType_var guid = current_->id ();

  size_t gu_id;
  ACE_OS::memcpy (&gu_id,
                  guid->get_buffer (),
                  guid->length ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MIF_Scheduler::receive_reply Guid = %d Imp = %d\n",
                gu_id,
                importance));


  DT* new_dt;
  ACE_NEW (new_dt,
           DT (this->lock_,
               gu_id));

  new_dt->msg_priority (importance);

  lock_.acquire ();
  ready_que_.enqueue_prio (new_dt);
  int priority;
  ACE_hthread_t current;
  ACE_Thread::self (current);
  if (ACE_Thread::getprio (current, priority) == -1)
    return;

  RTCORBA::Priority rtpriority;
  RTCORBA::PriorityMapping* pm = this->mapping_manager_->mapping ();
  if (pm->to_CORBA(priority - 1, rtpriority))
    {
      current_->the_priority (rtpriority);
    }

  new_dt->suspend ();
  lock_.release ();
}

void
MIF_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr)
{
  CORBA::Policy_var sched_param = current_->scheduling_parameter ();

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_var =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_param.in ());

  int importance = sched_param_var->importance ();

  RTScheduling::Current::IdType_var guid = current_->id ();

  size_t gu_id;
  ACE_OS::memcpy (&gu_id,
                  guid->get_buffer (),
                  guid->length ());

  DT* new_dt;
  ACE_NEW (new_dt,
           DT (this->lock_,
               gu_id));

  new_dt->msg_priority (importance);

  lock_.acquire ();
  ready_que_.enqueue_prio (new_dt);

  int priority;
  ACE_hthread_t current;
  ACE_Thread::self (current);
  if (ACE_Thread::getprio (current, priority) == -1)
    return;

  RTCORBA::Priority rtpriority;
  RTCORBA::PriorityMapping* pm = this->mapping_manager_->mapping ();
  if (pm->to_CORBA(priority - 1, rtpriority))
    {
      current_->the_priority (rtpriority);
    }

  new_dt->suspend ();
  lock_.release ();
}

void
MIF_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr)
{
  CORBA::Policy_var sched_param = current_->scheduling_parameter ();

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_var =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_param.in ());

  int importance = sched_param_var->importance ();

  RTScheduling::Current::IdType_var guid = current_->id ();

  size_t gu_id;
  ACE_OS::memcpy (&gu_id,
                  guid->get_buffer (),
                  guid->length ());

  DT* new_dt = 0;
  ACE_NEW (new_dt,
           DT (this->lock_,
               gu_id));

  new_dt->msg_priority (importance);

  lock_.acquire ();
  ready_que_.enqueue_prio (new_dt);

  int priority;
  ACE_hthread_t current;
  ACE_Thread::self (current);
  if (ACE_Thread::getprio (current, priority) == -1)
    return;

  RTCORBA::Priority rtpriority;
  RTCORBA::PriorityMapping* pm = this->mapping_manager_->mapping ();
  if (pm->to_CORBA(priority - 1, rtpriority))
    {
      current_->the_priority (rtpriority);
    }

  new_dt->suspend ();
  lock_.release ();
}

void
MIF_Scheduler::cancel (const RTScheduling::Current::IdType &)
{
}

CORBA::PolicyList*
MIF_Scheduler::scheduling_policies (void)
{
  return 0;
}

void
MIF_Scheduler::scheduling_policies (const CORBA::PolicyList &)
{
}

CORBA::PolicyList*
MIF_Scheduler::poa_policies (void)
{
        return 0;
}

char *
MIF_Scheduler::scheduling_discipline_name (void)
{
        return 0;
}

RTScheduling::ResourceManager_ptr
MIF_Scheduler::create_resource_manager (const char *,
                                        CORBA::Policy_ptr)
{
        return 0;
}

void
MIF_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         const char *,
                                         CORBA::Policy_ptr)
{
}
