/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Event.h
//
// = DESCRIPTION
//   Abstraction for Notify's event types.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NOTIFY_EVENT_H
#define TAO_NOTIFY_EVENT_H

#include "ace/pre.h"
#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/TimeBaseC.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosEventCommC.h"

// @@ Pradeep: this is another case of multiple classes bunched in the
// same file, you should really think about separating it.

class TAO_Notify_Export TAO_Notify_EventType
{
  // = TITLE
  //   TAO_Notify_EventType
  //
  // = DESCRIPTION
  //   This type is used to compare different event types.
  //   It is used by the Event Manager as a key to find subscription lists.
  //
public:
  // = Initialization and termination
  TAO_Notify_EventType (void);
  TAO_Notify_EventType (const char* domain_name, const char* type_name);
  TAO_Notify_EventType (const CosNotification::EventType& event_type);
  // Constuctor

  ~TAO_Notify_EventType ();
  // Destructor

  u_long hash (void) const;
  // hash value

  void operator=(const CosNotification::EventType& event_type);
  // Assignment from CosNotification::EventType

  int operator==(const TAO_Notify_EventType& notify_event_type) const;
  // == operator

  static TAO_Notify_EventType& special_event_type (void);
  // Return the special event type.

  CORBA::Boolean is_special (void) const;
  // Is this the special event (accept everything).

  const CosNotification::EventType& get_native (void) const;
  // Get the type underneath us.

protected:
  void recompute_hash (void);
  // Recompute the hash value.

  // = Data Members
  CosNotification::EventType event_type_;
  // The event_type that we're decorating.

  u_long hash_value_;
  // The hash value computed.

  static TAO_Notify_EventType special_event_type_;
  // A special event type
};

// ****************************************************************
// @@ Pradeep: please remember to separate your classes with a line
// like the one above.  Or better yet, do not put multiple classes in
// the same file.

class TAO_Notify_Export TAO_Notify_Event
{
  // = TITLE
  //   TAO_Notify_Event
  //
  // = DESCRIPTION
  //   Abstraction for an event
  //   This class allows us to treat event types homogenously.
  //   Derived types for anys and structured events provide the implementation.
  //   This the the "prototype" creational pattern.
  //
public:
  TAO_Notify_Event (void);
  // The lock for its ref. count.
  // Owns the lock.

  virtual ~TAO_Notify_Event ();

  virtual CORBA::Boolean is_special_event_type (void) const = 0;
  // Is this the "special" event type.

  virtual const TAO_Notify_EventType& event_type (void) const = 0;
  // Get the event type.

  virtual TAO_Notify_Event* clone (void) = 0;
  // We may need to make a copy of the underlying data if it is not owned
  // by us.
  // Note this behaviour: If this object owns the data, then we *transfer*
  // ownership of the data to the new object otherwise we copy the data
  // for the new object.

  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter TAO_ENV_ARG_DECL) const = 0;
  // Returns true if the filter matches.

  virtual void do_push (CosEventComm::PushConsumer_ptr consumer TAO_ENV_ARG_DECL) const = 0;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer TAO_ENV_ARG_DECL) const = 0;
  // Push self to <consumer>

  // = QoS Properties.
  //

  CORBA::Short event_reliability (void);
  void event_reliability (CORBA::Short event_reliability);
  // Not implemented.

  CORBA::Short priority (void);
  void priority (CORBA::Short priority);
  // Event priority

  TimeBase::UtcT start_time (void);
  void start_time (TimeBase::UtcT start_time);
  // Earliest delivery time.

  TimeBase::UtcT stop_time (void);
  void stop_time (TimeBase::UtcT stop_time);
  // Latest absolute expiry time for this event.

  TimeBase::TimeT timeout (void);
  void timeout (TimeBase::TimeT timeout);
  // Relative expiry time.

  // = Refcounted lifetime
  void _incr_refcnt (void);
  void _decr_refcnt (void);

 protected:
  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  // = QoS properties
  CORBA::Short event_reliability_;
  CORBA::Short priority_;
  TimeBase::UtcT start_time_;
  TimeBase::UtcT stop_time_;
  TimeBase::TimeT timeout_;
};

// ****************************************************************

class TAO_Notify_Export TAO_Notify_Any : public TAO_Notify_Event
{
  // = TITLE
  //   TAO_Notify_Any
  //
  // = DESCRIPTION
  //   This class is the concrete prototype for the Any type.
  //

public:
  TAO_Notify_Any (CORBA::Any* data);
  // Refers to the data. Owns it!

  TAO_Notify_Any (const CORBA::Any* data);
  // Does not own data.

  virtual ~TAO_Notify_Any ();

  virtual TAO_Notify_Event* clone (void);

  void operator=(const TAO_Notify_Any& notify_any);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter TAO_ENV_ARG_DECL) const;
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer TAO_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer TAO_ENV_ARG_DECL) const;

protected:
  CORBA::Any* data_;
  // The data

  CORBA::Boolean is_owner_;
  // Do we own the data.
};

// ****************************************************************

class TAO_Notify_Export TAO_Notify_StructuredEvent : public TAO_Notify_Event
{
  // = TITLE
  //   TAO_Notify_StructuredEvent
  //
  // = DESCRIPTION
  //   This class is the concrete prototype for the Structured Event Type.
  //
public:
  TAO_Notify_StructuredEvent (CosNotification::StructuredEvent * notification);
  TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent * notification);
  virtual ~TAO_Notify_StructuredEvent ();

  virtual TAO_Notify_Event* clone (void);
  void operator=(const TAO_Notify_StructuredEvent &structured_event);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter TAO_ENV_ARG_DECL) const;
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer TAO_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer TAO_ENV_ARG_DECL) const;

protected:

  void init_QoS (void);
  // Load the QoS properties specified for this event from <data_>.

  // = Data Members
  CosNotification::StructuredEvent* data_;
  // The data

  TAO_Notify_EventType event_type_;
  // The event type of <data_>

  CORBA::Boolean is_owner_;
  // Do we own the data.
};


#if defined (__ACE_INLINE__)
#include "Notify_Event.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_NOTIFY_EVENT_H */
