// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_Task.h"

#if !defined (__ACE_INLINE__)
# include "tao/Dynamic_TP/Dynamic_TP_Task.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_Task::~TAO_Dynamic_TP_Task()
{
}

int
TAO_Dynamic_TP_Task::open(void* num_threads_ptr, void* thread_stack_size_ptr)
{
  Thread_Counter num = 1;

  Thread_Counter* tmp = static_cast<Thread_Counter*> (num_threads_ptr);

  if (tmp == 0)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%P|%t) TP_Task failed to open.  ")
                        ACE_TEXT ("Invalid argument type passed to open().\n")),
                        -1);
      //FUZZ: enable check_for_lack_ACE_OS
    }

  num = *tmp;

  // We can't activate 0 threads.  Make sure this isn't the case.
  if (num < 1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%P|%t) TP_Task failed to open.  ")
                        ACE_TEXT ("num_threads (%u) is less-than 1.\n"),
                        num),
                       -1);
    }

  // We need the lock acquired from here on out.
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, -1);

  // We can assume that we are in the proper state to handle this open()
  // call as long as we haven't been open()'ed before.
  if (this->opened_)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%P|%t) TP_Task failed to open.  ")
                        ACE_TEXT ("Task has previously been open()'ed.\n")),
                       -1);
      //FUZZ: enable check_for_lack_ACE_OS
    }

  // Activate this task object with 'num' worker threads.
  if (this->activate(THR_NEW_LWP | THR_JOINABLE, num) != 0)
    {
      // Assumes that when activate returns non-zero return code that
      // no threads were activated.
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%P|%t) TP_Task failed to activate ")
                        ACE_TEXT ("(%d) worker threads.\n"),
                        num),
                       -1);
    }

  // Now we have past the point where we can say we've been open()'ed before.
  this->opened_ = true;

  // Now we wait until all of the threads have started.
  while (this->num_threads_ != num)
    {
      this->active_workers_.wait();
    }

  // We can now accept requests (via our add_request() method).
  this->accepting_requests_ = true;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
