//$Id$

#include "DT_Creator.h"
#include "ace/Arg_Shifter.h"
#include "Thread_Task.h"
#include "tao/ORB_Core.h"
#include "Task_Stats.h"
#include "ace/High_Res_Timer.h"

DT_Creator::DT_Creator (void)
{
  state_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
  active_dt_count_ = 0;
}

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
				       load,
				       this), -1);
	  
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
      active_dt_count_++;
    }
  
  ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch...\n"));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "Tasks have synched...\n"));


}

void
DT_Creator::dt_ended (void)
{
  ACE_DEBUG ((LM_DEBUG, "Active job count = %d\n",active_dt_count_));
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_dt_count_;
  }
  
  this->check_ifexit ();
}

void
DT_Creator::check_ifexit (void)
{
 // All tasks have finished and all jobs have been shutdown.
  if (active_dt_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Shutdown in progress ...\n"));
      // ask all tasks to dump stats.

      // TASK_LIST task_list;
      //       int count = builder_->task_list (task_list);
      
      //       char msg[BUFSIZ];
      //       ACE_OS::sprintf (msg, "# Stats generated on --\n");
      
      //       for (int i = 0; i < count; ++i)
      //         {
      //           task_list[i]->dump_stats (msg);
      //         }
      TASK_STATS::instance ()->dump_samples ("schedule",
					     "Schedule Output",
					     ACE_High_Res_Timer::global_scale_factor ());
      
      // shutdown the ORB
      orb_->shutdown (0);
    }
}

int
DT_Creator::dt_count (void)
{
  return dt_count_;
}

DT_Creator::~DT_Creator (void)
{
  delete state_lock_;
}


