/* -*- C++ -*- */
/**
 *  @file Event_Map_Observer_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_EVENT_MAP_OBSERVER_T_H
#define TAO_EVENT_MAP_OBSERVER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Types.h"
#include "Event_Map_Observer.h"
#include "orbsvcs/ESF/ESF_Worker.h"

/**
 * @class TAO_NS_Update_Added_Worker_T
 *
 * @brief Inform Peers of new offered type.
 *
 */
template <class PROXY>
class TAO_Notify_Export TAO_NS_Update_Added_Worker : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_NS_Update_Added_Worker (const TAO_NS_EventType& event_type, TAO_NS_Updates_Dispatch_Observer* dispatch_observer);

protected:
  ///= TAO_ESF_Worker method
  void work (PROXY* proxy ACE_ENV_ARG_DECL);

  const TAO_NS_EventType& event_type_;

  /// Updates Observer.
  TAO_NS_Updates_Dispatch_Observer* dispatch_observer_;
};

/*******************************************************************************************/


/**
 * @class TAO_NS_Update_Removed_Worker_T
 *
 * @brief Inform Peers of offered type removed.
 *
 */
template <class PROXY>
class TAO_Notify_Export TAO_NS_Update_Removed_Worker : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_NS_Update_Removed_Worker (const TAO_NS_EventType& event_type, TAO_NS_Updates_Dispatch_Observer* dispatch_observer);

protected:
  ///= TAO_ESF_Worker method
  void work (PROXY* proxy ACE_ENV_ARG_DECL);

  const TAO_NS_EventType& event_type_;

  /// Updates Observer.
  TAO_NS_Updates_Dispatch_Observer* dispatch_observer_;
};

/*******************************************************************************************/

/**
 * @class TAO_Event_Map_Observer_T
 *
 * @brief Observe significant changes to the Event Map.
 *
 */
template <class PROXY>
class TAO_Notify_Export TAO_NS_Event_Map_Observer_T : public TAO_NS_Event_Map_Observer
{
  typedef TAO_NS_Event_Map_T<PROXY, TAO_SYNCH_RW_MUTEX> EVENT_MAP;
  typedef TAO_NS_Update_Added_Worker<PROXY> UPDATE_ADDED_WORKER;
  typedef TAO_NS_Update_Removed_Worker<PROXY> UPDATE_REMOVED_WORKER;
  typedef TAO_ESF_Proxy_Collection<PROXY> PROXY_COLLECTION;

public:
  /// Constructor
  TAO_NS_Event_Map_Observer_T (void);

  /// Destructor
  virtual ~TAO_NS_Event_Map_Observer_T ();

  /// Set the inverse event map and the updates dispatch observer.
  void init (EVENT_MAP* event_map, TAO_NS_Updates_Dispatch_Observer* dispatch_observer);

  /// <event_type> was seen for the first time in the event map.
  virtual void type_added (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

  /// <event_type> does not have any proxy interested in it.
  virtual void type_removed (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL);

protected:
  /// Event Map
  EVENT_MAP* event_map_;

  /// Updates Observer.
  TAO_NS_Updates_Dispatch_Observer* dispatch_observer_;
};

#if defined (__ACE_INLINE__)
#include "Event_Map_Observer_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Event_Map_Observer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Map_Observer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_EVENT_MAP_OBSERVER_T_H */
