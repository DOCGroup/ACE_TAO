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

#ifndef TAO_NOTIFY_EVENT_H
#define TAO_NOTIFY_EVENT_H

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Copy_Disabled.h"

#include "Refcountable.h"
#include "Refcountable_Guard_T.h"

#include "orbsvcs/Event_ForwarderS.h"
#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosNotificationC.h"

#include "Property.h"
#include "Property_Boolean.h"
#include "Property_T.h"

class TAO_Notify_Consumer;
class TAO_Notify_EventType;
class TAO_Notify_Event;

typedef TAO_Notify_Refcountable_Guard_T<TAO_Notify_Event> TAO_Notify_Event_var_Base;

/**
 * @class TAO_Notify_Event_var
 *
 * @brief A Non-Copy version of the smart pointer that hides the constructors.
 *
 */
class TAO_Notify_Event_var : public TAO_Notify_Event_var_Base
{
public:
  /// Default Constructor
  TAO_Notify_Event_var (void);

protected:
  /// Constructor
  TAO_Notify_Event_var (const TAO_Notify_Event* event);
};

/**
 * @class TAO_Notify_Event
 *
 * @brief A smart pointer that allows construction from a TAO_Notify_Event
 *
 */
class TAO_Notify_Event_Copy_var : public TAO_Notify_Event_var
{
public:
  /// Default Constructor
  TAO_Notify_Event_Copy_var (void);

  /// Constructor
  TAO_Notify_Event_Copy_var (const TAO_Notify_Event* event);
};

/**
 * @class TAO_Notify_Event
 *
 * @brief Base class abstraction for Events flowing through the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Event
    : public TAO_Notify_Refcountable
    , private ACE_Copy_Disabled
{
public:
  // Codes to distinguish marshaled events in persistent storage
  enum {MARSHAL_ANY=1,MARSHAL_STRUCTURED=2};
  /// Constuctor
  TAO_Notify_Event (void);

  /// Destructor
  virtual ~TAO_Notify_Event ();

  virtual void release (void);

  /// Translate Any to Structured
  static void translate (const CORBA::Any& any, CosNotification::StructuredEvent& notification);

  /// Translate Structured to Any
  static void translate (const CosNotification::StructuredEvent& notification, CORBA::Any& any);

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const = 0;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const = 0;

  /// Push event to consumer
  virtual void push (TAO_Notify_Consumer* consumer ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const = 0;

  /// Return a pointer to a copy of this event on the heap
  void queueable_copy (TAO_Notify_Event_var & ptr ACE_ENV_ARG_DECL) const;

  /// marshal this event into a CDR buffer (for persistence)
  virtual void marshal (TAO_OutputCDR & cdr) const = 0;

  /// Unmarshal an event from a CDR. (for persistence)
  static TAO_Notify_Event * unmarshal (TAO_InputCDR & cdr);

  ///= Accessors
  /// Priority
  const TAO_Notify_Property_Short& priority (void) const;

  /// Timeout
  const TAO_Notify_Property_Time& timeout (void) const;

  /// Reliable
  const TAO_Notify_Property_Boolean& reliable(void) const;

protected:

  /// Return a pointer to a copy of this event on the heap
  virtual TAO_Notify_Event * copy (ACE_ENV_SINGLE_ARG_DECL) const = 0;

  /// = QoS properties

  /// Priority.
  TAO_Notify_Property_Short priority_;

  /// Timeout.
  TAO_Notify_Property_Time timeout_;

  /// Reliability
  TAO_Notify_Property_Boolean reliable_;

  TAO_Notify_Event * event_on_heap_;
};

#if defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_NOTIFY_EVENT_H */
