// $Id$

#ifndef TAO_NS_EVENT_MAP_T_C
#define TAO_NS_EVENT_MAP_T_C

#include "Event_Map_T.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Event_Map_Entry_T.h"
#include "Properties.h"
#include "Factory.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Event_Map_T, "$Id$")

template <class PROXY, class ACE_LOCK>
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::TAO_NS_Event_Map_T (void)
  :proxy_count_ (0)
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
  ACE_CHECK;

  this->updates_entry_.init (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class PROXY, class ACE_LOCK> void
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::connect (PROXY* proxy ACE_ENV_ARG_DECL)
{
  this->updates_entry_.connected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);
  ++this->proxy_count_;
}

template <class PROXY, class ACE_LOCK> void
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::disconnect (PROXY* proxy ACE_ENV_ARG_DECL)
{
  this->updates_entry_.disconnected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);
  --this->proxy_count_;
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

  if (result == -1) // This type is being seen for the first time.
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

    if (this->event_types_.insert (event_type) == -1)
      return -1;

    return 1;
  }
  else // Add to existing entry or the broadcast entry.
    {
      entry->connected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::remove (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  ENTRY* entry;

  if (event_type.is_special () == 1)
    {
      entry = &this->broadcast_entry_;

      entry->disconnected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
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
          entry->disconnected (proxy ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

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

#endif /* TAO_NS_EVENT_MAP_T_C */
