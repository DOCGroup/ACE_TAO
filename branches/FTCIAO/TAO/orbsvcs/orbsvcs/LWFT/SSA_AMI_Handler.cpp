#include "SSA_AMI_Handler.h"
#include <ace/Guard_T.h>


SSA_AMI_Handler::SSA_AMI_Handler()
  : accumulated_results_(0),
    success_(true)
    //phase_end_condition_(phase_wait_mutex_)
{}

bool SSA_AMI_Handler::wait_for_results(size_t expected_results, ACE_Time_Value abstime)
{
  if(expected_results == 0) 
    return true;

  ACE_Time_Value tv(0, 100), total(0,0);
  while(true)
  {
    ami_result_lock_.acquire();
    if((accumulated_results_ >= expected_results) || (total > abstime))
    {
      ami_result_lock_.release();
      break;
    }
    else
    {
      ami_result_lock_.release();
      ACE_OS::sleep(tv);
      total += tv;
    }
  }
  if(total > abstime)
    return false;
  else
    return success_;
}

void SSA_AMI_Handler::precommit_state(CORBA::Boolean b)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= (b ? 1 : 0);
  ++accumulated_results_;
}

void SSA_AMI_Handler::precommit_state_excep(Messaging::ExceptionHolder*)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  ++accumulated_results_;
}

void SSA_AMI_Handler::commit_state()
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= true;
  ++accumulated_results_;
}

void SSA_AMI_Handler::commit_state_excep(Messaging::ExceptionHolder*)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  ++accumulated_results_;
}

void SSA_AMI_Handler::transfer_state() 
{
  ACE_ERROR ((LM_EMERGENCY, 
              "(%P|%t) SSA_AMI_Handler::transfer_state () "
              "for the application %s\n"));
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= true;
  ++accumulated_results_;
}

void SSA_AMI_Handler::transfer_state_excep(Messaging::ExceptionHolder*) 
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  ++accumulated_results_;
}
   

/*
SSA_AMI_Handler::SSA_AMI_Handler()
  : ssa_invoked_(0),
    success_(true),
    phase_end_condition_(phase_wait_mutex_)
{}

bool SSA_AMI_Handler::wait_for_results(const ACE_Time_Value *abstime)
{
  if(!ssa_invoked_)
    return false;
  else if(phase_end_condition_.wait(phase_wait_mutex_, abstime) == -1)
    return false;
  else
    return success_;
}

void SSA_AMI_Handler::ssa_invoked()
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  ++ssa_invoked_;
}

void SSA_AMI_Handler::release()
{
  --ssa_invoked_;
  
  if((ssa_invoked_ == 0) || !success_)
      phase_end_condition_.broadcast();
}

void SSA_AMI_Handler::precommit_state(CORBA::Boolean b)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= (b ? 1 : 0);
  release();
}

void SSA_AMI_Handler::precommit_state_excep(Messaging::ExceptionHolder*)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  release();
}

void SSA_AMI_Handler::commit_state()
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= true;
  release();
}

void SSA_AMI_Handler::commit_state_excep(Messaging::ExceptionHolder*)
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  release();
}

void SSA_AMI_Handler::transfer_state() 
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ &= true;
  release();
}

void SSA_AMI_Handler::transfer_state_excep(Messaging::ExceptionHolder*) 
{
  ACE_Guard<ACE_Thread_Mutex> guard(ami_result_lock_);
  success_ = false;
  release();
}
    */ 
