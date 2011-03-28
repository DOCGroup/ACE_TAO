/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Latency
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_LATENCY_H
#define EC_LATENCY_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Sample_History;
ACE_END_VERSIONED_NAMESPACE_DECL

/// Simple consumer, receives events and record roundtrip delays.
class EC_Latency_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  EC_Latency_Consumer (ACE_Sample_History *history,
                       TAO_SYNCH_MUTEX *mutex,
                       int message_count);

  /// Return 1 when all the messages have been received
  int done (void);

  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);

private:
  /// Roundtrip delays are recorded here
  ACE_Sample_History *history_;

  /// Use this mutex to synchronize access to history_
  TAO_SYNCH_MUTEX *mutex_;

  /// Number of messages yet to be received
  int remaining_messages_;
};

// ****************************************************************

class EC_Latency_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  EC_Latency_Supplier (void);

  virtual void disconnect_push_supplier (void);
};

// ****************************************************************

/// Send events in another thread
class Task : public ACE_Task_Base
{
public:
  /// Constructor
  Task (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
        int iterations);

  /// Return 1 when all the messages have been sent
  int done (void);

  /// Run the experiment
  int svc (void);

private:
  /// The consumer
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;

  /// Number of messages that have to be sent
  int remaining_messages_;

  /// Synchronize access to remaining_messages_
  TAO_SYNCH_MUTEX mutex_;
};

#endif /* EC_LATENCY_H */
