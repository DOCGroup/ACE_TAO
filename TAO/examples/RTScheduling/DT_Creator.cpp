//$Id$

#include "DT_Creator.h"
#include "ace/Arg_Shifter.h"
#include "Thread_Task.h"
#include "tao/ORB_Core.h"

int
DT_Creator::init (int argc, char *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);
  
  const ACE_TCHAR* current_arg = 0;
  
  dt_count_ = 0;
  int dt_index = 0;
  int start_time;
  int load;
  int importance;
  Thread_Task *task;
  
  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-DT_Count")))
        {
          dt_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (dt_list_, Thread_Task*[dt_count_], -1);
          arg_shifter.consume_arg ();
        }
      if (arg_shifter.cur_arg_strncasecmp ("-DT_Task") == 0)
	{
          arg_shifter.consume_arg ();
	  
	  if (arg_shifter.cur_arg_strncasecmp ("-Importance") == 0)
	    {
	      arg_shifter.consume_arg ();
	      current_arg = arg_shifter.get_current ();
	      importance = ACE_OS::atoi (current_arg);
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
	  
        ACE_NEW_RETURN (task, 
			  Thread_Task (importance,
				       start_time,
				       load), -1);
	  
	  dt_list_ [dt_index++] = task;
	}
    }
  return 0;
      
}

void 
DT_Creator::create_distributable_threads (CORBA::ORB_ptr orb,
										  RTScheduling::Current_ptr current
					  ACE_ENV_ARG_DECL)
{
  ACE_NEW (barrier_,
	   ACE_Barrier (this->dt_count_ + 1));
  
  orb_ = CORBA::ORB::_duplicate (orb);  

  long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |= 
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();

  CORBA::Policy_var sched_param;
    
  
  for (int i = 0; i < this->dt_count_; i++)
    {
      sched_param = CORBA::Policy::_duplicate (this->sched_param (dt_list_ [i]->importance ()));
      dt_list_ [i]->activate_task (current,
				   sched_param.in (),
				   flags,
				   barrier_
				   ACE_ENV_ARG_PARAMETER);
    }
  
  ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch...\n"));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "Tasks have synched...\n"));


}






