/* -*- C++ -*- */
/**
 *  @file Event_Map_Observer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENT_MAP_OBSERVER_H
#define TAO_NS_EVENT_MAP_OBSERVER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_NS_Event_Map_Observer
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Event_Map_Observer
{
public:
  /// <event_type> was seen for the first time in the event map.
  virtual void type_added (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL) = 0;

  /// <event_type> does not have any proxy interested in it.
  virtual void type_removed (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NS_EVENT_MAP_OBSERVER_H */
