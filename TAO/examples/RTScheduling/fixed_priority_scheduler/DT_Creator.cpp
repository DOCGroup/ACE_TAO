//$Id$

#include "DT_Creator.h"

DT_Creator::DT_Creator (void)
{
  DT_Test::instance ()->dt_creator (this);
  scheduler_ = 
}

int
DT_Creator::init (int argc, char *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);
  
  const ACE_TCHAR* current_arg = 0;
  
  dt_count_ = 0;
  int dt_index = 0;
  FP_Scheduling::SegmentSchedulingParameterPolicy_var sched_param;
  int start_time;
  int load;

  flags_ = THR_NEW_LWP | THR_JOINABLE;
  flags_ |= 
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();
  
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
		  sched_param = DT_Test::instance ()->scheduler ()->create_segment_scheduling_parameter (ACE_OS::atoi (current_arg));
		  arg_shifter.consume_arg ();
		}
	      if ((current_arg = arg_shifter.get_the_parameter ("-Start_Time")))
		{
		  start_time = ACE_OS::atoi (current_arg);
		  arg_shifter.consume_arg ();
		}
	      if ((current_arg = arg_shifter.get_the_parameter ("-Load")))
		{
		  load = ACE_OS::atoi (current_arg);
		  arg_shifter.consume_arg ();
		}
	    }
  	  
          Thread_Task *task;
	  
          ACE_NEW (task, 
		   Thread_Task (DT_Test::instance ()->current (),
				sched_param.in (),
				start_time,
				load,
				flags_));
	  
	  dt_list_ [dt_index++] = task;
	}
    }
      
}

void 
DT_Creator::create_distributable_threads (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
{
  ACE_NEW (barrier_,
	   ACE_Barrier (this->dt_count_ + 1));
  
  for (int i = 0; i < this->dt_count_; i++)
    {
      dt_list_ [i]->activate_task (barrier_);
    }
  
  ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch...\n"));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "Tasks have synched...\n"));
}


ACE_STATIC_SVC_DEFINE(DT_Creator,
                      ACE_TEXT ("DT_Creator"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (DT_Creator),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (DT_Creator, DT_Creator)

