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
#include "Notify_Signal_Property_T.h"


typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX,CORBA::Long> TAO_Notify_Property_Long;
typedef TAO_Notify_Signal_Property <TAO_SYNCH_MUTEX,CORBA::Long> TAO_Notify_Signal_Property_Long;

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
  virtual ~TAO_Notify_AdminProperties ();

  virtual CosNotification::AdminProperties * get_admin (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_admin (
    const CosNotification::AdminProperties & admin
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedAdmin
  ));

  // = Accessors
  // There was no reason for these not to be const
  CORBA::Long max_queue_length (void) const;
  CORBA::Long max_consumers (void) const;
  CORBA::Long max_suppliers (void) const;
  CORBA::Boolean reject_new_events (void) const;

  TAO_Notify_Signal_Property_Long* queue_length (void);
  TAO_Notify_Property_Long* consumers (void);
  TAO_Notify_Property_Long* suppliers (void);

  // = Helper method
  CORBA::Boolean queue_full (void);
  // Returns true if Queue is full

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
  TAO_Notify_Signal_Property_Long queue_length_;
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
