//$Id$

#include "DT_Creator.h"
#include "ace/Arg_Shifter.h"
#include "Thread_Task.h"
#include "tao/ORB_Core.h"
#include "Task_Stats.h"
#include "ace/High_Res_Timer.h"
#include "Task.h"

int log_index = 0;
int
DT_Creator::init (int argc, char *argv [])
{
  state_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
  shutdown_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
  active_dt_count_ = 0;
  ACE_NEW_RETURN (log, char*[BUFSIZ],-1);

  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;
  
  dt_count_ = 0;
  int dt_index = 0;
  int start_time;
  int load;
  int importance;
  int total_load = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-DT_Count")))
        {
          dt_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (dt_list_, Thread_Task*[dt_count_], -1);
	  active_dt_count_ = dt_count_;
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
	      total_load += load;
	      arg_shifter.consume_arg ();
	    }

	  dt_list_ [dt_index++] = this->create_thr_task (importance,
							 start_time,
							 load);
	}
    }
  
  TASK_STATS::instance ()->init (total_load);
    
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
  sched_param = CORBA::Policy::_duplicate (this->sched_param (15));
  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  current->begin_scheduling_segment (name,
				     sched_param.in (),
				     implicit_sched_param
				     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  ACE_Time_Value* base_time;
  ACE_NEW (base_time,
	   ACE_Time_Value (ACE_OS::gettimeofday ()));

  for (int i = 0; i < this->dt_count_; i++)
    {
      ACE_Time_Value now (ACE_OS::gettimeofday ());
      
      ACE_Time_Value elapsed_time =  now - *base_time;
      
      char buf [BUFSIZ];
      ACE_OS::sprintf (buf, "elapsed time = %d\n now = %d\n base_time = %d\n",
		       (int) elapsed_time.sec (),
		       (int) now.sec (),
		       (int) base_time->sec());
      
      log [log_index++] = ACE_OS::strdup (buf) ; 
      
      ACE_hthread_t curr_thr;
      ACE_Thread::self (curr_thr);
      
      if (dt_list_ [i]->start_time () != 0 && (elapsed_time.sec () < dt_list_[i]->start_time ()))
	{
	  int suspension_time = dt_list_[i]->start_time () - elapsed_time.sec ();
	  ACE_OS::sprintf (buf,"suspension_tome = %d\n",
			   suspension_time);
	  log [log_index++] = ACE_OS::strdup (buf);
	  yield (suspension_time);
	}
      
      sched_param = CORBA::Policy::_duplicate (this->sched_param (dt_list_ [i]->importance ()));
      dt_list_ [i]->activate_task (current,
				   sched_param.in (),
				   flags,
				   base_time,
				   barrier_
				   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

    }
  
  current->end_scheduling_segment (name
				   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
DT_Creator::dt_ended (void)
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_dt_count_;
    //ACE_DEBUG ((LM_DEBUG, "Active job count = %d\n",active_dt_count_));
    char buf [BUFSIZ];
    ACE_OS::sprintf (buf,"Active job count = %d\n",active_dt_count_);
    log [log_index++] = ACE_OS::strdup (buf);
  }
  this->check_ifexit ();
}

void
DT_Creator::check_ifexit (void)
{
  static int shutdown = 0;

  {
    ACE_GUARD (ACE_Lock, ace_mon, *shutdown_lock_); 

    if (!shutdown)
      {
	// All tasks have finished and all jobs have been shutdown.
	if (active_dt_count_ == 0)
	  {
	
	    ACE_DEBUG ((LM_DEBUG, "Shutdown in progress ...\n"));
	    
	    TASK_STATS::instance ()->dump_samples ("schedule.dat",
						   "#Schedule Output",
						   ACE_High_Res_Timer::global_scale_factor ());

	    shutdown = 1;
	    
	    FILE* log_file = ACE_OS::fopen ("log_file", "w");

	    if (log_file != NULL)
	      {
		// first dump what the caller has to say.
		ACE_OS::fprintf (log_file, "Log File\n");
		
		for (int i = 0; i < log_index; i++)
		  {
		    ACE_OS::fprintf (log_file, "%s\n", log [i]);
		  }
		
		ACE_OS::fclose (log_file);
	      }

	    // shutdown the ORB
	    orb_->shutdown (0);

	    
	  }
      }
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


void
DT_Creator::log_msg (char* msg)
{
  log [log_index++] = ACE_OS::strdup (msg);
}
