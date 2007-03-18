/* -*- C++ -*- */
/**
 *  @file StructuredEvent.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */
#ifndef TAO_Notify_STRUCTUREDEVENT_H
#define TAO_Notify_STRUCTUREDEVENT_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/EventType.h"
#include "orbsvcs/CosNotificationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_StructuredEvent;

/**
 * @class TAO_Notify_StructuredEvent_No_Copy
 *
 * @brief StructuredEvent implementation.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_StructuredEvent_No_Copy : public TAO_Notify_Event
{
public:
  /// Constuctor
  TAO_Notify_StructuredEvent_No_Copy (const CosNotification::StructuredEvent& notification);

  /// Destructor
  virtual ~TAO_Notify_StructuredEvent_No_Copy ();

  /// marshal this event into a CDR buffer (for persistence)
  virtual void marshal (TAO_OutputCDR & cdr) const;

  CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter) const;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const;

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const;

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

  /// unmarshal this event from a CDR buffer (for persistence)
  /// \return the new event, or NULL if this is the wrong type of event.
  static TAO_Notify_StructuredEvent * unmarshal (TAO_InputCDR & cdr);

protected:
  /// returns a copy of this event allocated on the heap
  virtual TAO_Notify_Event * copy (void) const;

  /// Structured Event
  const CosNotification::StructuredEvent* notification_;

  /// Our type.
  const TAO_Notify_EventType type_;
};

/*****************************************************************************/

/**
 * @class TAO_Notify_StructuredEvent
 *
 * @brief StructuredEvent implementation.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredEvent : public TAO_Notify_StructuredEvent_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent& notification);

  /// Destructor
  virtual ~TAO_Notify_StructuredEvent ();


protected:
  /// Copy of the Event.
  CosNotification::StructuredEvent notification_copy;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDEVENT_H */
