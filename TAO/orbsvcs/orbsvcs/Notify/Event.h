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
#include "orbsvcs/CosNotificationC.h"
#include "Property.h"
#include "Property_T.h"

class TAO_NS_Consumer;
class TAO_NS_EventType;

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

  /// Translate Any to Structured
  static void translate (const CORBA::Any& any, CosNotification::StructuredEvent& notification);

  /// Translate Structured to Any
  static void translate (const CosNotification::StructuredEvent& notification, CORBA::Any& any);

  /// Get the event type.
  virtual const TAO_NS_EventType& type (void) const = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) = 0;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) = 0;

  /// Push event to consumer
  virtual void push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) = 0;

  ///= Accessors
  /// Priority
  TAO_NS_Property_Short& priority (void);

  /// Timeout
  TAO_NS_Property_Time& timeout (void);

protected:
  /// = QoS properties

  /// Priority.
  TAO_NS_Property_Short priority_;

  /// Timeout.
  TAO_NS_Property_Time timeout_;
};

template <class X, class ACE_LOCK> class ACE_Refcounted_Auto_Ptr;
template <class T> class ACE_Unbounded_Queue;
typedef ACE_Refcounted_Auto_Ptr<TAO_NS_Event, TAO_SYNCH_MUTEX> TAO_NS_Event_var;
typedef ACE_Unbounded_Queue<TAO_NS_Event_var> TAO_NS_Event_Collection;

#if defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_H */
