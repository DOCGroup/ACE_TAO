/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_AdminProperties.h
//
// = DESCRIPTION
//   Implements get/set methods for EC Admin properties.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_ADMINPROPERTIES_H
#define TAO_NOTIFY_ADMINPROPERTIES_H

#include "ace/pre.h"
#include "orbsvcs/CosNotificationC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Listeners.h"
#include "notify_export.h"

typedef ACE_Atomic_Op <ACE_SYNCH_MUTEX,CORBA::Long> TAO_Notify_Property_Long;

class TAO_Notify_Export TAO_Notify_AdminProperties
{
  // = TITLE
  //   TAO_Notify_AdminProperties
  //
  // = DESCRIPTION
  //   All the EC Admin properties are get/set via this class.
  //
public:
  TAO_Notify_AdminProperties (void);
  ~TAO_Notify_AdminProperties ();

  virtual CosNotification::AdminProperties * get_admin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_admin (
    const CosNotification::AdminProperties & admin,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedAdmin
  ));

  // = Accessors
  CORBA::Long max_queue_length (void);
  CORBA::Long max_consumers (void);
  CORBA::Long max_suppliers (void);
  CORBA::Boolean reject_new_events (void);

  TAO_Notify_Property_Long* const queue_length (void);
  TAO_Notify_Property_Long* const consumers (void);
  TAO_Notify_Property_Long* const suppliers (void);

protected:
  // @@ Pradeep can you explain why there is any maximum for these
  // values? Should they be configurable by the user so the resource
  // requirements can be bounded?

  // = Admin. properties
  // for all these properties the default O implies no limit
  CORBA::Long max_queue_length_;
  // The maximum number of events that will be queued by the channel before
  // the channel begins discarding events or rejecting new events upon
  // receipt of each new event.

  CORBA::Long max_consumers_;
  // The maximum number of consumers that can be connected to the channel at
  // any given time.

  CORBA::Long max_suppliers_;
  // The maximum number of suppliers that can be connected to the channel at
  // any given time.

  CORBA::Boolean reject_new_events_;
  // Reject any new event.

  //= Variables
  TAO_Notify_Property_Long queue_length_;
  // This is used to count the queue length across all buffers in the Notify Service
  // to enforce the "MaxQueueLength" property.

  TAO_Notify_Property_Long consumers_;
  TAO_Notify_Property_Long suppliers_;
  // These are used to count the number of consumers and suppliers connected to
  // the system.
};


#if defined (__ACE_INLINE__)
#include "Notify_AdminProperties.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_ADMINPROPERTIES_H */
