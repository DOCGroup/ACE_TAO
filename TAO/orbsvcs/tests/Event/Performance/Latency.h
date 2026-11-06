/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Latency.h
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
  EC_Latency_Consumer (EC_Latency_Consumer const&) = delete;
  EC_Latency_Consumer (EC_Latency_Consumer&&) = delete;
  EC_Latency_Consumer& operator= (EC_Latency_Consumer const&) = delete;
  EC_Latency_Consumer& operator= (EC_Latency_Consumer&&) = delete;

  /// Return 1 when all the messages have been received
  int done () const;

  void push (RtecEventComm::EventSet const& events) override;
  void disconnect_push_consumer () override;

private:
  /// Roundtrip delays are recorded here
  /// Non-owning; the history outlives this servant.
  ACE_Sample_History *history_;

  /// Use this mutex to synchronize access to history_
  /// Non-owning; the mutex outlives this servant.
  TAO_SYNCH_MUTEX *mutex_;

  /// Number of messages yet to be received
  int remaining_messages_ {};
};

// ****************************************************************

class EC_Latency_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  EC_Latency_Supplier () = default;
  EC_Latency_Supplier (EC_Latency_Supplier const&) = delete;
  EC_Latency_Supplier (EC_Latency_Supplier&&) = delete;
  EC_Latency_Supplier& operator= (EC_Latency_Supplier const&) = delete;
  EC_Latency_Supplier& operator= (EC_Latency_Supplier&&) = delete;

  void disconnect_push_supplier () override;
};

// ****************************************************************

/// Send events in another thread
class Task : public ACE_Task_Base
{
public:
  /// Constructor
  Task (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
        int iterations);
  Task (Task const&) = delete;
  Task (Task&&) = delete;
  Task& operator= (Task const&) = delete;
  Task& operator= (Task&&) = delete;

  /// Return 1 when all the messages have been sent
  int done () const;

  /// Run the experiment
  int svc () override;

private:
  /// The consumer
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;

  /// Number of messages that have to be sent
  int remaining_messages_ {};

  /// Synchronize access to remaining_messages_
  mutable TAO_SYNCH_MUTEX mutex_;
};

#endif /* EC_LATENCY_H */
