// $Id$

#ifndef TAO_NS_EVENT_MAP_ENTRY_CPP
#define TAO_NS_EVENT_MAP_ENTRY_CPP

#include "Event_Map_Entry_T.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Map_Entry_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Event_Map_Entry_T, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Properties.h"
#include "Factory.h"

template <class PROXY>
TAO_NS_Event_Map_Entry_T<PROXY>::TAO_NS_Event_Map_Entry_T (void)
  : collection_ (0), count_ (0), usage_count_ (1)
{
}

template <class PROXY>
TAO_NS_Event_Map_Entry_T<PROXY>::~TAO_NS_Event_Map_Entry_T ()
{
  delete collection_;
}

template <class PROXY> void
TAO_NS_Event_Map_Entry_T<PROXY>::init (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  factory->create (collection_ ACE_ENV_ARG_PARAMETER);
}

template <class PROXY> void
TAO_NS_Event_Map_Entry_T<PROXY>::connected (PROXY* proxy ACE_ENV_ARG_DECL)
{
  this->collection_->connected (proxy ACE_ENV_ARG_PARAMETER);
  ++count_;
}

template <class PROXY> void
TAO_NS_Event_Map_Entry_T<PROXY>::disconnected (PROXY* proxy ACE_ENV_ARG_DECL)
{
  this->collection_->disconnected (proxy ACE_ENV_ARG_PARAMETER);
  --count_;
}

#endif /* TAO_NS_EVENT_MAP_ENTRY_CPP */
