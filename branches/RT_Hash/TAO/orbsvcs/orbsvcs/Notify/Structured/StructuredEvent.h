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
#ifndef TAO_NS_STRUCTUREDEVENT_H
#define TAO_NS_STRUCTUREDEVENT_H

#include "ace/pre.h"
#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../Event.h"
#include "../EventType.h"
#include "orbsvcs/CosNotificationC.h"

/**
 * @class TAO_NS_StructuredEvent
 *
 * @brief StructuredEvent implementation.
 *
 */

class TAO_Notify_Export TAO_NS_StructuredEvent : public TAO_NS_Event
{
public:
  /// Constuctor
  TAO_NS_StructuredEvent (const CosNotification::StructuredEvent& notification);

  /// Destructor
  ~TAO_NS_StructuredEvent ();

  CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL);

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification);

  /// Get the event type.
  virtual const TAO_NS_EventType& type (void) const;

  /// Push event to consumer
  virtual void push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL);

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL);

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL);

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL);

protected:
  /// Structured Event
  CosNotification::StructuredEvent notification_;

  /// Our type.
  const TAO_NS_EventType type_;
};

#if defined (__ACE_INLINE__)
#include "StructuredEvent.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDEVENT_H */
