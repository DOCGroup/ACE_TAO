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
#include "ace/Copy_Disabled.h"
#include "Types.h"
#include "EventType.h"

template <class PROXY, class ACE_LOCK> class TAO_NS_Event_Map_T;

/**
 * @class TAO_NS_Event_Map_Entry_T
 *
 * @brief The entry stored in the event map.
 *
 */
template <class PROXY>
class TAO_NS_Event_Map_Entry_T : private ACE_Copy_Disabled
{
public:
  typedef TAO_ESF_Proxy_Collection<PROXY> COLLECTION;

  /// Constructor
  TAO_NS_Event_Map_Entry_T (void);

  /// Destructor
  ~TAO_NS_Event_Map_Entry_T (void);

  /// Init - Allocates collection
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Connect
  void connected (PROXY* proxy ACE_ENV_ARG_DECL);

  /// Disconnect
  void disconnected (PROXY* proxy ACE_ENV_ARG_DECL);

  /// Collection accessor
  COLLECTION* collection (void);

  /// Count accessor
  int count (void);
  
protected:
  /// The Collection 
  COLLECTION* collection_;

  /// Count of PROXY's connected in the collection;
  int count_;
};

/**
 * @class TAO_NS_Event_Map_T
 *
 * @brief Template class for storing the collection of Proxys.
 *
 */
template <class PROXY, class ACE_LOCK>
class TAO_NS_Event_Map_T
{
  typedef TAO_NS_Event_Map_Entry_T<PROXY> ENTRY;

public:
  typedef TAO_ESF_Proxy_Collection<PROXY> COLLECTION;
  /// Constuctor
  TAO_NS_Event_Map_T (void);

  /// Destructor
  ~TAO_NS_Event_Map_T ();  

  /// An entry can be precreated for an event_type. else it is created when required (during insert). 
  COLLECTION* create_entry (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Associate PROXY and event_type. returns count of PROXYs.
  int insert (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Remove association of PROXY and event_type. returns count of PROXYs.
  int remove (PROXY* proxy, const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Find the collection mapped mapped to the <event_type> 
  COLLECTION* find (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// Access count
  int count (void);

protected:
  /// The Map that stores eventtype to entry mapping.
  ACE_Hash_Map_Manager <TAO_NS_EventType, ENTRY*, ACE_SYNCH_NULL_MUTEX> map_;

  /// The lock to use.
  ACE_LOCK lock_;

  /// Count of items entered in the map.
  int count_;
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
