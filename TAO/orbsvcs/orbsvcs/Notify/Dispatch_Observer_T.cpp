// $Id$

#ifndef TAO_NS_DISPACH_OBSERVER_T_CPP
#define TAO_NS_DISPACH_OBSERVER_T_CPP

#include "Dispatch_Observer_T.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatch_Observer_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Dispatch_Observer_T, "$id$")

#include "tao/debug.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Peer.h"
#include "Proxy.h"
#include "Properties.h"
#include "Factory.h"

template <class PEER>
TAO_NS_Dispatch_Observer_T<PEER>::TAO_NS_Dispatch_Observer_T (void)
: max_retry_attempts_ (0)
{
}

template <class PEER>
TAO_NS_Dispatch_Observer_T<PEER>::~TAO_NS_Dispatch_Observer_T ()
{
  delete this->peer_collection_;
}

template <class PEER> void
TAO_NS_Dispatch_Observer_T<PEER>::init (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  factory->create (this->peer_collection_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class PEER> void
TAO_NS_Dispatch_Observer_T<PEER>::enqueue (PEER* peer ACE_ENV_ARG_DECL)
{
  this->peer_collection_->connected (peer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class PEER> void
TAO_NS_Dispatch_Observer_T<PEER>::dispatch_failure (PEER* peer, int retry_count ACE_ENV_ARG_DECL)
{
  if (retry_count > this->max_retry_attempts_)
    {
      this->peer_collection_->disconnected (peer ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_TRY
        {
          peer->proxy ()->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "Dispatch Observer: Exxception in dispatch_failure handling\n"));
        }
      ACE_ENDTRY;
    }
  else if (retry_count == 1) // failed for the 1st time.
    {
      this->peer_collection_->connected (peer ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

template <class PEER> void
TAO_NS_Dispatch_Observer_T<PEER>::dispatch_success (PEER* peer ACE_ENV_ARG_DECL)
{
  this->peer_collection_->disconnected (peer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#endif /* TAO_NS_DISPACH_OBSERVER_T_CPP */
