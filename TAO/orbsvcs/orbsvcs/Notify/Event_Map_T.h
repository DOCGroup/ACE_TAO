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

#ifndef TAO_NS_EVENT_MAP_T_H
#define TAO_NS_EVENT_MAP_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "Types.h"
#include "EventType.h"
#include "Event_Map_Entry_T.h"

class TAO_NS_Event_Map_Observer;

/**
 * @class TAO_NS_Event_Map_T
 *
 * @brief Template class for storing the collection of Proxys.
 *
 */
template <class PROXY, class ACE_LOCK>
class TAO_NS_Event_Map_T
{

public:
  typedef ACE_TYPENAME TAO_NS_Event_Map_Entry_T<PROXY> ENTRY;
  
  /// Constuctor
  TAO_NS_Event_Map_T (void);

  /// Destructor
  ~TAO_NS_Event_Map_T ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Attach an Observer.
  void attach_observer (TAO_NS_Event_Map_Observer* observer);

  /// Associate PROXY and event_type. returns count of PROXYs.
  int insert (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Remove association of PROXY and event_type. returns count of PROXYs.
  int remove (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Find the collection mapped to the <event_type>
  ACE_TYPENAME ENTRY::COLLECTION* find (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Find the default broadcast list.
  ACE_TYPENAME ENTRY::COLLECTION* broadcast_collection (void);

  /// Access count, number of different event types in the map.
  int event_type_count (void);

protected:
  /// The Map that stores eventtype to entry mapping.
  ACE_Hash_Map_Manager <TAO_NS_EventType, ENTRY*, ACE_SYNCH_NULL_MUTEX> map_;

  /// The lock to use.
  ACE_LOCK lock_;

  /// Count of items entered in the map.
  int event_type_count_;

  /// The default broadcast list for EventType::special.
  ENTRY broadcast_entry_;

  /// Observer attached to us.
  TAO_NS_Event_Map_Observer* observer_;
};

#if defined (__ACE_INLINE__)
#include "Event_Map_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Event_Map_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Map_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_MAP_T_H */
