//$Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"
#include "Task_Stats.h"

//#include "test.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_count = 0;

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
			    ACE_Barrier* barrier
			    ACE_ENV_ARG_DECL)
{
  barrier_ = barrier;
   
  base_time_ = base_time;

  current_ = RTScheduling::Current::_narrow (current
					     ACE_ENV_ARG_PARAMETER);	
  ACE_CHECK;

  sched_param_ = CORBA::Policy::_duplicate (sched_param);

  this->count_ = ++thread_count.value_i ();
 
//    ACE_DEBUG ((LM_DEBUG,
//  	      "thr_id = %d \n importance = %d\n",
//  	      count_,
//  	      importance_));

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
  
  return 0;
}

int
Thread_Task::svc (void)
{
  //this->barrier_->wait ();
  
  //ACE_OS::sleep (start_time_);
  
  //    ACE_hthread_t current;
  //    ACE_Thread::self (current);
  
  //    int priority;
  //    if (ACE_Thread::getprio (current, priority) == -1)
  //      {
  //        ACE_DEBUG ((LM_DEBUG,
  //                    ACE_TEXT ("TAO (%P|%t) - ")
  //                    ACE_TEXT ("RT_Protocols_Hooks::get_thread_priority: ")
  //                    ACE_TEXT (" ACE_Thread::get_prio\n")));
  
  //        return -1;
  //      }
  
  //    ACE_DEBUG ((LM_DEBUG,
  //  	      "::svc %d priority = %d\n",
  //  	      count_,
  //  	      priority));

  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::svc %d\n", count_);
  dt_creator_->log_msg (msg);
  
  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  this->current_->begin_scheduling_segment (name,
					    sched_param_.in (),
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->perform_task ();
  
  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  dt_creator_->dt_ended ();

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
}
