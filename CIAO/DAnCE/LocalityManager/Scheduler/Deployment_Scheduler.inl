// $Id$
#ifndef DEPLOYMENT_SCHEDULER_INL
#define DEPLOYMENT_SCHEDULER_INL

namespace DAnCE
{
  ACE_INLINE
  void Deployment_Scheduler::activate_scheduler (size_t threads)
    {
      this->multithread_ = threads > 0;
      if (this->multithread_)
        this->activate (THR_DETACHED,
                        threads);
    }

  ACE_INLINE
  bool Deployment_Scheduler::multithreaded (void)
    {
      return this->multithread_;
    }

  ACE_INLINE
  bool Deployment_Scheduler::work_pending (void)
    {
      return !this->event_queue_.is_empty ();
    }

  ACE_INLINE
  void Deployment_Scheduler::perform_work (void)
    {
      auto_ptr < ACE_Method_Request > de (this->event_queue_.dequeue ());

      if (de.get ())
        {
          DANCE_DEBUG (10, (LM_TRACE, DLINFO
                            ACE_TEXT ("Deployment_Scheduler::perform_work - ")
                            ACE_TEXT ("Invoking a deployment event\n")));
          de->call ();
        }
    }
}

#endif
