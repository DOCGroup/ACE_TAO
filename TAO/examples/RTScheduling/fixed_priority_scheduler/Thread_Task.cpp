//$Id$

#include "Thread_Task.h"

#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_count = 0;

Thread_Task::Thread_Task (RTScheduling::Current_ptr current,
			  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr sched_param,
			  int start_time,
			  int load,
			  long flags)
{
  this->current_ = RTScheduling::Current::_narrow (current);
  this->sched_param_ = FP_Scheduling::SegmentSchedulingParameterPolicy::_duplicate (sched_param);
  this->load_ = load;
  this->start_time_ = start_time;
  this->flags_ = flags;

  this->count_ = ++thread_count.value_i ();
}


int
Thread_Task::activate_task (ACE_Barrier* barrier)
{
  this->barrier_ = barrier;
  
  if (this->activate (flags_,
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
Thread_Task::perform_task (int times)
{
  static CORBA::ULong prime_number = 9619;
  
  for (int j = 0; j < times; j++)
    {
      
      ACE::is_prime (prime_number,
		     2,
		     prime_number / 2);
    }
  return 0;
}

int
Thread_Task::svc (void)
{

  ACE_DEBUG ((LM_DEBUG,
	      "After Thread_Task::svc \n"));
      
  this->barrier_->wait ();

  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  this->current_->begin_scheduling_segment (name,
					    this->sched_param_.in (),
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_OS::sleep (start_time_);

  for (int i = 0; i < 100; i++)
    {

      this->perform_task (load_);

      ACE_DEBUG ((LM_DEBUG,
		  "%d\n", 
		  this->count_));
      
    }

  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  return 0;
}



