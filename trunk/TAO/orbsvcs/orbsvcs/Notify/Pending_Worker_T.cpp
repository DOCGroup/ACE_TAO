// $Id$

#ifndef TAO_NS_PENDING_WORKER_T_CPP
#define TAO_NS_PENDING_WORKER_T_CPP

#include "Pending_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "Pending_Worker_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Pending_Worker_T, "$id$")

#include "tao/debug.h"
#include "Properties.h"
#include "Dispatch_Observer_T.h"

/*****************************************************************************************/

template <class PEER> void
TAO_NS_Dispatch_Pending_Worker_T<PEER>::work (PEER* peer ACE_ENV_ARG_DECL)
{
  peer->dispatch_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*****************************************************************************************/

template <class PEER>
TAO_NS_Pending_Worker_T<PEER>::TAO_NS_Pending_Worker_T (void)
  :dispatch_observer_ (0), done_ (0), suspended_ (0)
{
}

template <class PEER>
TAO_NS_Pending_Worker_T<PEER>::~TAO_NS_Pending_Worker_T ()
{
}

template <class PEER> void
TAO_NS_Pending_Worker_T<PEER>::init (TAO_NS_Dispatch_Observer_T<PEER>* dispatch_observer, ACE_Time_Value& update_period ACE_ENV_ARG_DECL)
{
  this->dispatch_observer_ = dispatch_observer;
  this->update_period_ = update_period;

  long flags = THR_NEW_LWP | THR_JOINABLE;

  flags |=
    TAO_NS_PROPERTIES::instance()->scope_policy () |
    TAO_NS_PROPERTIES::instance()->sched_policy ();

  // Become an active object.
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
                                            1,
                                            0,
                                            ACE_DEFAULT_THREAD_PRIORITY) == -1)
    {
      if (TAO_debug_level > 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Insufficient privilege.\n")));
          else
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%t) task activation at priority %d failed\n")
                        ACE_TEXT ("exiting!\n%a"),
                        ACE_DEFAULT_THREAD_PRIORITY));
        }

      ACE_THROW (CORBA::BAD_PARAM ());
    }
}

template <class PEER> int
TAO_NS_Pending_Worker_T<PEER>::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  while (!this->done_)
    {
      ACE_OS::sleep (this->update_period_);

      if (this->suspended_ == 1)
        continue;

      TAO_NS_Dispatch_Pending_Worker_T<PEER> worker;

      TAO_NS_Dispatch_Observer_T<PEER>::PEER_COLLECTION* peer_collection
        = this->dispatch_observer_->peer_collection ();

      if (peer_collection != 0)
        peer_collection->for_each (&worker ACE_ENV_ARG_PARAMETER);
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Exiting TAO_NS_Pending_Worker_T<PEER>::svc \n"));
    }
  return 0;
}

#endif /* TAO_NS_PENDING_WORKER_T_CPP */
