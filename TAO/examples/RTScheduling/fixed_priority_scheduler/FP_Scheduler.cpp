//$Id$

#include "FP_Scheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "test.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

RTCORBA::Priority 
Segment_Sched_Param_Policy::value (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

void 
Segment_Sched_Param_Policy::value (RTCORBA::Priority value)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->value_ = value;
}

Fixed_Priority_Scheduler::Fixed_Priority_Scheduler (CORBA::ORB_ptr orb)
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTScheduler_Current" 
				     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  
  this->current_ =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  
}

Fixed_Priority_Scheduler::~Fixed_Priority_Scheduler (void)
{
}

FP_Scheduling::SegmentSchedulingParameterPolicy_ptr 
Fixed_Priority_Scheduler::create_segment_scheduling_parameter (RTCORBA::Priority segment_priority)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr segment_policy;
  ACE_NEW_THROW_EX (segment_policy,
                    Segment_Sched_Param_Policy,
                    CORBA::NO_MEMORY (
				      CORBA::SystemException::_tao_minor_code (
				       TAO_DEFAULT_MINOR_CODE,
				       ENOMEM),
				      CORBA::COMPLETED_NO));

  segment_policy->value (segment_priority);

  return segment_policy;

}

  
void 
Fixed_Priority_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &,
							const char *,
							CORBA::Policy_ptr /*sched_policy*/,
							CORBA::Policy_ptr
							ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
//    FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param = 
//      FP_Scheduling::SegmentSchedulingParameterPolicy::_narrow (sched_policy);

//    RTCORBA::Priority desired_priority = sched_param->value ();

//    if (desired_priority != 100)
//      {
//        this->current_->the_priority (desired_priority
//  				    ACE_ENV_ARG_PARAMETER);
//        ACE_CHECK;

//        CORBA::Short priority = 
//  	this->current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
//        ACE_CHECK;
      
//        if (desired_priority != priority)
//  	{
//  	  ACE_ERROR ((LM_ERROR,
//  		      "ERROR: Unable to set thread "
//  		      "priority to %d\n", desired_priority));
//  	  ACE_THROW ((CORBA::INTERNAL ()));
//  	}
//      }
}

void 
Fixed_Priority_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &guid,
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
Fixed_Priority_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &guid,
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
Fixed_Priority_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &,
				       const char *
				       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
Fixed_Priority_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
							 const char *,
							 CORBA::Policy_ptr
							 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  
}
  

void 
Fixed_Priority_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info
					ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{

  IOP::ServiceContext* srv_con = new IOP::ServiceContext;
  srv_con->context_id = Client_Interceptor::SchedulingInfo;
  srv_con->context_data.length (sizeof (long));
  ACE_OS::memcpy (srv_con->context_data.get_buffer (),
		  current_->id (ACE_ENV_ARG_PARAMETER)->get_buffer (),
		  sizeof (long));
  ACE_CHECK;
  request_info->add_request_service_context (*srv_con,
					     0
					     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}

void 
Fixed_Priority_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
					   RTScheduling::Current::IdType_out guid_out,
					   CORBA::String_out /*name*/,
					   CORBA::Policy_out /*sched_param*/,
					   CORBA::Policy_out /*implicit_sched_param*/
					   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{

  //***************************************
  /*
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
   */
    //***************************************

  IOP::ServiceContext* serv_cxt = 
    request_info->get_request_service_context (Server_Interceptor::SchedulingInfo
					       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  if (serv_cxt != 0)
    {
      int gu_id;
      ACE_OS::memcpy (&gu_id,
		      serv_cxt->context_data.get_buffer (),
		      serv_cxt->context_data.length ());
      
      char msg [BUFSIZ];
      ACE_OS::sprintf (msg,"The Guid is %d\n", gu_id);
      DT_TEST::instance ()->dt_creator ()->log_msg (msg);
      
      RTScheduling::Current::IdType* guid;
      ACE_NEW (guid,
	       RTScheduling::Current::IdType);
      
      
      // Generate GUID.
      guid->length (sizeof(long));
      
      ACE_OS::memcpy (guid->get_buffer (),
		      &gu_id,
		      sizeof(long));
      
      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "Receive request The Guid is %d\n",
		    gu_id));
      
      guid_out.ptr () = guid;
    }

}

void 
Fixed_Priority_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void 
Fixed_Priority_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
			       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
Fixed_Priority_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr
			   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
Fixed_Priority_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
    
void 
Fixed_Priority_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
				  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}

void 
Fixed_Priority_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr
			      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
}
    
void 
Fixed_Priority_Scheduler::cancel (const RTScheduling::Current::IdType &
		       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList* 
Fixed_Priority_Scheduler::scheduling_policies (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
void 
Fixed_Priority_Scheduler::scheduling_policies (const CORBA::PolicyList &
				    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
      
CORBA::PolicyList* 
Fixed_Priority_Scheduler::poa_policies (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
char * 
Fixed_Priority_Scheduler::scheduling_discipline_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
RTScheduling::ResourceManager_ptr 
Fixed_Priority_Scheduler::create_resource_manager (const char *,
					CORBA::Policy_ptr
					ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
	return 0;
}
    
void 
Fixed_Priority_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
					 const char *,
					 CORBA::Policy_ptr
					 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
