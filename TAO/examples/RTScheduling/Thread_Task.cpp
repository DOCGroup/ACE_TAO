//$Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"
#include "Task_Stats.h"
#include "tao/ORB_Core.h"
#include "ace/High_Res_Timer.h"

//#include "test.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_count = 0;

Thread_Task::Thread_Task (void)
{
    

}

//  Thread_Task::Thread_Task (int importance,
//  			  int start_time,
//  			  int load,
//  			  DT_Creator *dt_creator)
//  {
//    this->load_ = load;
//    this->start_time_ = start_time;
//    this->importance_ = importance;
//    this->dt_creator_ = dt_creator;
//    //this->task_ = task;
//  }


int
Thread_Task::activate_task (RTScheduling::Current_ptr current,
			    CORBA::Policy_ptr sched_param,
			    long flags,
			    ACE_Time_Value* base_time,
			    ACE_hrtime_t base_hr_time,
			    ACE_Barrier* barrier
			    ACE_ENV_ARG_DECL)
{

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Thread_Task::activate %d\n",
		importance_));
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::activate task\n");
  dt_creator_->log_msg (msg);

  barrier_ = barrier;
   
  base_time_ = base_time;

  base_hr_time_ = base_hr_time;

  current_ = RTScheduling::Current::_narrow (current
					     ACE_ENV_ARG_PARAMETER);	
  ACE_CHECK;

  sched_param_ = CORBA::Policy::_duplicate (sched_param);

  //  this->count_ = ++thread_count.value_i ();
  
  //    ACE_DEBUG ((LM_DEBUG,
  //  	      "thr_id = %d \n importance = %d\n",
  //  	      count_,
  //  	      importance_));
  
  
  pre_activate ();

  
  CORBA::ORB_var orb =  CORBA::ORB::_duplicate (dt_creator_->orb ());
  TAO_ORB_Core* orb_core = orb->orb_core ();
  long sched_policy =  orb_core->orb_params ()->sched_policy ();
  

  if (sched_policy == THR_SCHED_FIFO || sched_policy == THR_SCHED_RR)
    {
      //ACE_DEBUG ((LM_DEBUG, "Activated thread \n"));
      
      if (this->activate (flags,
			  1,
			  0,
			  this->importance_) == -1)
	{
	  if (ACE_OS::last_error () == EPERM)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       ACE_TEXT ("Insufficient privilege to run this test.\n")),
			      -1);
	}
    }
  else 
    if (this->activate (flags,
			1) == -1)
      {
	if (ACE_OS::last_error () == EPERM)
	  ACE_ERROR_RETURN ((LM_ERROR,
			     ACE_TEXT ("Insufficient privilege to run this test.\n")),
			    -1);
      }
  
  //    mutex_.acquire ();
  //    while (!start_)
  //      task_cond_.wait ();
  //    mutex_.release ();
  post_activate ();
  return 0;
}

int
Thread_Task::svc (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Thread_Task::svc %d\n",
		importance_));
  /*
  ACE_hthread_t current;
  ACE_Thread::self (current);
  */

  /*
  int priority;    
  if (ACE_Thread::getprio (current, priority) == -1)
    return -1;
  */

  
  //  static int count = 0;
  //count++;

  
  const char * name = 0;
  CORBA::Policy_var implicit_sched_param = CORBA::Policy::_duplicate (sched_param_.in ());;
  this->current_->begin_scheduling_segment (name,
					    sched_param_.in (),
					    implicit_sched_param.in ()
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;



  ACE_OS::memcpy (&count_,
		  this->current_->id ()->get_buffer (),
		  this->current_->id ()->length ());
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::svc Imp=%d Guid=%d\n", importance_, count_);
  dt_creator_->log_msg (msg);
  
  
  this->perform_task ();

  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  dt_creator_->dt_ended () ;

  return 0;
}

int
Thread_Task::importance (void)
{
  return this->importance_;
}

int
Thread_Task::start_time (void)
{
  return this->start_time_;
}

int
Thread_Task::perform_task (void)
{
  return 0;
}

void
Thread_Task::pre_activate (void)
{
}

void
Thread_Task::post_activate (void)
{
}

const char*
Thread_Task::job (void)
{
  return job_name_;
}

void
Thread_Task::job (Job_ptr job)
{
  job_ = Job::_duplicate (job);
}

int
Thread_Task::dist (void)
{
  return this->dist_;
}


void
Thread_Task::dump_stats (void)
{
  char fname [BUFSIZ];
  ACE_OS::sprintf (fname, "DT_%d.dat",count_);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"File name %s\n",
		fname));
  
  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "#Schedule Output for DT %d", count_);
  
  task_stats_->dump_samples (fname,
			     msg,
			     ACE_High_Res_Timer::global_scale_factor ());


}
