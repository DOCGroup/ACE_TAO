// $Id$

#ifndef TAO_Notify_EVENT_MAP_T_CPP
#define TAO_Notify_EVENT_MAP_T_CPP

#include "orbsvcs/Notify/Event_Map_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/Notify/Event_Map_Entry_T.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY, class ACE_LOCK>
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::TAO_Notify_Event_Map_T (void)
  :proxy_count_ (0)
{

}

template <class PROXY, class ACE_LOCK>
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::~TAO_Notify_Event_Map_T ()
{
}

template <class PROXY, class ACE_LOCK> void
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::init (void)
{
  this->broadcast_entry_.init ();

  this->updates_entry_.init ();
}

template <class PROXY, class ACE_LOCK> void
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::connect (PROXY* proxy)
{
  this->updates_entry_.connected (proxy);

  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);
  ++this->proxy_count_;
}

template <class PROXY, class ACE_LOCK> void
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::disconnect (PROXY* proxy)
{
  this->updates_entry_.disconnected (proxy);

  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);
  --this->proxy_count_;
}

template <class PROXY, class ACE_LOCK> int
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::insert (PROXY* proxy, const TAO_Notify_EventType& event_type)
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

  if (result == -1) // This type is being seen for the first time.
  {
    ACE_NEW_THROW_EX (entry,
                      ENTRY (),
                      CORBA::NO_MEMORY ());

    entry->init ();

    entry->connected (proxy);

    ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

    if (map_.bind (event_type, entry) == -1)
      throw CORBA::NO_MEMORY ();

    if (this->event_types_.insert (event_type) == -1)
      return -1;

    return 1;
  }
  else // Add to existing entry or the broadcast entry.
    {
      entry->connected (proxy);
    }

  return 0;
}

template <class PROXY, class ACE_LOCK> int
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::remove (PROXY* proxy, const TAO_Notify_EventType& event_type)
{
  ENTRY* entry = 0;

  if (event_type.is_special () == 1)
    {
      entry = &this->broadcast_entry_;

      entry->disconnected (proxy);
    }
  else
    {
      int result = -1;

      {
        ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

        result = this->map_.find (event_type, entry);
      }

      if (result == 0)
        {
          entry->disconnected (proxy);

          if (entry->count () == 0)
            {
              /// Exec a strategy for removing entries.
              /// Strategy 1: remove_immediately
              /// Strategy 2: remove a bunch_after crossing a threshold
              /// Strategy 3: use cached allocator and 1

              // Strategy 1:
              ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

              this->map_.unbind (event_type);

              if (entry->_decr_refcnt () == 0)
                delete entry;

              if (this->event_types_.remove (event_type) == -1)
                return -1;

              return 1;
            }
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_EVENT_MAP_T_CPP */
