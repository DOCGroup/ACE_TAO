// $Id$

#ifndef TAO_NS_EVENT_MAP_ENTRY_OBSERVER_T_CPP
#define TAO_NS_EVENT_MAP_ENTRY_OBSERVER_T_CPP

#include "Event_Map_Observer_T.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Map_Observer_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Event_Map_Observer_T, "$id$")

#include "Proxy.h"
#include "Peer.h"
#include "Dispatch_Observer_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Event_Map_T.h"

/******************************************************************************************/

template <class PROXY>
TAO_NS_Update_Added_Worker<PROXY>::TAO_NS_Update_Added_Worker (const TAO_NS_EventType& event_type, TAO_NS_Updates_Dispatch_Observer* dispatch_observer)
  :event_type_ (event_type), dispatch_observer_ (dispatch_observer)
{
}

template <class PROXY> void
TAO_NS_Update_Added_Worker<PROXY>::work (PROXY* proxy ACE_ENV_ARG_DECL)
{
  proxy->type_added (this->event_type_);

  if (proxy->updates_off () == 0)
    {
      this->dispatch_observer_->enqueue (proxy->peer () ACE_ENV_ARG_PARAMETER); // Tell the observer that this peer has a update pending.
    }
}

/******************************************************************************************/

template <class PROXY>
TAO_NS_Update_Removed_Worker<PROXY>::TAO_NS_Update_Removed_Worker (const TAO_NS_EventType& event_type, TAO_NS_Updates_Dispatch_Observer* dispatch_observer)
  :event_type_ (event_type), dispatch_observer_ (dispatch_observer)
{
}

template <class PROXY> void
TAO_NS_Update_Removed_Worker<PROXY>::work (PROXY* proxy ACE_ENV_ARG_DECL)
{
  if (proxy->updates_off () == 0)
    {
      proxy->type_removed (this->event_type_);
      this->dispatch_observer_->enqueue (proxy->peer () ACE_ENV_ARG_PARAMETER); // Tell the observer that this peer has a update pending.
    }
}

/******************************************************************************************/

template <class PROXY>
TAO_NS_Event_Map_Observer_T<PROXY>::TAO_NS_Event_Map_Observer_T (void)
  :event_map_ (0), dispatch_observer_ (0)
{
}

template <class PROXY>
TAO_NS_Event_Map_Observer_T<PROXY>::~TAO_NS_Event_Map_Observer_T ()
{
}

template <class PROXY> void
TAO_NS_Event_Map_Observer_T<PROXY>::init (EVENT_MAP* event_map, TAO_NS_Updates_Dispatch_Observer* dispatch_observer)
{
  this->event_map_ = event_map;
  this->dispatch_observer_ = dispatch_observer;
}

template <class PROXY> void
TAO_NS_Event_Map_Observer_T<PROXY>::type_added (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  UPDATE_ADDED_WORKER worker (event_type, this->dispatch_observer_);

  PROXY_COLLECTION* proxys = this->event_map_->find (event_type ACE_ENV_ARG_PARAMETER);

  if (proxys != 0)
    proxys->for_each (&worker ACE_ENV_ARG_PARAMETER);

  // Get the default broadcast collection.
  proxys = this->event_map_->broadcast_collection ();

  if (proxys != 0)
    proxys->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

template <class PROXY> void
TAO_NS_Event_Map_Observer_T<PROXY>::type_removed (const TAO_NS_EventType& event_type  ACE_ENV_ARG_DECL)
{
  UPDATE_REMOVED_WORKER worker (event_type, this->dispatch_observer_);

  PROXY_COLLECTION* proxys = this->event_map_->find (event_type ACE_ENV_ARG_PARAMETER);

  if (proxys != 0)
    proxys->for_each (&worker ACE_ENV_ARG_PARAMETER);

  // Get the default broadcast collection.
  proxys = this->event_map_->broadcast_collection ();

  if (proxys != 0)
    proxys->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_NS_EVENT_MAP_ENTRY_OBSERVER_T_CPP */
