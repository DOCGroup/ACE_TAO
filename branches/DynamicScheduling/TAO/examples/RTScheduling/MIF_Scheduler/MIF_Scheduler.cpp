//$Id$

#include "MIF_Scheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "test.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

DT::DT (ACE_Thread_Mutex &lock,
	int guid)
  : dt_cond_ (lock),
    guid_ (guid),
    eligible_ (0),
    mutex_ (lock)
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->importance_;
}

void
Segment_Sched_Param_Policy::importance (CORBA::Short importance)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->importance_ = importance;
}

MIF_Scheduler::MIF_Scheduler (CORBA::ORB_ptr orb)
  : wait_cond_ (lock_),
    wait_ (0)
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTScheduler_Current"
				     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->current_ =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

//  state_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;

  wait_ = 0;
}

MIF_Scheduler::~MIF_Scheduler (void)
{
}

void
MIF_Scheduler::incr_thr_count (void)
{
  lock_.acquire ();
  wait_++;
  ACE_DEBUG ((LM_DEBUG,
	      "Incr Thread Count  %d\n",
	      wait_));
  lock_.release ();
}

void
MIF_Scheduler::wait (void)
{
  lock_.acquire ();

  ACE_DEBUG ((LM_DEBUG,
	      "Before Wait %d\n",
	      wait_));

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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr segment_policy;
  ACE_NEW_THROW_EX (segment_policy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
				      CORBA::SystemException::_tao_minor_code (
				       TAO_DEFAULT_MINOR_CODE,
				       ENOMEM),
				      CORBA::COMPLETED_NO));

  segment_policy->importance (importance);

  return segment_policy;

}


void
MIF_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
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
      DT* new_dt;
      ACE_NEW (new_dt,
	       DT (this->lock_,
		   count));

      new_dt->msg_priority (desired_priority);
      lock_.acquire ();
      /*
      if (wait_que_.message_count () > 0)
	{
	  DT* main_dt;
	  ACE_Message_Block* msg;
	  wait_que_.dequeue_head (msg);
	  main_dt = ACE_dynamic_cast (DT*, msg);
	  main_dt->resume ();
	}
      */
      /*
      if (ready_que_.message_count () > 0)
	{
	  DT* main_dt;
	  ACE_Message_Block* msg;
	  ready_que_.dequeue_head (msg);
	  main_dt = ACE_dynamic_cast (DT*, msg);
	  if (main_dt->msg_priority () >= new_dt->msg_priority ())
	    {
	      main_dt->resume ();
	      ready_que_.enqueue_prio (new_dt);
	      new_dt->suspend ();
	    }
	  else
	    {
	      ready_que_.enqueue_prio (main_dt);
	      delete new_dt;
	    }
	}
      */
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
MIF_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &/*guid*/,
					  const char* /*name*/,
					  CORBA::Policy_ptr sched_policy,
					  CORBA::Policy_ptr /*implicit_sched_param*/
					  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  int count;
  ACE_OS::memcpy (&count,
		  this->current_->id ()->get_buffer (),
		  this->current_->id ()->length ());

  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

  CORBA::Short desired_priority = sched_param->importance ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"%t MIF_Scheduler::update_scheduling_segment - Importance %d\n",
		desired_priority));

  DT* new_dt;
  ACE_NEW (new_dt,
	   DT (this->lock_,
	       count));

  new_dt->msg_priority (desired_priority);

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      //wait_que_.enqueue_prio (new_dt);
      if ((desired_priority == 100) || run_dt->msg_priority () >= (unsigned int)desired_priority)
	{
	  ready_que_.enqueue_prio (new_dt);
	  lock_.acquire ();
	  run_dt->resume ();
	  new_dt->suspend ();
	  lock_.release ();
	}
      else
	{
	  ready_que_.enqueue_prio (run_dt);
	  delete new_dt;
	}
    }
  else delete new_dt;
  //}
      /*
  else
    {
      if (wait_que_.message_count () > 0)
	{
 	  DT* run_dt;
	  ACE_Message_Block* msg;
	  wait_que_.dequeue_head (msg);
	  run_dt = ACE_dynamic_cast (DT*, msg);
	  ready_que_.enqueue_prio (new_dt);
	  lock_.acquire ();
	  run_dt->resume ();
	  new_dt->suspend ();
	  lock_.release ();
	}
      else
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "It reached here\n"));

	  if (ready_que_.message_count() > 0)
	    {
	      DT* run_dt;
	      ACE_Message_Block* msg;
	      ready_que_.dequeue_head (msg);
	      run_dt = ACE_dynamic_cast (DT*, msg);
	      if (run_dt->msg_priority () > new_dt->msg_priority ())
		{
		  lock_.acquire ();
		  run_dt->resume ();
		  ready_que_.enqueue_prio (new_dt);
		  new_dt->suspend ();
		  lock_.release ();
		}
	      else
		{
		  ready_que_.enqueue_prio (run_dt);
		}
	    }
	}
    }
      */
}

void
MIF_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &guid,
				       const char *
				       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int count;
  ACE_OS::memcpy (&count,
		  guid.get_buffer (),
		  guid.length ());

  ACE_DEBUG ((LM_DEBUG,
	      "MIF_Scheduler::end_scheduling_segment %d\n",
		  count));

  /*
  if (wait_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }
  */
  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }
}

void
MIF_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
							 const char *,
							 CORBA::Policy_ptr
							 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}


void
MIF_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info
			     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param_var =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (current_->scheduling_parameter (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_CHECK;

  IOP::ServiceContext* srv_con = new IOP::ServiceContext;
  srv_con->context_id = Client_Interceptor::SchedulingInfo;

  int guid_length = current_->id (ACE_ENV_ARG_PARAMETER)->length ();
  ACE_CHECK;

  RTScheduling::Current::IdType* guid = current_->id (ACE_ENV_ARG_PARAMETER);

  CORBA::Octet *seq_buf = CORBA::OctetSeq::allocbuf (sizeof (guid_length));
  ACE_OS::memcpy (seq_buf,
		  guid->get_buffer (),
		  guid_length);

  int cxt_data_length = sizeof (int) + guid_length;
  srv_con->context_data.length (cxt_data_length);

  int i = 0;
  for (;i < guid_length;i++)
    {
      srv_con->context_data [i] = seq_buf [i];
    }

  int importance = sched_param_var->importance ();
  CORBA::Octet *int_buf = CORBA::OctetSeq::allocbuf (sizeof (int));
  ACE_OS::memcpy (int_buf,
		  &importance,
		  sizeof (int));

  int j = 0;
  for (;i < cxt_data_length;i++)
    {
      srv_con->context_data [i] = int_buf [j++];
    }

  request_info->add_request_service_context (*srv_con,
					     0
					     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
MIF_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
				RTScheduling::Current::IdType_out guid_out,
				CORBA::String_out,
				CORBA::Policy_out sched_param_out,
				CORBA::Policy_out /*implicit_sched_param*/
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{

  IOP::ServiceContext* serv_cxt =
    request_info->get_request_service_context (Server_Interceptor::SchedulingInfo);

  RTScheduling::Current::IdType* guid;
  ACE_NEW (guid,
	   RTScheduling::Current::IdType);

  guid->length (sizeof(long));
  ACE_OS::memcpy (guid->get_buffer (),
		  serv_cxt->context_data.get_buffer (),
		  sizeof (long));

  int gu_id;
  ACE_OS::memcpy (&gu_id,
		  guid->get_buffer (),
		  guid->length ());

  ACE_DEBUG ((LM_DEBUG,
	      "MIF_Scheduler::receive_request %d\n",
		   gu_id));


  CORBA::Octet *int_buf = CORBA::OctetSeq::allocbuf (sizeof (long));
  int i = sizeof (long);
  for (unsigned int j = 0;j < sizeof (int);j++)
    {
      int_buf [j] = serv_cxt->context_data [i++];
    }

  int importance;
  ACE_OS::memcpy (&importance,
		  int_buf,
		  sizeof (int));

  guid_out.ptr () = guid;
  sched_param_out.ptr () = DT_TEST::instance ()->scheduler ()->create_segment_scheduling_parameter (importance);

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

  /*
  lock_.acquire ();
  if (wait_que_.message_count () > 0)
    {
      DT* main_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      main_dt = ACE_dynamic_cast (DT*, msg);
      main_dt->resume ();
    }
  ready_que_.enqueue_prio (new_dt);
  //resume_main ();
  new_dt->suspend ();
  lock_.release ();
  */

  lock_.acquire ();
  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      if (run_dt->msg_priority () == 100)
	  run_dt->resume ();
      else ready_que_.enqueue_prio (run_dt);
    }
  ready_que_.enqueue_prio (new_dt);
  new_dt->suspend ();
  //resume_main ();
  lock_.release ();
}

void
MIF_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

  RTScheduling::Current::IdType* guid = current_->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  int count;
  ACE_OS::memcpy (&count,
		  guid->get_buffer (),
		  guid->length ());


  ACE_DEBUG ((LM_DEBUG,
	      "MIF_Scheduler::send_reply %d\n",
		   count));

  /*
  if (wait_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }
  */

  if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }
}

void
MIF_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
			       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  if (wait_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }
  else if (ready_que_.message_count () > 0)
    {
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      lock_.acquire ();
      run_dt->resume ();
      lock_.release ();
    }

}

void
MIF_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void
MIF_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
MIF_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
				  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void
MIF_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void
MIF_Scheduler::cancel (const RTScheduling::Current::IdType &
		       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList*
MIF_Scheduler::scheduling_policies (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}

void
MIF_Scheduler::scheduling_policies (const CORBA::PolicyList &
				    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList*
MIF_Scheduler::poa_policies (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}

char *
MIF_Scheduler::scheduling_discipline_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}

RTScheduling::ResourceManager_ptr
MIF_Scheduler::create_resource_manager (const char *,
					CORBA::Policy_ptr
					ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}

void
MIF_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
					 const char *,
					 CORBA::Policy_ptr
					 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
