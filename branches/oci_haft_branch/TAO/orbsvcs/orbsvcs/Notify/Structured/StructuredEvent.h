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

#include "ace/pre.h"
#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../Event.h"
#include "../EventType.h"
#include "orbsvcs/CosNotificationC.h"

/**
 * @class TAO_Notify_StructuredEvent_No_Copy
 *
 * @brief StructuredEvent implementation.
 *
 */

class TAO_Notify_Export TAO_Notify_StructuredEvent_No_Copy : public TAO_Notify_Event
{
public:
  /// Constuctor
  TAO_Notify_StructuredEvent_No_Copy (const CosNotification::StructuredEvent& notification);

  /// Destructor
  ~TAO_Notify_StructuredEvent_No_Copy ();

  /// Copy the event.
  virtual TAO_Notify_Event* copy (ACE_ENV_SINGLE_ARG_DECL) const;

  CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const;

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const;

  /// Push event to consumer
  virtual void push (TAO_Notify_Consumer* consumer ACE_ENV_ARG_DECL) const;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const;

protected:
  /// Structured Event
  const CosNotification::StructuredEvent* notification_;

  /// Our type.
  const TAO_Notify_EventType type_;
};

/*****************************************************************************************************/

/**
 * @class TAO_Notify_StructuredEvent
 *
 * @brief StructuredEvent implementation.
 *
 */
class TAO_Notify_Export TAO_Notify_StructuredEvent : public TAO_Notify_StructuredEvent_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent& notification);

  /// Destructor
  ~TAO_Notify_StructuredEvent ();

protected:
  /// Copy of the Event.
  CosNotification::StructuredEvent notification_copy;
};

#if defined (__ACE_INLINE__)
#include "StructuredEvent.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDEVENT_H */
