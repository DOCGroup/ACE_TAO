/* -*- C++ -*- */
/**
 *  @file AnyEvent.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_ANYEVENT_H
#define TAO_Notify_ANYEVENT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/EventType.h"

#include "orbsvcs/CosNotificationC.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Consumer;
class TAO_Notify_AnyEvent;
/**
 * @class TAO_Notify_AnyEvent
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_AnyEvent_No_Copy : public TAO_Notify_Event
{
  friend class TAO_Notify_Builder;
public:
  /// Constructor
  TAO_Notify_AnyEvent_No_Copy (const CORBA::Any &event);

  /// Destructor
  virtual ~TAO_Notify_AnyEvent_No_Copy ();

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const;

  CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter) const;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const;

  /// Push event to consumer
  virtual void push (TAO_Notify_Consumer* consumer) const;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const;

  /// marshal this event into a CDR buffer (for persistence)
  virtual void marshal (TAO_OutputCDR & cdr) const;

  /// unmarshal this event from a CDR buffer (for persistence)
  /// \param code a code indicating what type of event to create.
  /// \param cdr a CDR stream containing the marshalled data for the event.
  /// \return the new event, or NULL if this is the wrong type of event.
  static TAO_Notify_AnyEvent * unmarshal (TAO_InputCDR & cdr);

protected:
  /// returns a copy of this event allocated from the heap
  virtual TAO_Notify_Event * copy (void) const;

protected:

  // This must be a pointer rather than a reference, because the derived
  // class needs to redirect it to a new copy
  const CORBA::Any* event_;

  /// Our event type.
  static TAO_Notify_EventType event_type_;
};

/****************************************************************************/

/**
 * @class TAO_Notify_AnyEvent
 *
 * @brief AnyEvent implementation.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_AnyEvent : public TAO_Notify_AnyEvent_No_Copy
{
public:
  /// Constructor
  TAO_Notify_AnyEvent (const CORBA::Any &event);

  /// Destructor
  virtual ~TAO_Notify_AnyEvent ();

protected:
  /// Copy of the Event.
  CORBA::Any any_copy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_ANYEVENT_H */
