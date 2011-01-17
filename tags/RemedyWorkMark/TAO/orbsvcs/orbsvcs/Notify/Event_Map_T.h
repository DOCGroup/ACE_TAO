/* -*- C++ -*- */
/**
 *  @file Event_Map_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENT_MAP_T_H
#define TAO_Notify_EVENT_MAP_T_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/CORBA_macros.h"

#include "orbsvcs/Notify/EventType.h"
#include "orbsvcs/Notify/Event_Map_Entry_T.h"
#include "orbsvcs/Notify/EventTypeSeq.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Event_Map_T
 *
 * @brief Template class for storing the collection of Proxys.
 *
 */
template <class PROXY, class ACE_LOCK>
class TAO_Notify_Event_Map_T
{

public:
  typedef  TAO_Notify_Event_Map_Entry_T<PROXY> ENTRY;

  /// Constructor
  TAO_Notify_Event_Map_T (void);

  /// Destructor
  ~TAO_Notify_Event_Map_T ();

  /// Init
  void init (void);

  /// Connect a PROXY
  void connect (PROXY* proxy);

  /// Disconnect a PROXY
  void disconnect (PROXY* proxy);

  /// Associate PROXY and event_type.
  /// Returns 1 if <event_type> is being seem for the 1st time otherwise returns 0.
  /// Returns -1 on error.
  int insert (PROXY* proxy, const TAO_Notify_EventType& event_type);

  /// Remove association of PROXY and event_type.
  /// Returns 1 if <event_type> is being seem for the last time otherwise returns 0.
  /// Returns -1 on error.
  int remove (PROXY* proxy, const TAO_Notify_EventType& event_type);

  /// Find the collection mapped to the <event_type>
  /// The usage_count on the entry returned is incremented.
  ENTRY* find (const TAO_Notify_EventType& event_type);

  /// Find the default broadcast list.
  typename ENTRY::COLLECTION* broadcast_collection (void);

  /// Find the update list. This is all the PROXYS connected to this Map.
  typename ENTRY::COLLECTION* updates_collection (void);

  /// Release the usage count on this entry.
  void release (ENTRY* entry);

  /// Access all the event types available
  const TAO_Notify_EventTypeSeq& event_types (void);

  /// Access number of proxys connected in all.
  int proxy_count (void);

protected:
  /// The Map that stores eventtype to entry mapping.
  ACE_Hash_Map_Manager <TAO_Notify_EventType, ENTRY*, ACE_SYNCH_NULL_MUTEX> map_;

  /// The lock to use.
  ACE_LOCK lock_;

  /// Count of proxys connected.
  int proxy_count_;

  /// The default broadcast list for EventType::special.
  ENTRY broadcast_entry_;

  /// Update Entry - Keeps a list of all PROXY's connected to this Map. Updates are send to this list.
  ENTRY updates_entry_;

  /// The event types that are available in this map.
  TAO_Notify_EventTypeSeq event_types_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Event_Map_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Map_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_EVENT_MAP_T_H */
