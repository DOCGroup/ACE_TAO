/* -*- C++ -*- */
/**
 *  @file AdminProperties.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_ADMINPROPERTIES_H
#define TAO_Notify_ADMINPROPERTIES_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "tao/orbconf.h"
#include "PropertySeq.h"
#include "Property_T.h"
#include "Property.h"
#include "Property_Boolean.h"

/**
 * @class TAO_Notify_AdminProperties
 *
 * @brief The AdminProperties per EventChannel.
 *
 */
class TAO_Notify_Export TAO_Notify_AdminProperties : public TAO_Notify_PropertySeq
{
public:
  /// Constuctor
  TAO_Notify_AdminProperties (void);

  /// Destructor
  ~TAO_Notify_AdminProperties ();

  // Init
  int init (const CosNotification::PropertySeq& prop_seq);

  // = Accessors
  const TAO_Notify_Property_Long& max_global_queue_length (void) const;
  const TAO_Notify_Property_Long& max_consumers (void) const;
  const TAO_Notify_Property_Long& max_suppliers (void) const;
  const TAO_Notify_Property_Boolean& reject_new_events (void) const;

  CORBA::Long& global_queue_length (void);
  TAO_SYNCH_MUTEX& global_queue_lock (void);
  TAO_SYNCH_CONDITION& global_queue_not_full_condition (void);

  TAO_Notify_Atomic_Property_Long& consumers (void);
  TAO_Notify_Atomic_Property_Long& suppliers (void);

  // = Helper method
  /// Returns true if Queue is full
  CORBA::Boolean queue_full (void);

protected:
  // @@ Pradeep can you explain why there is any maximum for these
  // values? Should they be configurable by the user so the resource
  // requirements can be bounded?

  // = Admin. properties
  // for all these properties the default O implies no limit
  /**
   * The maximum number of events that will be queued by the channel before
   * the channel begins discarding events or rejecting new events upon
   * receipt of each new event.
   */
  TAO_Notify_Property_Long max_global_queue_length_;

  /// The maximum number of consumers that can be connected to the channel at
  /// any given time.
  TAO_Notify_Property_Long max_consumers_;

  /// The maximum number of suppliers that can be connected to the channel at
  /// any given time.
  TAO_Notify_Property_Long max_suppliers_;

  /// Reject any new event.
  TAO_Notify_Property_Boolean reject_new_events_;

  //= Variables
  /// This is used to count the queue length across all buffers in the Notify Service
  /// to enforce the "MaxQueueLength" property.
  CORBA::Long global_queue_length_;

  /// Global queue lock used to serialize access to all queues.
  TAO_SYNCH_MUTEX global_queue_lock_;

  /// The condition that the queue_length_ is not at max.
  TAO_SYNCH_CONDITION global_queue_not_full_condition_;

  /// These are used to count the number of consumers and suppliers connected to
  /// the system.
  TAO_Notify_Atomic_Property_Long consumers_;
  TAO_Notify_Atomic_Property_Long suppliers_;
};

typedef ACE_Refcounted_Auto_Ptr<TAO_Notify_AdminProperties, TAO_SYNCH_MUTEX> TAO_Notify_AdminProperties_var;

#if defined (__ACE_INLINE__)
#include "AdminProperties.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_ADMINPROPERTIES_H */
