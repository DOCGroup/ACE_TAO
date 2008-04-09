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

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Refcountable.h"
#include "orbsvcs/Notify/Property.h"
#include "orbsvcs/Notify/Property_Boolean.h"
#include "orbsvcs/Notify/Property_T.h"

#include "orbsvcs/Event_ForwarderS.h"
#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosNotificationC.h"

#include "ace/Copy_Disabled.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Consumer;
class TAO_Notify_EventType;

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
  typedef TAO_Notify_Refcountable_Guard_T<TAO_Notify_Event> Ptr;

  // Codes to distinguish marshaled events in persistent storage
  enum {MARSHAL_ANY=1,MARSHAL_STRUCTURED=2};
  /// Constuctor
  TAO_Notify_Event (void);

  /// Destructor
  virtual ~TAO_Notify_Event ();

  /// Translate Any to Structured
  static void translate (const CORBA::Any& any, CosNotification::StructuredEvent& notification);

  /// Translate Structured to Any
  static void translate (const CosNotification::StructuredEvent& notification, CORBA::Any& any);

  /// Get the event type.
  virtual const TAO_Notify_EventType& type (void) const = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter) const = 0;

  /// Convert to CosNotification::Structured type
  virtual void convert (CosNotification::StructuredEvent& notification) const = 0;

  /// Push event to consumer
  virtual void push (TAO_Notify_Consumer* consumer) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const = 0;

  /// Push event to the Event_Forwarder interface
  virtual void push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const = 0;

  /// Return a pointer to a copy of this event on the heap.  The
  /// event is not owned by the caller, so it should not be deleted or
  /// released.
  TAO_Notify_Event* queueable_copy (void) const;

  /// marshal this event into a CDR buffer (for persistence)
  virtual void marshal (TAO_OutputCDR& cdr) const = 0;

  /// Unmarshal an event from a CDR. (for persistence)
  static TAO_Notify_Event* unmarshal (TAO_InputCDR & cdr);

  ///= Accessors
  /// Priority
  const TAO_Notify_Property_Short& priority (void) const;

  /// Timeout
  const TAO_Notify_Property_Time& timeout (void) const;

  /// Reliable
  const TAO_Notify_Property_Boolean& reliable(void) const;

  /// Event creation time
  const ACE_Time_Value& creation_time (void) const;

protected:
  /// = QoS properties

  /// Priority.
  TAO_Notify_Property_Short priority_;

  /// Timeout.
  TAO_Notify_Property_Time timeout_;

  /// Reliability
  TAO_Notify_Property_Boolean reliable_;

private:
  /// Return a pointer to a copy of this event on the heap
  virtual TAO_Notify_Event* copy (void) const = 0;

  virtual void release (void);

  mutable Ptr clone_;
  bool        is_on_heap_;
  ACE_Time_Value time_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_NOTIFY_EVENT_H */
