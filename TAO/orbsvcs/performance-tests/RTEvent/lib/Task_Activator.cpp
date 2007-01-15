/**
 * @file Task_Activator.cpp
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_TASK_ACTIVATOR_CPP
#define TAO_PERF_RTEC_TASK_ACTIVATOR_CPP

#include "Task_Activator.h"
#include "tao/Environment.h"
#include "ace/Log_Msg.h"

#if !defined(__ACE_INLINE__)
#include "Task_Activator.inl"
#endif /* __ACE_INLINE__ */

template<class Task>
Task_Activator<Task>::Task_Activator (int priority,
                                      int scheduling_class,
                                      int nthreads,
                                      Task *task)
  : task_ (task)
{
  if (this->task_ == 0)
    return;

  // We explicitly ignore errors, this is exactly what we do in the
  // test anyway.  In a generic class we would need to throw an
  // exception.
  if (this->task_->activate (scheduling_class | THR_NEW_LWP | THR_JOINABLE,
                             nthreads,
                             1, // force_activation
                             priority) == -1)
    {
      // Release, we cannot wait for it or anything like that...
      this->task_ = 0;
      ACE_DEBUG ((LM_DEBUG,
                  "Task_Activator failed %p\n", ""));
    }
}

template<class Task>
Task_Activator<Task>::~Task_Activator (void)
{
  if (this->task_ == 0)
    return;

  // Wait until the task terminates
  this->task_->wait ();
}

#endif /* TAO_PERF_RTEC_TASK_ACTIVATOR_CPP */
