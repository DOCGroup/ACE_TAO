/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_QoSAdmin_i.h
//
// = DESCRIPTION
//   Implements get/set methods for QoS properties.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_QOSADMIN_I_H
#define TAO_NOTIFY_QOSADMIN_I_H
#include "ace/pre.h"

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/TimeBaseC.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Export TAO_Notify_QoSAdmin_i
{
  // = TITLE
  //   TAO_Notify_QoSAdmin_i
  //
  // = DESCRIPTION
  //
  //
public:
  TAO_Notify_QoSAdmin_i (void);
  // Constructor

  virtual ~TAO_Notify_QoSAdmin_i (void);
  // Destructor

  virtual CosNotification::QoSProperties * get_qos (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

protected:
  // = QoS Properties
  CORBA::Short event_reliability_;
  CORBA::Short connection_reliability_;
  // Reliability properties (to be implemented "later").

  CORBA::Short priority_;
  // Priority for the event

  /*
  TimeBase::UtcT start_time_;
  // Earliest delivery time

  TimeBase::UtcT stop_time_;

  */

  TimeBase::TimeT timeout_;
  // Expiry time

  CORBA::Boolean start_time_supported_;
  CORBA::Boolean stop_time_supported_;
  // Are start/stop times supported

  CORBA::Long max_events_per_consumer_;
  // Max events allowed to be queued on behalf of a consumer

  CORBA::Short order_policy_;
  // Order of events in internal buffers.

  CORBA::Short discard_policy_;
  // Policy to discard when buffers are full.

  CORBA::Long maximum_batch_size_;
  // Batch size for sequences of structured events.

  TimeBase::TimeT pacing_interval_;
  // Max. period of time that events are collected before delivering
  // them as a sequence of structured events.
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_QOSADMIN_I_H */
