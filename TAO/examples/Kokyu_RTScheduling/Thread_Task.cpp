//$Id$

#include "Thread_Task.h"
#include "ace/Atomic_Op.h"
#include "Task_Stats.h"
#include "tao/ORB_Core.h"

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
			    ACE_Barrier* barrier
			    ACE_ENV_ARG_DECL)
{
  barrier_ = barrier;
   
  base_time_ = base_time;

  current_ = RTScheduling::Current::_narrow (current
					     ACE_ENV_ARG_PARAMETER);	
  ACE_CHECK;

  sched_param_ = CORBA::Policy::_duplicate (sched_param);

  //  this->count_ = ++thread_count.value_i ();
  
    ACE_DEBUG ((LM_DEBUG,
    	      "thr_id = %d \n importance = %d\n",
    	      count_,
    	      importance_));


  pre_activate ();

  long sched_policy = dt_creator_->orb ()->orb_core ()->orb_params ()->sched_policy ();

  if (sched_policy == THR_SCHED_FIFO || sched_policy == THR_SCHED_RR)
    {
      /*      if (this->activate (flags,
			  1,
			  0,
			  this->importance_) == -1)
      */
      if (this->activate (flags,1,0,0) == -1)
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
  static int count = 0;
  count++;
  printf("task started running\n");

  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  this->current_->begin_scheduling_segment (name,
					    sched_param_.in (),
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  barrier_->wait ();

  ACE_OS::memcpy (&count_,
		  this->current_->id ()->get_buffer (),
		  this->current_->id ()->length ());

  char msg [BUFSIZ];
  ACE_OS::sprintf (msg, "Thread_Task::svc %d\n", count_);
  dt_creator_->log_msg (msg);


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
