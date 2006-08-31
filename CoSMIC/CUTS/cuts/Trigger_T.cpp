// $Id$

#if !defined (__CUTS_INLINE__)
# include "cuts/Trigger_T.inl"
#endif

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "ace/Time_Value.h"

//
// CUTS_Periodic_Trigger_T
//
template <typename COMPONENT>
CUTS_Periodic_Trigger_T <COMPONENT>::CUTS_Periodic_Trigger_T (void)
: component_ (0),
  method_ (0),
  timer_ (-1),
  timeout_ (1000),
  probability_ (1.0)
{
  ACE_OS::srand (ACE_OS::time (0));
}

//
// ~CUTS_Periodic_Trigger_T
//
template <typename COMPONENT>
CUTS_Periodic_Trigger_T <COMPONENT>::~CUTS_Periodic_Trigger_T (void)
{

}

//
// activate
//
template <typename COMPONENT>
void CUTS_Periodic_Trigger_T <COMPONENT>::activate (long msec)
{
  // Schedule the timeout and activate the <timer_queue_>.
  int retval = this->timer_queue_.activate ();
  this->schedule_timeout (msec);

  if (retval == -1)
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - failed to activate periodic task\n"));
  }
}

//
// deactivate
//
template <typename COMPONENT>
void CUTS_Periodic_Trigger_T <COMPONENT>::deactivate (void)
{
  this->timer_queue_.deactivate ();
  this->cancel_timeout ();
}

//
// handle_timeout
//
template <typename COMPONENT>
int CUTS_Periodic_Trigger_T <COMPONENT>::handle_timeout (
  const ACE_Time_Value &current_time,
  const void * act)
{
  // Calculate the score for this <timeout_> event.
  double score = (double) ACE_OS::rand () / (double) RAND_MAX;

  // invoke the method if the score is within the <probability_>
  if (score <= this->probability_)
  {
    (this->component_->*this->method_) ();
  }

  return 0;

  ACE_UNUSED_ARG (current_time);
  ACE_UNUSED_ARG (act);
}

//
// schedule_timeout
//
template <typename COMPONENT>
void CUTS_Periodic_Trigger_T <COMPONENT>::schedule_timeout (long msec)
{
  if (this->timer_ == -1)
  {
    ACE_Time_Value interval;
    interval.msec (msec);

    ACE_Time_Value abstime =
      this->timer_queue_.timer_queue ()->gettimeofday () + interval;

    this->timer_ = this->timer_queue_.schedule (this, 0, abstime, interval);

    if (this->timer_ == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - failed to schedule timer\n"));
    }
  }
  else
  {
    ACE_ERROR ((LM_NOTICE,
                "[%M] -%T - timer has already been scheduled\n"));
  }
}
