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

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../Event.h"
#include "../EventType.h"
#include "orbsvcs/CosNotificationC.h"

class TAO_Notify_Consumer;

/**
 * @class TAO_Notify_AnyEvent
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_Notify_AnyEvent_No_Copy : public TAO_Notify_Event
{
  friend class TAO_Notify_Builder;
public:
  /// Constuctor
  TAO_Notify_AnyEvent_No_Copy (const CORBA::Any &event);

  /// Destructor
  ~TAO_Notify_AnyEvent_No_Copy ();

  /// Copy the event.
  virtual TAO_Notify_Event* copy (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const;

  CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const;

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
  /// Any Event
  const CORBA::Any* event_;

  /// Our event type.
  static TAO_Notify_EventType event_type_;
};

/*****************************************************************************************************/

/**
 * @class TAO_Notify_AnyEvent
 *
 * @brief AnyEvent implementation.
 *
 */
class TAO_Notify_Export TAO_Notify_AnyEvent : public TAO_Notify_AnyEvent_No_Copy
{
public:
  /// Constuctor
  TAO_Notify_AnyEvent (const CORBA::Any &event);

  /// Destructor
  ~TAO_Notify_AnyEvent ();

protected:
  /// Copy of the Event.
  CORBA::Any any_copy_;
};

#if defined (__ACE_INLINE__)
#include "AnyEvent.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_ANYEVENT_H */
