/* -*- C++ -*- */
/**
 *  @file Event.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENT_H
#define TAO_NS_EVENT_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Copy_Disabled.h"
#include "orbsvcs/Event_ForwarderS.h"
#include "orbsvcs/CosNotifyFilterC.h"

class TAO_NS_EventType;
class TAO_NS_Consumer;

/**
 * @class TAO_NS_Event
 *
 * @brief Base class abstraction for Events flowing through the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_Event : private ACE_Copy_Disabled
{
public:
  /// Constuctor
  TAO_NS_Event (void);

  /// Destructor
  virtual ~TAO_NS_Event ();

  /// Get the event type.
  virtual const TAO_NS_EventType& type (void) const = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) = 0;

  /// Push event to consumer
  virtual void push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Notify_Internal::Event_Forwarder_ptr forwarder ACE_ENV_ARG_DECL) = 0;

    /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Notify_Internal::Event_Forwarder_ptr forwarder ACE_ENV_ARG_DECL) = 0;
};

#if defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_H */
