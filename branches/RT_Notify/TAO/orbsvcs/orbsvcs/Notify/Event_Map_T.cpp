// $Id$

#ifndef TAO_NS_EVENT_MAP_T_C
#define TAO_NS_EVENT_MAP_T_C

#include "Event_Map_T.h"
#include "Properties.h"
#include "Factory.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#if ! defined (__ACE_INLINE__)
#include "Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Event_Map_T, "$Id$")

template <class PROXY>
TAO_NS_Event_Map_Entry_T<PROXY>::TAO_NS_Event_Map_Entry_T (void)
  : collection_ (0), count_ (0)
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

/*****************************************************************************/

template <class PROXY, class ACE_LOCK>
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::TAO_NS_Event_Map_T (void)
  :count_ (0)
{

}

template <class PROXY, class ACE_LOCK>
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::~TAO_NS_Event_Map_T ()
{
}

template <class PROXY, class ACE_LOCK> TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::COLLECTION* 
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::create_entry (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  TAO_NS_Event_Map_Entry_T<PROXY>* entry;

  int result = -1;

  {
    ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);
    
    result = this->map_.find (event_type, entry);
  }

  if (result == -1)
    {
      ACE_NEW_THROW_EX (entry,
			ENTRY (),
			CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      entry->init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

      if (map_.bind (event_type, entry) == -1)
	ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);
  }

  return entry->collection ();
}

template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::insert (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  TAO_NS_Event_Map_Entry_T<PROXY>* entry;

  int result = -1;

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

    return ++count_;
  }
  else
    {
      entry->connected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);
      return ++count_;
    }
}

template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::remove (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  TAO_NS_Event_Map_Entry_T<PROXY>* entry;

  int result = -1;
  {
    ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);

    result = this->map_.find (event_type, entry);
  }

  if (result == 0)
    {
      entry->disconnected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_WRITE_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, -1);            
      return --count_;

      if (entry->count () == 0)
	{
	  /// @@TODO: Exec a strategy for removing entries.
	}
    }

  return -1;
}

template <class PROXY, class ACE_LOCK> int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::count (void)
{
  return this->count_;
}

#endif /* TAO_NS_EVENT_MAP_T_C */

