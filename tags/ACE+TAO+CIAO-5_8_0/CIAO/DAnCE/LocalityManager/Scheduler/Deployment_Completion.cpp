// $Id$
#include "Deployment_Completion.h"

#include "Logger/Log_Macros.h"

namespace DAnCE
{
  Deployment_Completion::Deployment_Completion (Deployment_Scheduler &sched)
    : Completion_Counter_Base< TAO_SYNCH_MUTEX > (0, 0),
      sched_ (sched),
      mutex_ (),
      condition_ (this->mutex_)
  {
  }

  Deployment_Completion::~Deployment_Completion (void)
  {
  }

  void
  Deployment_Completion::accept (Event_Future &future)
  {
    future.attach (this);
    this->increment_exec_count ();
  }

  void
  Deployment_Completion::update (const Event_Future &evt)
  {
    this->completed_events_.push_back (evt);
    this->decrement_exec_count ();
  }

  bool
  Deployment_Completion::wait_on_completion (ACE_Time_Value *tv)
  {
    if (!this->sched_.multithreaded ())
      return this->single_threaded_wait_on_completion (tv);

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->mutex_, false);

    while (!this->all_completed ())
      {
        int retval = this->condition_.wait (tv);

        if (retval == -1)
          {
            DANCE_ERROR (2, (LM_ERROR, DLINFO
                             ACE_TEXT ("Deployment_Completion::wait_on_completion - ")
                             ACE_TEXT ("Timed out waiting on event completion\n")));
            return false;
          }
      }

    DANCE_DEBUG (8, (LM_DEBUG, DLINFO
                     ACE_TEXT ("Deployment_Completion::wait_on_completion - ")
                     ACE_TEXT ("All events completed\n")));

    return true;
  }

  void
  Deployment_Completion::completed_events (Event_List &event_list)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->mutex_, CORBA::NO_RESOURCES ());

    event_list.swap (this->completed_events_);
    this->completed_events_.clear ();
  }

  void
  Deployment_Completion::on_all_completed ()
  {
    this->condition_.broadcast ();
  }

  void
  Deployment_Completion::on_all_completed_with_failure ()
  {
    this->condition_.broadcast ();
  }

  bool
  Deployment_Completion::single_threaded_wait_on_completion (ACE_Time_Value * /*tv*/)
  {
    while (this->sched_.work_pending ())
      this->sched_.perform_work ();

    return true;
  }
}

