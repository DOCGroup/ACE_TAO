//$Id$

#include "test.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"
#include "ace/Arg_Shifter.h"
//#include "FP_DT_Creator.h"
#include "../Thread_Task.h"
#include "../Task_Stats.h"
#include "../Synch_i.h"

DT_Test::DT_Test (void)
{
  base_t = ACE_OS::gethrtime ();
}	

void
DT_Test::check_supported_priorities (void)
{
  // Check that we have sufficient priority range to run this
  // test, i.e., more than 1 priority level.

  this->thr_sched_policy_ = orb_->orb_core ()->orb_params ()->sched_policy ();
  this->thr_scope_policy_ = orb_->orb_core ()->orb_params ()->scope_policy ();

  if (thr_sched_policy_ == THR_SCHED_RR)
    {
      //if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_RR\n"));

      sched_policy_ = ACE_SCHED_RR;
    }
  else 
  if (thr_sched_policy_ == THR_SCHED_FIFO)
    {
      // if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_FIFO\n"));

      sched_policy_ = ACE_SCHED_FIFO;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_OTHER\n"));
      
      sched_policy_ = ACE_SCHED_OTHER;
    }

  if (thr_sched_policy_ == THR_SCHED_RR || thr_sched_policy_ == THR_SCHED_FIFO)
    {
      max_priority_ = ACE_Sched_Params::priority_max (sched_policy_);
      min_priority_ = ACE_Sched_Params::priority_min (sched_policy_);
      
      if (max_priority_ == min_priority_)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Not enough priority levels on this platform"
		      " to run the test, aborting \n"));
	  ACE_OS::exit (2);
	}
      else ACE_DEBUG ((LM_DEBUG, "max_priority = %d, min_priority = %d\n",
		       max_priority_, min_priority_));
    }
}

int
DT_Test::init (int argc, char *argv []
	       ACE_ENV_ARG_DECL)
{
  orb_ = CORBA::ORB_init (argc,
			  argv,
			  ""
			  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->check_supported_priorities ();

  dt_creator_->orb (orb_.in ());
  
  TASK_STATS::instance ()->init (dt_creator_->total_load ());

  CORBA::Object_ptr manager_obj = orb_->resolve_initial_references ("RTSchedulerManager"
								   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
      
  TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
      
  
  ACE_NEW_RETURN (scheduler_,
		  Fixed_Priority_Scheduler (orb_.in ()), -1);
  
  manager->rtscheduler (scheduler_);
  
  CORBA::Object_var object =
    orb_->resolve_initial_references ("RTScheduler_Current" 
				      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  current_  =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  
  if (sched_policy_ != ACE_SCHED_OTHER)
    {

        //Set the main thread to max priority...
        if (ACE_OS::sched_params (ACE_Sched_Params (sched_policy_,
  						  55,
  						  ACE_SCOPE_PROCESS)) != 0)
  	{
  	  if (ACE_OS::last_error () == EPERM)
  	    {
  	      ACE_DEBUG ((LM_DEBUG,
  			  "(%P|%t): user is not superuser, "
  			  "test runs in time-shared class\n"));
  	    }
  	  else
  	    ACE_ERROR ((LM_ERROR,
  			"(%P|%t): sched_params failed\n"));
  	}
    }
  
  return 0;
}

void
DT_Test::run (int argc, char* argv [] 
	      ACE_ENV_ARG_DECL)
{
  init (argc,argv
	ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->dt_creator_->resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER) == -1)
    return;
  ACE_CHECK;

  
  //TASK_STATS::instance ()->init (this->dt_creator_->dt_count () * 100);
  
  
  this->dt_creator_->activate_root_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  
  this->dt_creator_->activate_poa_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->dt_creator_->activate_job_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->dt_creator_->activate_schedule (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  DT_Creator* dt_creator = this->dt_creator_;
  dt_creator->register_synch_obj (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  
  ACE_DEBUG ((LM_DEBUG,
	      "Registered Synch Object\n"));
  
  /*
  dt_creator_->create_distributable_threads (current_.in ()
					     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  */
  
  this->activate_task ();
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "ORB RUN\n");
  dt_creator_->log_msg (msg);

  //ACE_Thread_Manager::instance ()->wait ();
  orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


}


void
DT_Test::dt_creator (FP_DT_Creator* dt_creator)
{
  this->dt_creator_ = dt_creator;
}

FP_DT_Creator*
DT_Test::dt_creator (void)
{
  return this->dt_creator_;
}


Fixed_Priority_Scheduler* 
DT_Test::scheduler (void)
{
  return this->scheduler_;
}

int
DT_Test::activate_task (void)
{

  ACE_DEBUG ((LM_DEBUG,
	      "Test Activate Task\n"));

  long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |= 
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();
  
  if (this->activate (flags,
		      1,
		      0,
		      50) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
	ACE_ERROR_RETURN ((LM_ERROR,
			   ACE_TEXT ("Insufficient privilege to run this test.\n")),
			  -1);

    }
  return 0;
}

int
DT_Test::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
		  "In test::svc\n"));
      
      dt_creator_->create_distributable_threads (current_.in ()
						 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY; 

  return 0;
}


CORBA::ORB_ptr
DT_Test::orb (void)
{
  return this->orb_.in ();
}

int
main (int argc, char* argv [])
{
  ACE_TRY_NEW_ENV
    {
      ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_FP_DT_Creator);
      
      DT_TEST::instance ()->run (argc, argv
				 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY; 
  
  return 0;
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<DT_Test, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<DT_Test, ACE_Null_Mutex>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
