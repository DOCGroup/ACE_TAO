/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 * It simply register for two event typesone event type
 * The class is just a helper to simplify common tasks in EC
 * tests, such as subscribing for a range of events, disconnecting
 * from the EC, informing the driver of shutdown messages, etc.
 * There are several ways to connect and disconnect this class,
 * and it is up to the driver program to use the right one.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier (RtecEventComm::EventSourceID id,
            RtecEventChannelAdmin::ProxyPushConsumer_ptr const consumer_proxy);
  Supplier (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier &operator= (Supplier &&) = delete;

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  void disconnect_push_supplier () override;

  void timeout_occurred ();

private:
  RtecEventComm::EventSourceID id_;
  RtecEventChannelAdmin::ProxyPushConsumer_ptr const consumer_proxy_;
};

/**
 * @class Timeout_Consumer
 *
 * @brief Timer consumer object
 *
 * This class is a consumer of timeout events from EC.
 * It registers for timeout event with EC and calls
 * the
 */
class Timeout_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  explicit Timeout_Consumer (Supplier *supplier_impl);
  Timeout_Consumer (Timeout_Consumer const &) = delete;
  Timeout_Consumer (Timeout_Consumer &&) = delete;
  Timeout_Consumer &operator= (Timeout_Consumer const &) = delete;
  Timeout_Consumer &operator= (Timeout_Consumer &&) = delete;

  // = The RtecEventComm::PushConsumer methods

  /// The skeleton methods.
  void push (RtecEventComm::EventSet const &events) override;
  void disconnect_push_consumer () override;

private:
  Supplier *supplier_impl_;
};

#endif /* SUPPLIER_H */
