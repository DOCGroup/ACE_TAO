// $Id$

#include "EC_MT_Dispatching.h"

#if ! defined (__ACE_INLINE__)
#include "EC_MT_Dispatching.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_MT_Dispatching, "$Id$")

TAO_EC_MT_Dispatching::TAO_EC_MT_Dispatching (int nthreads,
                                              int thread_creation_flags,
                                              int thread_priority,
                                              int force_activate)
  :  nthreads_ (nthreads),
     thread_creation_flags_ (thread_creation_flags_),
     thread_priority_ (thread_priority),
     force_activate_ (force_activate),
     task_ (&this->thread_manager_),
     active_ (0)
{
}

void
TAO_EC_MT_Dispatching::activate (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->active_ != 0)
    return;
  
  this->active_ = 1;

  if (this->task_.activate (this->thread_creation_flags_,
                            this->nthreads_,
                            1,
                            this->thread_priority_) == -1)
    {
      if (this->force_activate_ != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "EC (%P|%t) activating dispatching queue at"
                      " default priority\n"));
          if (this->task_.activate (THR_BOUND, this->nthreads_) == -1)
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate dispatching queue"));
        }
    }
}

void
TAO_EC_MT_Dispatching::shutdown (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->active_ == 0)
    return;

  for (int i = 0; i < this->nthreads_; ++i)
    {
      this->task_.putq (new TAO_EC_Shutdown_Command);
    }
  this->thread_manager_.wait ();
}

void
TAO_EC_MT_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                             const RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment& ACE_TRY_ENV)
{
  RtecEventComm::EventSet event_copy = event;
  this->push_nocopy (proxy, event_copy, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_MT_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                    RtecEventComm::EventSet& event,
                                    TAO_EC_QOS_Info& qos_info,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Double checked locking....
  if (this->active_ == 0)
    this->activate ();

  this->task_.push (proxy, event, ACE_TRY_ENV);
}
