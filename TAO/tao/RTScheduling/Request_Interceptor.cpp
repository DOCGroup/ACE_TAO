//$Id$

#include "Request_Interceptor.h"
#include "Current.h"
#include "Distributable_Thread.h"
#include "tao/ORB_Core.h"

// Client_Interceptor::Client_Interceptor (RTScheduling::Current_ptr current)
//   : current_ (RTScheduling::Current::_duplicate (current))
// {
// } 

ACE_Atomic_Op<ACE_Thread_Mutex, long> server_guid_counter;

void 
Client_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri
				  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::send_request\n"));

  // Temporary current.
  TAO_RTScheduler_Current_i *new_current = 0;
  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  
  if (current == 0)
    ACE_DEBUG ((LM_DEBUG,
		"No Scheduling Segment Context\n"));
  else 
    {
      // If this is a one way request
      if (!ri->response_expected ()) 
	{
	  
	  // Generate GUID.
	  RTScheduling::Current::IdType guid;
	  guid.length (sizeof(long));
	  
	  long temp = ++guid_counter;
	  ACE_OS::memcpy (guid.get_buffer (),
			  &temp,
			  sizeof(long));
	  
	  int id;
	  ACE_OS::memcpy (&id,
			  guid.get_buffer (),
			  guid.length ());
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "The Guid is %d %d\n",
		      id,
		      guid_counter.value_i ()));
      
	  // Create new DT.
	  RTScheduling::DistributableThread_var dt = TAO_DistributableThread_Factory::create_DT ();
      
 

	  // Add new DT to map.
	  int result = current->dt_hash ()->bind (guid, dt);
	  if (result != 0)
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "No Scheduling Segment Context\n"));
	      
	    }
	  
	  // Create new temporary current. Note that
	  // the new <sched_param> is the current
	  // <implicit_sched_param> and there is no
	  // segment name.
	  ACE_NEW (new_current,
		   TAO_RTScheduler_Current_i (current->orb (),
					      current->dt_hash (),
					      guid,
					      0,
					      current->implicit_scheduling_parameter (),
					      0,
					      dt.in (),
					      current));
      
      
	  // Install new current in the ORB.
	  //current->implementation (new_current);
	  tss->rtscheduler_current_impl_ = new_current;

	}
  
      // Scheduler populates the service context with
      // scheduling parameters.
      current->scheduler ()->send_request (ri);
  
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
Client_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr
			       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::send_poll\n"));
    
}

void 
Client_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
				   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::receive_reply\n"));

  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  if (current != 0)
    current->scheduler ()->receive_reply (ri);
}
  
void 
Client_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri
				       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::receive_exception\n"));

  CORBA::Any_var ex = ri->received_exception ();
  CORBA::TypeCode_var type = ex->type ();
  const char * id = type->id ();

  ACE_DEBUG ((LM_DEBUG,
	      "Received Exception %s\n",
	      id));
  
  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  
  if (current != 0)
    {
      // If the remote host threw a THREAD_CANCELLED
      // exception, make sure to take the appropriate
      // local action.
      if (ACE_OS_String::strstr (id, "CORBA::THREAD_CANCELLED") == 0)
	{
	  // Perform the necessary cleanup as the
	  // thread was cancelled.
	  current->cancel_thread ();
	}
      else
	{
	  // Inform scheduler that exception was
	  // received.
	  current->scheduler ()->receive_exception (ri);
	}
    }
}
  
void 
Client_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri
				   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Client_Interceptor::receive_other\n"));

  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  if (current != 0)
    current->scheduler ()->receive_other (ri);

}

char* 
Client_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return  CORBA::string_dup ("RTSchdeuler_Client_Interceptor");
}

void 
Client_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Server_Interceptor::Server_Interceptor (TAO_RTScheduler_Current_ptr current)
{
	this->current_ = TAO_RTScheduler_Current::_duplicate (current);

} 

void 
Server_Interceptor::receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr
						      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Server_Interceptor::receive_request_service_contexts\n"));

}
  
void 
Server_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
				     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, 
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Server_Interceptor::receive_request\n"));

  RTScheduling::Current::IdType guid;
  char* name = 0;
  CORBA::Policy_ptr sched_param = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  
  TAO_RTScheduler_Current_i* new_current = this->current_->implementation ();

  if (new_current != 0)
    {

      // Scheduler retrieves scheduling parameters
      // from request and populates the out
      // parameters.
      new_current->scheduler()->receive_request(ri,
						guid,
						name,
						sched_param,
						implicit_sched_param);
      //***************************************
      // Generate GUID.
      guid.length (sizeof(long));
  
      long temp = ++server_guid_counter;
      ACE_OS::memcpy (guid.get_buffer (),
		      &temp,
		      sizeof(long));
      
      int id;
      ACE_OS::memcpy (&id,
		      guid.get_buffer (),
		      guid.length ());
      
      ACE_DEBUG ((LM_DEBUG,
		  "The Guid is %d %d\n",
		  id,
		  server_guid_counter.value_i ()));
      //***************************************
      
      // Create new DT.
      RTScheduling::DistributableThread_var dt = TAO_DistributableThread_Factory::create_DT ();
      
      // Add new DT to map.
      new_current->dt_hash ()->bind (guid, dt.in ());
  
      // Create new temporary current. Note that
      // the new <sched_param> is the current
      // <implicit_sched_param> and there is no
      // segment name.
      new_current->id (guid);
      new_current->name (name);
      new_current->scheduling_parameter (sched_param);
      new_current->implicit_scheduling_parameter (implicit_sched_param);
      new_current->DT (dt.in ());
	  
      // Install new current in the ORB.
      //current->implementation (new_current);
      this->current_->implementation (new_current);
    }
}

void 
Server_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Server_Interceptor::send_reply\n"));

  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      if (current->DT ()->state () == RTScheduling::DistributableThread::CANCELLED)
	{
	  current->cancel_thread (ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;
	}

      // Inform scheduler that upcall is complete.
      current->scheduler ()->send_reply (ri);

      current->cleanup_DT ();
      current->cleanup_current ();
    }
}
  
void 
Server_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
				    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Server_Interceptor::send_exception\n"));

  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      // Inform scheduler that upcall is complete.
      current->scheduler ()->send_other (ri);
      
      current->cleanup_DT ();
      current->cleanup_current ();
    }
}
  
void 
Server_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr ri
				ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Server_Interceptor::send_other\n"));

  TAO_RTScheduler_Current_i *current = 0;
  
  TAO_TSS_Resources *tss = TAO_TSS_RESOURCES::instance ();
  
  current = ACE_static_cast (TAO_RTScheduler_Current_i *,
				  tss->rtscheduler_current_impl_);
  if (current != 0)
    {
      // Inform scheduler that upcall is complete.
      current->scheduler ()->send_other (ri);
      
      current->cleanup_DT ();
      current->cleanup_current ();
    }
}

char* 
Server_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    return  CORBA::string_dup ("RTSchdeuler_Server_Interceptor");
}

void 
Server_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}
