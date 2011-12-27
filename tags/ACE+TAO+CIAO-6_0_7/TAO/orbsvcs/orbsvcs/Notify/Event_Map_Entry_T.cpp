// $Id$

#ifndef TAO_Notify_EVENT_MAP_ENTRY_CPP
#define TAO_Notify_EVENT_MAP_ENTRY_CPP

#include "orbsvcs/Notify/Event_Map_Entry_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event_Map_Entry_T.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY>
TAO_Notify_Event_Map_Entry_T<PROXY>::TAO_Notify_Event_Map_Entry_T (void)
  : collection_ (0), count_ (0), usage_count_ (1)
{
}

template <class PROXY>
TAO_Notify_Event_Map_Entry_T<PROXY>::~TAO_Notify_Event_Map_Entry_T ()
{
  delete collection_;
}

template <class PROXY> void
TAO_Notify_Event_Map_Entry_T<PROXY>::init (void)
{
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  factory->create (collection_);
}

template <class PROXY> void
TAO_Notify_Event_Map_Entry_T<PROXY>::connected (PROXY* proxy)
{
  this->collection_->connected (proxy);
  ++count_;
}

template <class PROXY> void
TAO_Notify_Event_Map_Entry_T<PROXY>::disconnected (PROXY* proxy)
{
  this->collection_->disconnected (proxy);
  --count_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_EVENT_MAP_ENTRY_CPP */
