//$Id$

#include "test.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"
#include "ace/Arg_Shifter.h"
//#include "MIF_DT_Creator.h"
#include "../Thread_Task.h"
#include "../Task_Stats.h"


DT_Test::DT_Test (void)
{
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
  
  CORBA::Object_ptr manager_obj = orb_->resolve_initial_references ("RTSchedulerManager"
								   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
      
  TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
      
  
  ACE_NEW_RETURN (scheduler_,
		  MIF_Scheduler (orb_.in ()), -1);
  
  manager->rtscheduler (scheduler_);
  
  CORBA::Object_var object =
    orb_->resolve_initial_references ("RTScheduler_Current" 
				      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  current_  =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  printf ("DT_test::init done\n");
  return 0;
}

void
DT_Test::run (int argc, char* argv [] 
			  ACE_ENV_ARG_DECL)
{
  init (argc,argv
	ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //TASK_STATS::instance ()->init (this->dt_creator_->dt_count () * 100);
  
  this->activate_task ();

  orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  orb_->destroy ();

  ACE_Thread_Manager::instance ()->wait ();
}


void
DT_Test::dt_creator (MIF_DT_Creator* dt_creator)
{
  this->dt_creator_ = dt_creator;
}


MIF_Scheduler* 
DT_Test::scheduler (void)
{
  return this->scheduler_;
}

int
DT_Test::activate_task (void)
{
  
  
  long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |= 
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();
  
  if (this->activate (flags,
		      1) == -1)
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
  printf ("DT_test::svc started\n");
  ACE_TRY_NEW_ENV
    {
      dt_creator_->create_distributable_threads (orb_.in (),
						 current_.in ()
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

int
main (int argc, char* argv [])
{
  ACE_TRY_NEW_ENV
    {
      ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_MIF_DT_Creator);
      
      DT_TEST::instance ()->run (argc, argv
				 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "main ended\n"));
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
