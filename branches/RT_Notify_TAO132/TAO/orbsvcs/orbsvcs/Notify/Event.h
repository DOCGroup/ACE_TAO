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
#include "ace/Refcounted_Auto_Ptr.h"
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

  /// Copy the event.
  virtual TAO_NS_Event* copy (ACE_ENV_SINGLE_ARG_DECL) const = 0;

  /// Translate Any to Structured
  static void translate (const CORBA::Any& any, CosNotification::StructuredEvent& notification);

  /// Translate Structured to Any
  static void translate (const CosNotification::StructuredEvent& notification, CORBA::Any& any);

  /// Get the event type.
  virtual const TAO_NS_EventType& type (void) const = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const = 0;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const = 0;

  /// Push event to consumer
  virtual void push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  ///= Accessors
  /// Priority
  const TAO_NS_Property_Short& priority (void) const;

  /// Timeout
  const TAO_NS_Property_Time& timeout (void) const;

protected:
  /// = QoS properties

  /// Priority.
  TAO_NS_Property_Short priority_;

  /// Timeout.
  TAO_NS_Property_Time timeout_;
};

typedef ACE_Refcounted_Auto_Ptr<const TAO_NS_Event, TAO_SYNCH_MUTEX> TAO_NS_Event_var_Base;

/**
 * @class TAO_NS_Event_var
 *
 * @brief A Non-Copy version of the ACE_Refcounted_Auto_Ptr that hides the constructors.
 *
 */
class TAO_NS_Event_var : public TAO_NS_Event_var_Base
{
public:
  /// Default Constructor
  TAO_NS_Event_var (void);

protected:
  /// Constructor
  TAO_NS_Event_var (TAO_NS_Event* event);
};

/**
 * @class TAO_NS_Event
 *
 * @brief A version of the ACE_Refcounted_Auto_Ptr that allows construction from a TAO_NS_Event
 *
 */
class TAO_NS_Event_Copy_var : public TAO_NS_Event_var
{
public:
  /// Default Constructor
  TAO_NS_Event_Copy_var (void);

  /// Constructor
  TAO_NS_Event_Copy_var (TAO_NS_Event* event);
};

typedef ACE_Unbounded_Queue<TAO_NS_Event_var> TAO_NS_Event_Collection;

#if defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENT_H */
