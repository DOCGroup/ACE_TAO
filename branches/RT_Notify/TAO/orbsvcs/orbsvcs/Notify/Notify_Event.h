/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Event.h
 *
 *  $Id$
 *
 * Abstraction for Notify's event types.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

// Needed for the SequencePushConsumer
class TAO_Notify_QoSAdmin_i;

// @@ Pradeep: this is another case of multiple classes bunched in the
// same file, you should really think about separating it.

/**
 * @class TAO_Notify_EventType
 *
 * @brief TAO_Notify_EventType
 *
 * This type is used to compare different event types.
 * It is used by the Event Manager as a key to find subscription lists.
 */
class TAO_Notify_Export TAO_Notify_EventType
{
public:
  // = Initialization and termination
  /// Constuctor
  TAO_Notify_EventType (void);
  TAO_Notify_EventType (const char* domain_name, const char* type_name);
  TAO_Notify_EventType (const CosNotification::EventType& event_type);

  /// Destructor
  ~TAO_Notify_EventType ();

  /// Hash value
  u_long hash (void) const;

  /// Assignment from CosNotification::EventType
  void operator=(const CosNotification::EventType& event_type);
  
  /// Assignment from CosNotification::EventType
  void operator=(CosNotification::EventType& event_type);

  /// == operator
  int operator==(const TAO_Notify_EventType& notify_event_type) const;

  /// Return the special event type.
  static TAO_Notify_EventType& special_event_type (void);

  /// Is this the special event (accept everything).
  CORBA::Boolean is_special (void) const;

  /// Get the type underneath us.
  const CosNotification::EventType& get_native (void) const;

protected:
  /// Init this object.
  void init_i (const char* domain_name, const char* type_name);
  
  /// Recompute the hash value.
  void recompute_hash (void);

  // = Data Members
  /// The event_type that we're decorating.
  CosNotification::EventType event_type_;

  /// The hash value computed.
  u_long hash_value_;

  /// A special event type
  static TAO_Notify_EventType special_event_type_;
};

// ****************************************************************
// @@ Pradeep: please remember to separate your classes with a line
// like the one above.  Or better yet, do not put multiple classes in
// the same file.

/**
 * @class TAO_Notify_Event
 *
 * @brief TAO_Notify_Event
 *
 * Abstraction for an event
 * This class allows us to treat event types homogenously.
 * Derived types for anys and structured events provide the implementation.
 * This the the "prototype" creational pattern.
 */
class TAO_Notify_Export TAO_Notify_Event
{
public:
  /// The lock for its ref. count.
  /// Owns the lock.
  TAO_Notify_Event (void);

  virtual ~TAO_Notify_Event ();

  /// Is this the "special" event type.
  virtual CORBA::Boolean is_special_event_type (void) const = 0;

  /// Get the event type.
  virtual const TAO_Notify_EventType& event_type (void) const = 0;

  /**
   * We may need to make a copy of the underlying data if it is not owned
   * by us.
   * Note this behaviour: If this object owns the data, then we *transfer*
   * ownership of the data to the new object otherwise we copy the data
   * for the new object.
   */
  virtual TAO_Notify_Event* clone (void) = 0;

  /// Returns true if the filter matches.
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) = 0;

  /// Push self to <consumer>
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer ACE_ENV_ARG_DECL) const = 0;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer ACE_ENV_ARG_DECL) const = 0;
  virtual void do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                        const TAO_Notify_QoSAdmin_i& qos_admin,
                        CosNotification::EventBatch& unsent,
                        int flush_queue
                        ACE_ENV_ARG_DECL) const = 0;

  // = QoS Properties.
  //

  /// Not implemented.
  CORBA::Short event_reliability (void);
  void event_reliability (CORBA::Short event_reliability);

  /// Get the event priority
  CORBA::Short priority (void);

  /// Set the event priority
  void priority (CORBA::Short priority);

  /// Earliest delivery time.
  TimeBase::UtcT start_time (void);
  void start_time (TimeBase::UtcT start_time);

  /// Latest absolute expiry time for this event.
  TimeBase::UtcT stop_time (void);
  void stop_time (TimeBase::UtcT stop_time);

  /// Relative expiry time.
  TimeBase::TimeT timeout (void);
  void timeout (TimeBase::TimeT timeout);

  // = Refcounted lifetime
  void _incr_refcnt (void);
  void _decr_refcnt (void);

protected:
  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  // = QoS properties
  CORBA::Short event_reliability_;
  CORBA::Short priority_;
  TimeBase::UtcT start_time_;
  TimeBase::UtcT stop_time_;
  TimeBase::TimeT timeout_;
};

// ****************************************************************

/**
 * @class TAO_Notify_Any
 *
 * @brief TAO_Notify_Any
 *
 * This class is the concrete prototype for the Any type.
 */
class TAO_Notify_Export TAO_Notify_Any : public TAO_Notify_Event
{

public:
  /// Refers to the data. Owns it!
  TAO_Notify_Any (CORBA::Any* data);

  /// Does not own data.
  TAO_Notify_Any (const CORBA::Any* data);

  virtual ~TAO_Notify_Any ();

  virtual TAO_Notify_Event* clone (void);

  void operator=(const TAO_Notify_Any& notify_any);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL);
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                        const TAO_Notify_QoSAdmin_i& qos_admin,
                        CosNotification::EventBatch& unsent,
                        int flush_queue
                        ACE_ENV_ARG_DECL) const;

protected:
  /// The data
  CORBA::Any* data_;

  /// Do we own the data.
  CORBA::Boolean is_owner_;
};

// ****************************************************************

/**
 * @class TAO_Notify_StructuredEvent
 *
 * @brief TAO_Notify_StructuredEvent
 *
 * This class is the concrete prototype for the Structured Event Type.
 */
class TAO_Notify_Export TAO_Notify_StructuredEvent : public TAO_Notify_Event
{
public:
  TAO_Notify_StructuredEvent (CosNotification::StructuredEvent * notification);
  TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent * notification);
  virtual ~TAO_Notify_StructuredEvent ();

  virtual TAO_Notify_Event* clone (void);
  void operator=(const TAO_Notify_StructuredEvent &structured_event);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL);
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                        const TAO_Notify_QoSAdmin_i& qos_admin,
                        CosNotification::EventBatch& unsent,
                        int flush_queue
                        ACE_ENV_ARG_DECL) const;

protected:

  /// Load the QoS properties specified for this event from <data_>.
  void init_QoS (void);

  // = Data Members
  /// The data
  CosNotification::StructuredEvent* data_;

  /// The event type of <data_>
  TAO_Notify_EventType event_type_;

  /// Do we own the data.
  CORBA::Boolean is_owner_;
};


#if defined (__ACE_INLINE__)
#include "Notify_Event.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_NOTIFY_EVENT_H */
