// $Id$

#ifndef TAO_NS_EVENT_MAP_T_C
#define TAO_NS_EVENT_MAP_T_C

#include "Event_Map_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Event_Map_Entry_T.h"
#include "Properties.h"
#include "Factory.h"
#include "Event_Map_Observer.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Event_Map_T, "$Id$")

template <class PROXY, class ACE_LOCK>
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::TAO_NS_Event_Map_T (void)
  :event_type_count_ (0), observer_ (0)
{

}

template <class PROXY, class ACE_LOCK>
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::~TAO_NS_Event_Map_T ()
{
}

template <class PROXY, class ACE_LOCK> void
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::init (ACE_ENV_SINGLE_ARG_DECL)
{
  this->broadcast_entry_.init (ACE_ENV_SINGLE_ARG_PARAMETER);
}


template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::insert (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  ENTRY* entry;

  int result = -1;

  if (event_type.is_special () == 1)
    {
      entry = &this->broadcast_entry_;
      result = 0;
    }
  else
    {
      ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

      result = this->map_.find (event_type, entry);
    }

  if (result == -1)
  {
    ACE_NEW_THROW_EX (entry,
                      ENTRY (),
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (-1);

    entry->init (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    entry->connected (proxy ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

    if (map_.bind (event_type, entry) == -1)
      ACE_THROW_RETURN (CORBA::NO_MEMORY (), -1);

    if (this->observer_ != 0)
      this->observer_->type_added (event_type ACE_ENV_ARG_PARAMETER);

    return ++event_type_count_;
  }
  else
    {
      entry->connected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);
      return ++event_type_count_;
    }
}

template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::remove (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  ENTRY* entry;

  int result = -1;

  if (event_type.is_special () == 1)
    {
      entry = &this->broadcast_entry_;
      result = 0;
    }
  else
    {
      ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

      result = this->map_.find (event_type, entry);
    }

  if (result == 0)
    {
      entry->disconnected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

      if (entry->count () == 0)
        {
          if (this->observer_ != 0)
            this->observer_->type_removed (event_type ACE_ENV_ARG_PARAMETER);

          /// @@TODO: Exec a strategy for removing entries.
          /// Strategy 1: remove_immediately
          /// Strategy 2: remove_bunch_after_threshold
          /// Strategy 3: use cached allocator and 1
        }

      return --event_type_count_;
    }

  return -1;
}

#endif /* TAO_NS_EVENT_MAP_T_C */
