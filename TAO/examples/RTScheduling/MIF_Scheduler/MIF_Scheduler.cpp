//$Id$

#include "MIF_Scheduler.h"
#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

DT::DT (ACE_Thread_Mutex *lock,
	int guid)
  : dt_cond_ (*lock),
    guid_ (guid)
{
}

void
DT::suspend (void)
{
  this->dt_cond_.wait ();
}

void
DT::resume (void)
{
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
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTScheduler_Current" 
				     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  
  this->current_ =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  
}

MIF_Scheduler::~MIF_Scheduler (void)
{
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
MIF_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &guid,
					     const char *,
					     CORBA::Policy_ptr sched_policy,
					     CORBA::Policy_ptr
					     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
  MIF_Scheduling::SegmentSchedulingParameterPolicy_var sched_param = 
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);
  
  CORBA::Short desired_priority = sched_param->importance ();

  int count;
  ACE_OS::memcpy (&count,
		  this->current_->id ()->get_buffer (),
		  this->current_->id ()->length ());
  
  DT* new_dt;
  ACE_NEW (new_dt,
	   DT (&this->lock_,
	       count));
  
  new_dt->msg_priority (desired_priority);
  
  if (count != 1)
    {
      //NOT Main Thread
      ready_que_.enqueue_prio (new_dt);
      DT* main_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      main_dt = ACE_dynamic_cast (DT*, msg);
      main_dt->resume ();
      new_dt->suspend ();
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
MIF_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &guid,
					  const char *name,
					  CORBA::Policy_ptr sched_policy,
					  CORBA::Policy_ptr implicit_sched_param
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

  DT* new_dt;
  ACE_NEW (new_dt,
	   DT (&this->lock_,
	       count));
  
  new_dt->msg_priority (desired_priority);
  
  if (count == 1)
    {
      wait_que_.enqueue_prio (new_dt);
      DT* run_dt;
      ACE_Message_Block* msg;
      ready_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      run_dt->resume ();
      new_dt->suspend ();
    }
  else
    {
      ready_que_.enqueue (new_dt);
      DT* run_dt;
      ACE_Message_Block* msg;
      wait_que_.dequeue_head (msg);
      run_dt = ACE_dynamic_cast (DT*, msg);
      run_dt->resume ();
      new_dt->suspend ();
    }
  //    this->begin_new_scheduling_segment (guid,
  //  				      name,
  //  				      sched_param,
  //  				      implicit_sched_param
  //  				      ACE_ENV_ARG_PARAMETER);
  //    ACE_CHECK;
}
    
void 
MIF_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &,
				       const char *
				       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
MIF_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr
			     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
MIF_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr,
				RTScheduling::Current::IdType_out guid_out,
				CORBA::String_out name,
				CORBA::Policy_out sched_param,
				CORBA::Policy_out implicit_sched_param
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  //***************************************
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
  
 //  ACE_DEBUG ((LM_DEBUG,
// 	      "The Guid is %d %d\n",
// 	      id,
// 	      server_guid_counter.value_i ()));

  guid_out.ptr () = guid;
  //***************************************
  
  
}

void 
MIF_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
MIF_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
			       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
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
