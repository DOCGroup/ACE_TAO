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

#ifndef TAO_NS_ADMINPROPERTIES_H
#define TAO_NS_ADMINPROPERTIES_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"
#include "tao/orbconf.h"
#include "Types.h"
#include "PropertySeq.h"
#include "Property_T.h"
#include "Property.h"
#include "Property_Boolean.h"
#include "Notify_Signal_Property_T.h"

/**
 * @class TAO_NS_AdminProperties
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_AdminProperties : public TAO_NS_PropertySeq
{
public:
  /// Constuctor
  TAO_NS_AdminProperties (void);

  /// Destructor
  ~TAO_NS_AdminProperties ();

  // Init
  int init (const CosNotification::PropertySeq& prop_seq);

  // = Accessors
  // There was no reason for these not to be const
  const TAO_NS_Property_Long& max_queue_length (void) const;
  const TAO_NS_Property_Long& max_consumers (void) const;
  const TAO_NS_Property_Long& max_suppliers (void) const;
  const TAO_NS_Property_Boolean& reject_new_events (void) const;

  TAO_NS_Signal_Property_Long& queue_length (void);
  TAO_NS_Atomic_Property_Long& consumers (void);
  TAO_NS_Atomic_Property_Long& suppliers (void);

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
  TAO_NS_Property_Long max_queue_length_;

  /// The maximum number of consumers that can be connected to the channel at
  /// any given time.
  TAO_NS_Property_Long max_consumers_;

  /// The maximum number of suppliers that can be connected to the channel at
  /// any given time.
  TAO_NS_Property_Long max_suppliers_;

  /// Reject any new event.
  TAO_NS_Property_Boolean reject_new_events_;

  //= Variables
  /// This is used to count the queue length across all buffers in the Notify Service
  /// to enforce the "MaxQueueLength" property.
  TAO_NS_Signal_Property_Long queue_length_;

  /// These are used to count the number of consumers and suppliers connected to
  /// the system.
  TAO_NS_Atomic_Property_Long consumers_;
  TAO_NS_Atomic_Property_Long suppliers_;
};

#if defined (__ACE_INLINE__)
#include "AdminProperties.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_ADMINPROPERTIES_H */
