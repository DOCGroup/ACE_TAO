//$Id$

#include "test.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/ORB_Core.h"


void
DT_Test::check_supported_priorities (void)
{
  // Check that we have sufficient priority range to run this
  // test, i.e., more than 1 priority level.

  this->thr_sched_policy_ = orb_->orb_core ()->orb_params ()->sched_policy ();
  this->thr_scope_policy_ = orb_->orb_core ()->orb_params ()->scope_policy ();

  if (thr_sched_policy_ == THR_SCHED_FIFO)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_FIFO\n"));

      sched_policy_ = ACE_SCHED_FIFO;
    }
  else if (thr_sched_policy_ == THR_SCHED_RR)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_RR\n"));

      sched_policy_ = ACE_SCHED_RR;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_OTHER\n"));

      sched_policy_ = ACE_SCHED_OTHER;
    }


  max_priority_ = ACE_Sched_Params::priority_max (sched_policy_);
  min_priority = ACE_Sched_Params::priority_min (sched_policy_);

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

void 
DT_Test::init_test (int argc, char *argv []
	    ACE_ENV_ARG_DECL)
{
  orb_ = CORBA::ORB_init (argc,
				argv,
				""
				ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  this->check_supported_priorities ();
  
  CORBA::Object_ptr manager_obj = orb_->resolve_initial_references ("RTSchedulerManager"
								   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
      
  TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj
									  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
      
  
  ACE_NEW (scheduler_,
	   Fixed_Priority_Scheduler (orb.in ()));
  
  manager->rtscheduler (scheduler_);
  
  CORBA::Object_var object =
    orb_->resolve_initial_references ("RTScheduler_Current" 
				      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  current_  =
    RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  flags_ = THR_NEW_LWP | THR_JOINABLE;
  flags_ |= 
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();
  
  // Set the main thread to max priority...
  if (ACE_OS::sched_params (ACE_Sched_Params (sched_policy_,
					      max_priority,
					      sched_scope_)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "(%P|%t): user is not superuser, "
		      "test runs in time-shared class\n"));
	}
      else
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t): sched_params failed\n"),-1);
    }
}

void 
DT_Test::init_distributable_threads (int argc, char *argv []
				  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  ACE_Arg_Shifter arg_shifter (argc, argv);
  
  const ACE_TCHAR* current_arg = 0;
  
  dt_count_ = 0;
  int dt_index = 0;
  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr sched_param;
  int start_time;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-DT_Count")))
        {
          dt_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW (dt_list_, Thread_Task*[dt_count_]);
          arg_shifter.consume_arg ();
        }
      if ((current_arg = arg_shifter.get_the_parameter ("-DT_Task")))
	{
          arg_shifter.consume_arg ();
	  
	  while (arg_shifter.is_anything_left ())
	    {
	      if ((current_arg = arg_shifter.get_the_parameter ("-Importance")))
		{
		  sched_param = scheduler.create_segment_scheduling_parameter (current_arg);
		  arg_shifter.consume_arg ();
		}
	      if ((current_arg = arg_shifter.get_the_parameter ("-Start_Time")))
		{
		  start_time = current_arg;
		  arg_shifter.consume_arg ();
		}
	      if ((current_arg = arg_shifter.get_the_parameter ("-Load")))
		{
		  load = current_arg;
		  arg_shifter.consume_arg ();
		}
	    }
  	  
          Thread_Task *task;
	  
          ACE_NEW (task, 
		   Thread_Task (current_.in (),
				sched_param,
				start_time,
				load,
				flags_,
				&barrier_));
	  
	  dt_list_ [dt_index++] = task;
	}
    }
      
}

void 
DT_Test::create_distributable_threads (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
{
  ACE_NEW (barrier_,
	   ACE_Barrier (this->dt_count_ + 1));
  
	   
  ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch...\n"));
  barrier.wait ();
  ACE_DEBUG ((LM_DEBUG, "Tasks have synched...\n"));
}

int
main (int argc, char* argv [])
{
  ACE_TRY_NEW_ENV
    {
      init_test (argc,argv
		 ACE_ENV_ARG_PARAMETER);
      
      
      
      orb->run ();
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

ACE_STATIC_SVC_DEFINE(DT_Test,
                      ACE_TEXT ("DT_Test"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (DT_Test),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (test, DT_Test)
