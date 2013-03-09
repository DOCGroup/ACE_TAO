// $Id$

#include "orbsvcs/CosEvent/CEC_MT_Dispatching.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL




TAO_CEC_MT_Dispatching::TAO_CEC_MT_Dispatching (int nthreads,
                                                int thread_creation_flags,
                                                int thread_priority,
                                                int force_activate)
  :  nthreads_ (nthreads),
     thread_creation_flags_ (thread_creation_flags),
     thread_priority_ (thread_priority),
     force_activate_ (force_activate),
     task_ (&this->thread_manager_),
     active_ (0)
{
}

void
TAO_CEC_MT_Dispatching::activate (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

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
          if (this->task_.activate (THR_BOUND, this->nthreads_) == -1)
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate dispatching queue"));
        }
    }
}

void
TAO_CEC_MT_Dispatching::shutdown (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->active_ == 0)
    return;

  for (int i = 0; i < this->nthreads_; ++i)
    {
      this->task_.putq (new TAO_CEC_Shutdown_Task_Command);
    }
  this->thread_manager_.wait ();
}

void
TAO_CEC_MT_Dispatching::push (TAO_CEC_ProxyPushSupplier* proxy,
                              const CORBA::Any& event)
{
  CORBA::Any event_copy = event;
  this->push_nocopy (proxy, event_copy);
}

void
TAO_CEC_MT_Dispatching::push_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                                     CORBA::Any& event)
{
  // Double checked locking....
  if (this->active_ == 0)
    this->activate ();

  this->task_.push (proxy, event);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_MT_Dispatching::invoke (TAO_CEC_ProxyPushSupplier* proxy,
                                const TAO_CEC_TypedEvent& typed_event)
{
  TAO_CEC_TypedEvent typed_event_copy = typed_event;
  this->invoke_nocopy (proxy, typed_event_copy);
}

void
TAO_CEC_MT_Dispatching::invoke_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                                       TAO_CEC_TypedEvent& typed_event)
{
  // Double checked locking....
  if (this->active_ == 0)
    this->activate ();

  this->task_.invoke (proxy, typed_event);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_END_VERSIONED_NAMESPACE_DECL
