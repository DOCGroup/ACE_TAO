//$Id$

#include "Thread_Task.h"

#include "ace/Atomic_Op.h"

ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_count = 0;

Thread_Task::Thread_Task (RTScheduling::Current_ptr current)
{
  this->current_ = RTScheduling::Current::_narrow (current);
}

int
Thread_Task::activate_task (CORBA::Policy_ptr sched_param,
			    long flags,
			    ACE_Barrier* barrier)
{
  this->sched_param_ = CORBA::Policy::_duplicate (sched_param);
  this->barrier_ = barrier;

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
      

  this->count_ = ++thread_count.value_i ();

  this->barrier_->wait ();

  const char * name = 0;
  CORBA::Policy_ptr implicit_sched_param = 0;
  this->current_->begin_scheduling_segment (name,
					    this->sched_param_.in (),
					    implicit_sched_param
					    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->count_ == 2)
    ACE_OS::sleep (10);

  for (int i = 0; i < 100; i++)
    {

      this->perform_task (10000);

      ACE_DEBUG ((LM_DEBUG,
		  "%t|%T %d\n", 
		  this->count_));
      
    }

  this->current_->end_scheduling_segment (name
					  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  return 0;
}



